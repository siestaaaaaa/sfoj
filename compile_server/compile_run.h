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
    auto filename = file_util::gen_filename();
    if (!file_util::write(path_util::src(filename), code)) {
      out_val["result"] = "Unknown Error"s;
      out_val["info"] = "";
      clean(filename);
      return out_json(out_val);
    }

    if (!compiler::start(filename)) {
      std::string temp;
      file_util::read(path_util::compile_error(filename), temp);
      out_val["result"] = "Complie Error"s;
      out_val["info"] = temp;
      clean(filename);
      return out_json(out_val);
    }

    int ret = runner::start(filename, cpu_limit, mem_limit);
    if (ret < 0) {
      out_val["result"] = "Unknown Error"s;
      out_val["info"] = "";
      clean(filename);
      return out_json(out_val);
    } else if (ret > 0) {
      out_val["result"] = handler(ret);
      out_val["info"] = "";
      clean(filename);
      return out_json(out_val);
    } else {
      out_val["result"] = "Finished"s;
      out_val["info"] = "";
      std::string _stdout, _stderr;
      file_util::read(path_util::_stdout(filename), _stdout);
      file_util::read(path_util::_stderr(filename), _stderr);
      out_val["stdout"] = _stdout;
      out_val["stderr"] = _stderr;
      clean(filename);
      return out_json(out_val);
    }
  }

 private:
  static void clean(const std::string& filename) {
    auto src = path_util::src(filename);
    if (file_util::is_exist(src)) {
      unlink(src.c_str());
    }

    auto compile_error = path_util::compile_error(filename);
    if (file_util::is_exist(compile_error)) {
      unlink(compile_error.c_str());
    }

    auto exe = path_util::exe(filename);
    if (file_util::is_exist(exe)) {
      unlink(exe.c_str());
    }

    auto __stdin = path_util::_stdin(filename);
    if (file_util::is_exist(__stdin)) {
      unlink(__stdin.c_str());
    }

    auto __stdout = path_util::_stdout(filename);
    if (file_util::is_exist(__stdout)) {
      unlink(__stdout.c_str());
    }

    auto __stderr = path_util::_stderr(filename);
    if (file_util::is_exist(__stderr)) {
      unlink(__stderr.c_str());
    }
  }

  static std::string out_json(const Json::Value& out_val) {
    Json::StyledWriter writer;
    return writer.write(out_val);
  }

  static std::string handler(int signo) {
    std::string msg;
    switch (signo) {
      case SIGXCPU:
        msg = "Time Limit Exceeded";
        break;
      case SIGFPE:
        msg = "Float Point Exception ";
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