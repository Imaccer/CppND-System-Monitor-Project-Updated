#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using LinuxParser::processCPUStates;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), username_(LinuxParser::User(pid)){};

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  long int uptime = LinuxParser::UpTime();

  std::vector<std::string> cpuValues = LinuxParser::CpuUtilization(pid_);

  long int utime = stol(cpuValues[processCPUStates::kUtime_]);
  long int stime = stol(cpuValues[processCPUStates::kStime_]);
  long int cutime = stol(cpuValues[processCPUStates::kCutime_]);
  long int cstime = stol(cpuValues[processCPUStates::kCstime_]);
  long int starttime = stol(cpuValues[processCPUStates::kStarttime_]);

  long int total_time = utime + stime + cutime + cstime;
  long int seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
  float CPU_usage =
      ((static_cast<float>(total_time) / sysconf(_SC_CLK_TCK)) / seconds);

  return CPU_usage;
}

// TODO: Return the command that generated this process
// string Process::Command() { return string(); }
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return username_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a[[maybe_unused]]) const { return
// true; }
bool Process::operator<(Process const& a) const {
  return (CpuUtilization() > a.CpuUtilization());
}
