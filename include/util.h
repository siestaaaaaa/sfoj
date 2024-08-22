#pragma once

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <atomic>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace util {

class path_util {
 public:
  static std::string src_path(const std::string& filename) {
    return add_prefix_and_suffix(filename, ".cpp");
  }

  static std::string exe_path(const std::string& filename) {
    return add_prefix_and_suffix(filename, ".exe");
  }

  static std::string compile_error_path(const std::string& filename) {
    return add_prefix_and_suffix(filename, ".compile_error");
  }

  // run-time
  static std::string stdin_path(const std::string& filename) {
    return add_prefix_and_suffix(filename, ".stdin");
  }
  static std::string stdout_path(const std::string& filename) {
    return add_prefix_and_suffix(filename, ".stdout");
  }
  static std::string stderr_path(const std::string& filename) {
    return add_prefix_and_suffix(filename, ".stderr");
  }

 private:
  static inline const std::string prefix = "./temp/";

  static std::string add_prefix_and_suffix(const std::string& filename,
                             const std::string& suffix) {
    auto res = prefix;
    res += filename;
    res += suffix;
    return res;
  }
};

class time_util {
 public:
  static std::string get_time() {
    auto tm = time(nullptr);
    auto cur = localtime(&tm);
    std::string buf;
    buf.resize(100);
    auto len = std::snprintf(&buf[0], buf.size(), "%d-%02d-%02d %02d:%02d:%02d",
                             cur->tm_year + 1900, cur->tm_mon + 1, cur->tm_mday,
                             cur->tm_hour, cur->tm_min, cur->tm_sec);
    buf.resize(len);
    return buf;
  }

  static std::string get_time_stamp_ms() {
    timeval _time;
    gettimeofday(&_time, nullptr);
    return std::to_string(_time.tv_sec * 1000 + _time.tv_usec / 1000);
  }
};

class file_util {
 public:
  static bool is_exist(const std::string path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
      return true;
    }
    return false;
  }

  static std::string unique_name() {
    static std::atomic_uint id{};
    ++id;
    auto uid = std::to_string(id);
    auto ms = time_util::get_time_stamp_ms();
    return ms + "." + uid;
  }

  static bool write_to_path(const std::string& path, const std::string& in) {
    std::ofstream ofs(path);
    if (!ofs.is_open()) {
      return false;
    }
    ofs.write(in.c_str(), in.size());
    ofs.close();
    return true;
  }

  static bool read_from_path(const std::string& path, std::string& out) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
      return false;
    }
    std::string line;
    while (std::getline(ifs, line)) {
      out += line;
      out += '\n';
    }
    return true;
  }
};

class string_util {
 public:
  static void split(const std::string s, std::vector<std::string>& res,
                    char sep = ' ') {
    std::stringstream ssin(s);
    std::string t;
    while (std::getline(ssin, t, sep)) {
      if (!t.empty()) {
        res.push_back(t);
      }
    }
  }
};

enum { INFO, DEBUG, WARNING, ERROR, FATAL };

class log_util {
 public:
  static std::ostream& log(const std::string& level,
                           const std::string& filename, int line) {
    std::string msg = "[";
    msg += level;
    msg += "][";
    msg += filename;
    msg += ":";
    msg += std::to_string(line);
    msg += "][";
    msg += time_util::get_time();
    msg += "] ";

    std::cout << msg;
    return std::cout;
  }
};
}  // namespace util