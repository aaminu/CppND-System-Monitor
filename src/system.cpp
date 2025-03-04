#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    
    vector<string> cpuValues = LinuxParser::CpuUtilization(); 
    float total = (float) LinuxParser::Jiffies();
    float idle = (float) LinuxParser::IdleJiffies();
    cpu_.setValues(total, idle);

    return cpu_;
    
    }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    vector<int> v_pids = LinuxParser::Pids();
    processes_.clear();
    for(auto elem : v_pids){
        processes_.emplace_back(elem);  
    }

    auto sorter = [](const Process &a, const Process &b){ return b < a;};
    std::sort(processes_.begin(), processes_.end(), sorter);
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    if (kernel_.empty())
        kernel_ = LinuxParser::Kernel();
    return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    if (os_.empty())
        os_ = LinuxParser::OperatingSystem();
    return os_;
} 

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
