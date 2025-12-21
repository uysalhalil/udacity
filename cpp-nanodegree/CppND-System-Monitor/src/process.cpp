#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  long total_jiffies = LinuxParser::ActiveJiffies(pid_);
  long proc_seconds = LinuxParser::UpTime(pid_);
  long hertz = sysconf(_SC_CLK_TCK);
  if (hertz <= 0 || proc_seconds <= 0) {
    cpu_usage_ = 0.0f;
    return cpu_usage_;
  }

  float total_seconds =
      static_cast<float>(total_jiffies) / static_cast<float>(hertz);
  cpu_usage_ = total_seconds / static_cast<float>(proc_seconds);
  return cpu_usage_;
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if (cpu_usage_ != a.cpu_usage_) return cpu_usage_ > a.cpu_usage_;
  return pid_ < a.pid_;
}