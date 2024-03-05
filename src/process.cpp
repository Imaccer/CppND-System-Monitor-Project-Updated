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

int Process::Pid() { return pid_; }

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

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return username_; }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  return (CpuUtilization() > a.CpuUtilization());
}
