#pragma once

#include <fcntl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "../include/util.h"

namespace run {
using namespace util;

#define LOG(level) log_util::log(#level, __FILE__, __LINE__)

class runner {
 public:
  // < 0: syscall failed
  // > 0: signal
  // == 0: ok
  static int start(const std::string& filename, int cpu_limit,
                   int mem_limit /*KB*/) {
    auto stdin_path = path_util::stdin_path(filename);
    auto stdout_path = path_util::stdout_path(filename);
    auto stderr_path = path_util::stderr_path(filename);

    auto stdin_fd = open(stdin_path.c_str(), O_CREAT | O_RDONLY, 0644);
    auto stdout_fd = open(stdout_path.c_str(), O_CREAT | O_WRONLY, 0644);
    auto stderr_fd = open(stderr_path.c_str(), O_CREAT | O_WRONLY, 0644);
    if (stdin_fd < 0 || stdout_fd < 0 || stderr_fd < 0) {
      LOG(WARNING) << "open() failed\n";
      return -1;
    }

    auto pid = fork();
    if (pid < 0) {
      close(stdin_fd);
      close(stdout_fd);
      close(stderr_fd);
      LOG(ERROR) << "fork() failed\n";
      return -2;
    } else if (pid == 0) {
      // close fd2, and make fd2 point to the file that fd points to
      dup2(stdin_fd, 0);
      dup2(stdout_fd, 1);
      dup2(stderr_fd, 2);
      
      set_limit(cpu_limit, mem_limit);

      auto exe_path = path_util::exe_path(filename);
      execl(exe_path.c_str(), exe_path.c_str(), nullptr);

      LOG(ERROR) << "execl() failed\n";
      exit(1);
    } else {
      close(stdin_fd);
      close(stdout_fd);
      close(stderr_fd);

      int status = 0;
      waitpid(pid, &status, 0);
      LOG(INFO) << "run() finished\n";
      return status & 0x7f;
    }
  }

 private:
  static void set_limit(int cpu, int mem) {
    struct rlimit cpu_rlimit;
    cpu_rlimit.rlim_max = RLIM_INFINITY;
    cpu_rlimit.rlim_cur = cpu;
    setrlimit(RLIMIT_CPU, &cpu_rlimit);

    struct rlimit mem_rlimit;
    mem_rlimit.rlim_max = RLIM_INFINITY;
    mem_rlimit.rlim_cur = mem * 1024;
    setrlimit(RLIMIT_AS, &mem_rlimit);
  }
};

#undef LOG
}  // namespace run