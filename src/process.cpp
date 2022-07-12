#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const{ return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    long seconds_ =  LinuxParser::UpTime() - UpTime();
    long totalTime = LinuxParser::ActiveJiffies(Pid()) / sysconf(_SC_CLK_TCK);
    utilization = (1.0 * totalTime) / seconds_;

    return utilization;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid())/sysconf(_SC_CLK_TCK); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return (this->utilization < a.utilization); 
}