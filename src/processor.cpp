#include <string>

#include "linux_parser.h"
#include "processor.h"

using LinuxParser::CPUStates;
using std::stol;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> cpuValues = LinuxParser::CpuUtilization();
  long int user = stol(cpuValues[CPUStates::kUser_]);
  long int nice = stol(cpuValues[CPUStates::kNice_]);
  long int system = stol(cpuValues[CPUStates::kSystem_]);
  long int idle = stol(cpuValues[CPUStates::kIdle_]);
  long int iowait = stol(cpuValues[CPUStates::kIOwait_]);
  long int irq = stol(cpuValues[CPUStates::kIRQ_]);
  long int softirq = stol(cpuValues[CPUStates::kSoftIRQ_]);
  long int steal = stol(cpuValues[CPUStates::kSteal_]);

  long int PrevIdle = previdle_ + previowait_;
  long int Idle = idle + iowait;

  long int PrevNonIdle = prevuser_ + prevnice_ + prevsystem_ + previrq_ +
                         prevsoftirq_ + prevsteal_;
  long int NonIdle = user + nice + system + irq + softirq + steal;

  long int PrevTotal = PrevIdle + PrevNonIdle;
  long int Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  long int totald = Total - PrevTotal;
  long int idled = Idle - PrevIdle;

  float CPU_Percentage = (totald - idled) / static_cast<float>(totald);

  // Set all prev values to current values
  previdle_ = idle;
  previowait_ = iowait;
  prevuser_ = user;
  prevnice_ = nice;
  prevsystem_ = system;
  previrq_ = irq;
  prevsoftirq_ = softirq;
  prevsteal_ = steal;

  return CPU_Percentage;
}
