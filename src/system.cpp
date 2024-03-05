#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
    : operatingSystem_(LinuxParser::OperatingSystem()),
      kernel_(LinuxParser::Kernel()) {}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  processes_.clear();

  for (auto pid : pids) {
    //    Process process(pid);
    // processes_.emplace_back(process);
    processes_.emplace_back(
        pid);  //  note, this is equivalent to above commented 2 lines. Process
               //  constructor gets called by emplace_back when acting on
               //  processes_ (which is declared as vector of Process of
               //  objects.
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operatingSystem_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
