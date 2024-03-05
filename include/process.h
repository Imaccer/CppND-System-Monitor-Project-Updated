#ifndef PROCESS_H
#define PROCESS_H

#include <string>

//  Basic class for Process representation
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  std::string username_;
};

#endif
