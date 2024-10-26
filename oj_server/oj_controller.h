#pragma once

#include <jsoncpp/json/json.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>

#include "../include/httplib.h"
#include "oj_model.h"
#include "oj_view.h"

namespace oj_controller {
using namespace util;
using namespace oj_model;
using namespace oj_view;

#define LOG(level) log_util::log(#level, __FILE__, __LINE__)

struct machine {
  std::string ip_;
  int port_;
  uint64_t usage_;
  std::shared_ptr<std::mutex> pmtx_;

  void usage_up() {
    if (pmtx_.use_count()) pmtx_->lock();
    ++usage_;
    if (pmtx_.use_count()) pmtx_->unlock();
  }

  void usage_down() {
    if (pmtx_.use_count()) pmtx_->lock();
    --usage_;
    if (pmtx_.use_count()) pmtx_->unlock();
  }

  void usage_reset() {
    if (pmtx_.use_count()) pmtx_->lock();
    usage_ = 0;
    if (pmtx_.use_count()) pmtx_->unlock();
  }

  auto get_usage() {
    decltype(usage_) res = 0;
    if (pmtx_.use_count()) pmtx_->lock();
    res = usage_;
    if (pmtx_.use_count()) pmtx_->unlock();
    return res;
  }
};

class scheduler {
  std::vector<machine> machines_;
  std::vector<int> online_;
  std::vector<int> offline_;
  std::mutex mtx_;

  static inline const std::string machine_list_path = "./conf/machine_list.conf";

 public:
  scheduler() { assert(load_machine_list()); }

  std::optional<std::tuple<int, machine*>>
  select() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (online_.empty()) {
      LOG(FATAL) << "all machines are offline\n";
      return {};
    }

    auto id = online_[0];
    auto m = &machines_[id];
    auto min_usage = machines_[id].get_usage();
    for (size_t i = 0; i < online_.size(); ++i) {
      auto tmp = machines_[online_[i]].get_usage();
      if (min_usage > tmp) {
        min_usage = tmp;
        id = online_[i];
        m = &machines_[online_[i]];
      }
    }
    return std::make_tuple(id, m);
  }

  void online() {
    std::lock_guard<std::mutex> lock(mtx_);
    online_.insert(online_.end(), offline_.begin(), offline_.end());
  }

  void offline(int id) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = std::find(online_.begin(), online_.end(), id);
    if (it != online_.end()) {
      machines_[id].usage_reset();
      online_.erase(it);
      offline_.push_back(id);
    }
  }

 private:
  bool load_machine_list() {
    std::ifstream ifs(machine_list_path);
    if (!ifs.is_open()) {
      LOG(FATAL) << "machine_list.conf not found\n";
      return false;
    }

    std::string line;
    while (std::getline(ifs, line)) {
      std::vector<std::string> tokens;
      string_util::split(line, tokens, ':');
      if (tokens.size() != 2) {
        LOG(WARNING) << "machine_list.conf format invalid\n";
        continue;
      }

      machine m;
      m.ip_ = tokens[0];
      m.port_ = std::stoi(tokens[1]);
      m.usage_ = 0;
      m.pmtx_ = std::make_shared<std::mutex>();

      online_.push_back(machines_.size());
      machines_.push_back(m);
    }
    ifs.close();
    LOG(INFO) << "load_machine_list() finished\n";
    return true;
  }
};

class controller {
  model model_;
  view view_;
  scheduler scheduler_;

 public:
  void online_service() {
    scheduler_.online();
    LOG(INFO) << "all machines are online again\n";
  }

  std::string get_problem(const std::string& id) {
    std::string res;
    auto problem = model_.get_problem(id);
    if (problem.has_value()) {
      view_.expand(problem.value(), res);
    } else {
      res = "problem ";
      res += id;
      res += " not found\n";
    }
    return res;
  }

  std::string get_problemset() {
    std::string res;
    auto problemset = model_.get_problemset();
    if (problemset.has_value()) {
      view_.expand(problemset.value(), res);
    } else {
      res = "problemset not found\n";
    }
    return res;
  }

  std::string judge(const std::string& id, const std::string& in_json) {
    auto problem = model_.get_problem(id);
    if (!problem.has_value()) {
      return {};
    }

    Json::Value in_val;
    Json::Reader reader;
    reader.parse(in_json, in_val);

    Json::Value out_val;
    auto solution = in_val["code"].asString();
    out_val["code"] = problem.value().header_ + solution + problem.value().test_;
    out_val["cpu_limit"] = problem.value().cpu_limit_;
    out_val["mem_limit"] = problem.value().mem_limit_;

    Json::FastWriter writer;
    auto out_json = writer.write(out_val);

    std::string res;
    while (true) {
      auto _ = scheduler_.select();
      if (!_.has_value()) {
        break;
      }

      auto [id, m] = _.value();
      LOG(INFO) << "scheduler select " << m->ip_ << ':' << m->port_
                << ", usage: " << m->get_usage() << '\n';

      httplib::Client cli(m->ip_.c_str(), m->port_);
      m->usage_up();
      if (auto rp = cli.Post("/compile_run", out_json,
                                "application/json; charset=utf-8")) {
        m->usage_down();
        if (rp->status == 200) {
          LOG(INFO) << "request compile_run success\n";
          res = rp->body;
          break;
        } else {
          LOG(INFO) << "request compile_run failed\n";
        }
      } else {
        LOG(ERROR) << m->ip_ << ':' << m->port_ << " is offline\n";
        scheduler_.offline(id);
      }
    }
    return res;
  }
};

#undef LOG
}  // namespace oj_controller