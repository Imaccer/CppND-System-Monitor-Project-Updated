#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T LinuxParser::getValueFromFile(const std::string& filePath,
                                const std::string& key) {
  T result = T();  // Default value for T
  std::ifstream filestream(filePath);
  if (filestream.is_open()) {
    std::string line;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::string currentKey;
      std::string value;
      while (linestream >> currentKey >> value) {
        if (currentKey == key) {
          std::istringstream iss(value);
          iss >> result;  // Convert value to the specified type
          return result;  // Found the key, return the result
        }
        break;
      }
    }
  }
  return result;  // Key not found or conversion failed
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal;
  float MemFree;
  float Cached;
  float SReclaimable;
  float Shmem;
  float Buffers;

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = std::stof(value);
        } else if (key == "MemFree") {
          MemFree = std::stof(value);
        } else if (key == "Cached") {
          Cached = std::stof(value);
        } else if (key == "SReclaimable") {
          SReclaimable = std::stof(value);
        } else if (key == "Shmem") {
          Shmem = stof(value);
        } else if (key == "Buffers") {
          Buffers = std::stof(value);
        }
      }
    }
  }
  float cachedMemory = Cached + SReclaimable - Shmem;
  float TotalUsedMemory = MemTotal - MemFree;
  float nonBufferNonCacheUsedMemory =
      TotalUsedMemory - (Buffers + cachedMemory);

  return (nonBufferNonCacheUsedMemory / MemTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptimeStr, idletimeStr, line;
  long int uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptimeStr >> idletimeStr;
  }
  uptime = static_cast<long int>(std::stof(uptimeStr));
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line, key;
  vector<string> cpuVariables;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          linestream >> user >> nice >> system >> idle >> iowait >> irq >>
              softirq >> steal >> guest >> guest_nice;
          cpuVariables = {user, nice,    system, idle,  iowait,
                          irq,  softirq, steal,  guest, guest_nice};
          break;
        }
      }
    }
  }
  return cpuVariables;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return getValueFromFile<int>(kProcDirectory + kStatFilename, "processes");
}
int LinuxParser::RunningProcesses() {
  return getValueFromFile<int>(kProcDirectory + kStatFilename, "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    cmd = line;
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Ram(int pid) {
  auto ram = getValueFromFile<int>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "VmSize:");
  int ramMB = ram * 0.001;
  return std::to_string(ramMB);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }
string LinuxParser::Uid(int pid) {
  int uid = getValueFromFile<int>(
      kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid:");
  return std::to_string(uid);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string username, x, uid;
  string line;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> uid) {
        if (uid == Uid(pid)) {
          //          linestream >> username;
          return username;
        }
      }
    }
  }

  return "";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::ifstream statFile(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (!statFile.is_open()) {
    std::cerr << "Error: Unable to open /proc/" << pid << "/stat" << std::endl;
    return -1;  // Return an error value
  }

  // Read the content of the stat file
  string line;
  std::getline(statFile, line);

  // Close the file
  statFile.close();

  // Extract the start time from the content
  std::istringstream linestream(line);
  string ignore;  // Variable to ignore unnecessary fields
  long int startTime;

  // Iterate through the fields until reaching the desired field (22nd field in
  // this case)
  for (int i = 1; i <= 21; ++i) {
    linestream >> ignore;
  }

  // Extract the start time
  linestream >> startTime;

  // Convert the start time from jiffies to seconds
  startTime /= sysconf(_SC_CLK_TCK);

  long int processUpTime = UpTime() - startTime;
  return processUpTime;
}

vector<string> LinuxParser::CpuUtilization(int pid) {
  string utime, stime, cutime, cstime, starttime;
  vector<string> cpuVariables;

  std::ifstream statFile(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (!statFile.is_open()) {
    std::cerr << "Error: Unable to open /proc/" << pid << "/stat" << std::endl;
  }

  // Read the content of the stat file
  string line;
  std::getline(statFile, line);

  // Close the file
  statFile.close();

  // Extract the start time from the content
  std::istringstream linestream(line);
  string ignore;  // Variable to ignore unnecessary fields

  for (int i = 1; i <= 22; ++i) {
    if (i == 14) {
      linestream >> utime;
      cpuVariables.emplace_back(utime);
    } else if (i == 15) {
      linestream >> stime;
      cpuVariables.emplace_back(stime);
    } else if (i == 16) {
      linestream >> cutime;
      cpuVariables.emplace_back(cutime);
    } else if (i == 17) {
      linestream >> cstime;
      cpuVariables.emplace_back(cstime);
    } else if (i == 22) {
      linestream >> starttime;
      cpuVariables.emplace_back(starttime);
    } else {
      linestream >> ignore;
    }
  }
  return cpuVariables;
}
