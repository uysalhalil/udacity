#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::getline;
using std::stof;
using std::string;
using std::to_string;
using std::vector;
using inputFileStream = std::ifstream;
using inputStringStream = std::istringstream;

long HZ = sysconf(_SC_CLK_TCK);

vector<long> LinuxParser::getStats() {
  string line, cpu;
  inputFileStream filestream(kProcDirectory + kStatFilename);
  if (!filestream.is_open()) {
    return {0};
  }
  if (!getline(filestream, line)) {
    return {0};
  }

  inputStringStream linestream(line);
  linestream >> cpu;
  vector<long> fields;
  long val;
  while (linestream >> val) {
    fields.push_back(val);
  }
  return fields;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line, key;
  long mem_total = 0;
  long mem_available = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "MemTotal:") {
        linestream >> mem_total;
      } else if (key == "MemAvailable:") {
        linestream >> mem_available;
      }
      if (mem_total > 0 && mem_available > 0) break;  // no need to continue
    }
  }
  if (mem_total <= 0) return 0.0;
  return static_cast<float>(mem_total - mem_available) /
         static_cast<float>(mem_total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<long> fields = getStats();
  long total_jiffies = 0;
  for (auto& jiffy : fields) {
    total_jiffies += jiffy;
  }
  return total_jiffies;
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  inputFileStream filestream(kProcDirectory + to_string(pid) + "/stat");
  if (!filestream.is_open()) {
    return 0;
  }
  getline(filestream, line);
  int close = line.rfind(")");
  if (static_cast<size_t>(close) == string::npos) {
    return 0;
  }

  inputStringStream linestream(line.substr(close + 2));
  vector<string> values;
  string value;
  while (linestream >> value) {
    values.push_back(value);
  }

  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;

  try {
    utime = std::stol(values[11]);
    stime = std::stol(values[12]);
    cutime = std::stol(values[13]);
    cstime = std::stol(values[14]);
  } catch (...) {
    return 0;
  }

  return utime + stime + cutime + cstime;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> fields = getStats();
  long active = 0;
  for (size_t i = 0; i < fields.size() && i < 8; ++i) {
    if (i == 3 || i == 4) {
      continue;
    }
    active += fields[i];
  }
  return active;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> fields = getStats();
  long idle = 0;
  for (size_t i = 3; i < 5 && i < fields.size(); ++i) {
    idle += fields[i];
  }
  return idle;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<long> fields = getStats();
  if (fields.empty() || (fields.size() == 1 && fields[0] == 0)) {
    return {};
  }

  vector<string> out;
  out.reserve(fields.size());
  for (size_t i = 0; i < fields.size(); ++i) {
    out.push_back(to_string(fields[i]));
  }
  return out;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int total_processes = 0;
  inputFileStream filestream(kProcDirectory + kStatFilename);
  if (!filestream.is_open()) {
    return 0;
  }
  while (getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "processes") {
      linestream >> total_processes;
      return total_processes;
    }
  }
  return total_processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int running_processes = 0;
  inputFileStream filestream(kProcDirectory + kStatFilename);
  if (!filestream.is_open()) {
    return 0;
  }
  while (getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "procs_running") {
      linestream >> running_processes;
      return running_processes;
    }
  }
  return running_processes;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  inputFileStream filestream(kProcDirectory + to_string(pid) + "/cmdline");
  if (!filestream.is_open()) {
    return string();
  }
  getline(filestream, line);
  if (!line.empty()) {
    return line;
  }
  return string();
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  inputFileStream filestream(kProcDirectory + to_string(pid) + "/status");
  if (!filestream.is_open()) {
    return string();
  }
  while (getline(filestream, line)) {
    std::istringstream linestream(line);
    string key;
    linestream >> key;
    if (key == "VmRSS:") {
      string ram_kb;
      linestream >> ram_kb;
      try {
        long ram_mb = std::stol(ram_kb) / 1024;
        return to_string(ram_mb);
      } catch (...) {
        return string();
      }
    }
  }
  return string();
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  inputFileStream filestream(kProcDirectory + to_string(pid) + "/status");
  if (!filestream.is_open()) {
    return string();
  }
  while (getline(filestream, line)) {
    std::istringstream linestream(line);
    string key;
    linestream >> key;
    if (key == "Uid:") {
      string uid;
      linestream >> uid;
      return uid;
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  if (uid.empty()) {
    return string();
  }
  string line;
  inputFileStream filestream(kPasswordPath);
  if (!filestream.is_open()) {
    return string();
  }
  while (getline(filestream, line)) {
    inputStringStream linestream(line);
    string username;
    string x;
    string file_uid;
    if (!getline(linestream, username, ':')) continue;
    if (!getline(linestream, x, ':')) continue;
    if (!getline(linestream, file_uid, ':')) continue;
    if (file_uid == uid) {
      return username;
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line{""};
  vector<string> values;
  inputFileStream filestream(kProcDirectory + std::to_string(pid) + "/stat");
  if (!filestream.is_open()) {
    return 0;
  }
  getline(filestream, line);
  size_t close = line.rfind(')');
  if (close == string::npos) {
    return 0;
  }

  std::istringstream rest(line.substr(close + 2));
  std::string value;
  while (rest >> value) {
    values.push_back(value);
  }

  if (values.size() <= 19) {
    return 0;
  }

  long starttime_ticks = 0;
  try {
    starttime_ticks = std::stol(values[19]);
  } catch (...) {
    return 0;
  }

  long starttime_seconds = starttime_ticks / HZ;
  long system_uptime = UpTime();
  long proc_uptime = system_uptime - starttime_seconds;
  return proc_uptime > 0 ? proc_uptime : 0;
}
