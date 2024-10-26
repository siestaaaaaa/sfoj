#pragma once

#include <fcntl.h>
#include <sys/wait.h>

#include "../include/util.h"

namespace compile {
using namespace util;

#define LOG(level) log_util::log(#level, __FILE__, __LINE__)

class compiler {
 public:
  static bool start(const std::string& filename) {
    auto pid = fork();
    if (pid < 0) {
      LOG(ERROR) << "fork() failed\n";
      return false;
    } else if (pid == 0) {
      auto compile_error_fd = open(path_util::compile_error_path(filename).c_str(),
                                   O_CREAT | O_WRONLY | O_TRUNC, 0644);
      if (compile_error_fd < 0) {
        LOG(WARNING) << "open() failed\n";
        exit(1);
      }
      dup2(compile_error_fd, 2);

      execlp("g++", "g++", path_util::src_path(filename).c_str(), "-o",
             path_util::exe_path(filename).c_str(), "-std=c++17", nullptr);

      LOG(ERROR) << "execlp() failed\n";
      exit(2);
    } else {
      waitpid(pid, nullptr, 0);
      if (file_util::is_exist(path_util::exe_path(filename))) {
        LOG(INFO) << "compile() finished\n";
        return true;
      }

      LOG(ERROR) << "compile() failed\n";
      return false;
    }
  }
};

#undef LOG
}  // namespace compile