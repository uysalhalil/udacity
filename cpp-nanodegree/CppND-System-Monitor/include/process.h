#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process() = delete;
  explicit Process(int pid) : pid_(pid) {
    command_ = LinuxParser::Command(pid);
    user_ = LinuxParser::User(pid);
    ram_ = LinuxParser::Ram(pid);
    uptime_ = LinuxParser::UpTime(pid);
  }
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
  int pid_{0};
  float cpu_usage_{0.0};
  std::string command_{""};
  std::string ram_{""};
  std::string user_{""};
  long int uptime_{0};
};

#endif