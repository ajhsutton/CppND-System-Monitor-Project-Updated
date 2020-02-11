#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

#ifndef SYSTEM_PARSER_H
#include "linux_parser.h"
#endif

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {    
    long int total_time = LinuxParser::UpTime(pid_);
    long int uptime_delta = total_time - prevTotal;
    if (uptime_delta > 0){
        long int active_time =  LinuxParser::ActiveSeconds(pid_);
        float active_delta = active_time - prevActive;
        prevActive = active_time;
        prevTotal = total_time;
        utilization_ = active_delta / uptime_delta;
    }
    return utilization_;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_);}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long int Hz = sysconf(_SC_CLK_TCK);
    long int proc_uptime = LinuxParser::UpTime(pid_);
    proc_uptime = proc_uptime/Hz;
    return proc_uptime;
    }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& b) const { 
    return (((utilization_ > b.Utilization())&&(valid_ && b.Valid())) ||
            (!valid_ && b.Valid()) || 
            ((utilization_ > b.Utilization())&&(!valid_ && !b.Valid()))); 
    }

float Process::Utilization() const {return utilization_;};
void Process::Valid(bool valid){valid_ = valid;};
bool Process::Valid() const {return valid_;};