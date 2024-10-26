#pragma once

#include <jsoncpp/json/json.h>
#include <unistd.h>

#include <csignal>

#include "../include/util.h"
#include "compile.h"
#include "run.h"

namespace compile_run {
using namespace compile;
using namespace run;
using namespace util;
using namespace std::string_literals;

class compile_and_run {
 public:
  static std::string start(const std::string& in_json) {
    Json::Value in_val;
    Json::Reader reader;
    reader.parse(in_json, in_val);
    auto code = in_val["code"].asString();
    auto cpu_limit = in_val["cpu_limit"].asInt();
    auto mem_limit = in_val["mem_limit"].asInt();

    Json::Value out_val;
    auto filename = file_util::unique_name();
    if (!file_util::write_to_path(path_util::src_path(filename), code)) {
      out_val["result"] = "Unknown Error"s;
      out_val["info"] = "";
      clean(filename);
      return out_json(out_val);
    }

    if (!compiler::start(filename)) {
      std::string temp;
      file_util::read_from_path(path_util::compile_error_path(filename), temp);
      out_val["result"] = "Complie Error"s;
      out_val["info"] = temp;
      clean(filename);
      return out_json(out_val);
    }

    int ret = runner::start(filename, cpu_limit, mem_limit);
    if (ret < 0) {
      //syscall failed
      out_val["result"] = "Unknown Error"s;
      out_val["info"] = "";
      clean(filename);
      return out_json(out_val);
    } else if (ret > 0) {
      //signal
      out_val["result"] = signal_msg(ret);
      out_val["info"] = "";
      clean(filename);
      return out_json(out_val);
    } else {
      //ok
      out_val["result"] = "Finished"s;
      out_val["info"] = "";
      std::string _stdout, _stderr;
      file_util::read_from_path(path_util::stdout_path(filename), _stdout);
      file_util::read_from_path(path_util::stderr_path(filename), _stderr);
      out_val["stdout"] = _stdout;
      out_val["stderr"] = _stderr;
      clean(filename);
      return out_json(out_val);
    }
  }

 private:
  static void clean(const std::string& filename) {
    auto src_path = path_util::src_path(filename);
    if (file_util::is_exist(src_path)) {
      unlink(src_path.c_str());
    }

    auto compile_error_path = path_util::compile_error_path(filename);
    if (file_util::is_exist(compile_error_path)) {
      unlink(compile_error_path.c_str());
    }

    auto exe_path = path_util::exe_path(filename);
    if (file_util::is_exist(exe_path)) {
      unlink(exe_path.c_str());
    }

    auto stdin_path = path_util::stdin_path(filename);
    if (file_util::is_exist(stdin_path)) {
      unlink(stdin_path.c_str());
    }

    auto stdout_path = path_util::stdout_path(filename);
    if (file_util::is_exist(stdout_path)) {
      unlink(stdout_path.c_str());
    }

    auto stderr_path = path_util::stderr_path(filename);
    if (file_util::is_exist(stderr_path)) {
      unlink(stderr_path.c_str());
    }
  }

  static std::string out_json(const Json::Value& out_val) {
    Json::StyledWriter writer;
    return writer.write(out_val);
  }

  static std::string signal_msg(int signo) {
    std::string msg;
    switch (signo) {
      case SIGXCPU:
        msg = "Time Limit Exceeded";
        break;
      case SIGFPE:
        msg = "Float Point Exception";
        break;
      case SIGSEGV:
        msg = "Segmentation Fault";
        break;
      default:
        msg = "Runtime Error";
        break;
    }
    return msg;
  }
};
}  // namespace compile_run