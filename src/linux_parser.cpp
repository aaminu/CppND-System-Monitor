#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
namespace fs = std::filesystem;

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
  string os, kernel, version;
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
  fs::path directory{kProcDirectory};
  for(auto const &dir_entry : fs::directory_iterator{directory}){
    if (dir_entry.is_directory()){
      string dir_entry_str{dir_entry.path().string()};
      
      auto pos = dir_entry_str.rfind('/');
      if (pos != string::npos){
        ++ pos;
        dir_entry_str = dir_entry_str.substr(pos); 
      }
      if(std::all_of(dir_entry_str.begin(), dir_entry_str.end(), isdigit)){
        int pid = stoi(dir_entry_str);
        pids.push_back(pid);
      }
    }
  }
  // DIR* directory = opendir(kProcDirectory.c_str());
  // struct dirent* file;
  // while ((file = readdir(directory)) != nullptr) {
  //   // Is this a directory?
  //   if (file->d_type == DT_DIR) {
  //     // Is every character of the name a digit?
  //     string filename(file->d_name);
  //     if (std::all_of(filename.begin(), filename.end(), isdigit)) {
  //       int pid = stoi(filename);
  //       pids.push_back(pid);
  //     }
  //   }
  // }
  // closedir(directory);
  return pids;

}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string memType, memTotal{"MemTotal:"}, memFree{"MemFree:"};
  float memValue, memTotalValue{0.0}, memFreeValue{0.0};
  string line;
  int count = 0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> memType >>  memValue;
      if (memType == memTotal){
        memTotalValue = memValue;
        count++;
      }else if (memType == memFree)
      {
        memFreeValue = memValue;
        count++;
      }

      if (count == 2) break;
    }
  }

  return ((memTotalValue - memFreeValue)/memTotalValue);
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long activeUptime, idleUptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> activeUptime >> idleUptime;
  }

  return activeUptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return (ActiveJiffies()+IdleJiffies()); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  vector<string> jiffies = CpuUtilization(pid);
  long total = 0;
  for (const auto &i : {ProcessStates::kUTime_, ProcessStates::kSTime_, ProcessStates::kCuTime_, ProcessStates::kCsTime_} ){
    total += std::stol(jiffies[i]); 
  }
  return total;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jiffies = CpuUtilization();
  long total = 0;
  for (const auto &i : {CPUStates::kUser_, CPUStates::kNice_, CPUStates::kSystem_, 
                        CPUStates::kIRQ_, CPUStates::kSoftIRQ_, CPUStates::kSteal_})
  {
    total += std::stol(jiffies[i]); 
  }
  return total;
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  long total = 0;
  for (const auto &i : {CPUStates::kIdle_, CPUStates::kIOwait_}){
    total += std::stol(jiffies[i]); 
  }
  return total;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuValues;
  string line, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    while (linestream >> value)
    {
      cpuValues.push_back(value);
    }
    
  }
 return cpuValues;
}

vector<string> LinuxParser::CpuUtilization(int pid) { 
  vector<string> cpuValues;
  string line, value;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    uint8_t count = 0;
    while (count < 24)
    {
      linestream >> value;
      cpuValues.push_back(value);
      count++;
    }
    
  }
 return cpuValues;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string strParser, line; 
  int intParser, totalProcesses{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> strParser >>  intParser;
      if (strParser == "processes"){
        totalProcesses = intParser;
        break;
      }
    }
  }
  return totalProcesses; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string strParser, line; 
  int intParser, runningProcesses{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> strParser >>  intParser;
      if (strParser == "procs_running"){
        runningProcesses = intParser;
        break;
      }
    }
  }
  return runningProcesses; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
  }
  return line; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string strParser, value, line;
  string ramMB{""};
  
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> strParser >> value;
      if (strParser == "VmSize:"){
        ramMB = to_string(stol(value)/1000);
        break;
      }
    }
  }
  return ramMB; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  string strParser, value, line;
  string uid{""};
  
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> strParser >> value;
      if (strParser == "Uid:"){
        uid = value;
        break;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string subPattern = ":x:" + uid;
  std::regex regexp("[a-z0-9\\-]+" + subPattern);
  std::smatch m;
  string user{""};

  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::regex_search(line, m, regexp);
      if(!m.empty()){
        user = m.str().substr(0, m.str().size() - subPattern.size());
        break;
      }
    }
  }
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  vector<string> jiffies = CpuUtilization(pid);
  
  return std::stol(jiffies[ProcessStates::kUpTime_]); 
  }
