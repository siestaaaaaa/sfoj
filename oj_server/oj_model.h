#pragma once

#include <cassert>
#include <optional>
#include <unordered_map>
#include <vector>

#include "../include/util.h"

namespace oj_model {
using namespace util;

#define LOG(level) log_util::log(#level, __FILE__, __LINE__)

struct problem {
  std::string id_;
  std::string name_;
  std::string level_;
  int cpu_limit_;
  int mem_limit_;
  std::string description_;
  std::string header_;
  std::string solution_;
  std::string test_;
};

const std::string problem_path = "./problem/";
const std::string problem_list_path = "./problem/problem_list.conf";

class model {
  std::map<std::string, problem> problemset;

 public:
  model() { assert(load_problem_list()); }

  bool load_problem_list() {
    std::ifstream ifs(problem_list_path);
    if (!ifs.is_open()) {
      LOG(FATAL) << "problem_list.conf not found\n";
      return false;
    }

    std::string line;
    while (std::getline(ifs, line)) {
      std::vector<std::string> tokens;
      string_util::split(line, tokens);
      if (tokens.size() != 5) {
        LOG(WARNING) << "problem_list.conf format invalid\n";
        continue;
      }

      problem pblm;
      pblm.id_ = tokens[0];
      pblm.name_ = tokens[1];
      pblm.level_ = tokens[2];
      pblm.cpu_limit_ = std::stoi(tokens[3]);
      pblm.mem_limit_ = std::stoi(tokens[4]) * 1024;

      auto resource_path = problem_path;
      resource_path += pblm.id_;
      resource_path += "/";

      file_util::read(resource_path + "description.txt", pblm.description_);
      file_util::read(problem_path + "header.cpp", pblm.header_);
      file_util::read(resource_path + "solution.cpp", pblm.solution_);
      file_util::read(resource_path + "test.cpp", pblm.test_);

      problemset[pblm.id_] = pblm;
    }
    ifs.close();
    LOG(INFO) << "load_problem_list() finished\n";
    return true;
  }

  std::optional<problem> get_one_problem(const std::string& id) {
    if (problemset.count(id) == 0) {
      LOG(ERROR) << "problem " << id << " not found\n";
      return {};
    }

    return problemset[id];
  }

  std::optional<std::vector<problem>> get_all_problem() {
    if (problemset.empty()) {
      LOG(ERROR) << "problemset is empty\n";
      return {};
    }

    std::vector<problem> res;
    for (const auto& [id, problem] : problemset) {
      res.push_back(problem);
    }
    return res;
  }
};

#undef LOG
}  // namespace oj_model