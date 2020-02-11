#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include <algorithm>  

#ifndef SYSTEM_PARSER_H
#include "linux_parser.h"
#endif

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_;}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    for (auto proc : processes_){
        proc.Valid(false);
    }
    //vector<Process> new_processes;

    // Get active PIDS
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids){
        auto it = std::find_if(processes_.begin(), processes_.end(),
             [pid](const Process p){return p.Pid() == pid;});
        if (it != processes_.end()){
            it->Valid(true);
            it->CpuUtilization();
        } else{
            Process new_process = Process(pid);
            new_process.Valid(true);
            new_process.CpuUtilization();
            processes_.push_back(new_process);
        }       
    }

    // Sort by Valid, and remove Invalid/expired processes
    std::sort(processes_.begin(), processes_.end());
    
    while(!(processes_.back().Valid())){
        Process p = processes_.back();
        if (p.Valid() == false){ processes_.pop_back();}
    }
    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return  LinuxParser::UpTime();}