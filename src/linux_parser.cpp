#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::FindValueForKeyInFile(string key, string path){
  vector<string> results = FindValuesForKeyInFile(key, path);
  return results[0];
}

vector<string> LinuxParser::FindValuesForKeyInFile(string key, string path){
  string line;
  string key_value;
  vector<string> value;
  string line_key;
  std::ifstream filestream(path);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> line_key) {
        if (line_key == key) {
          while(linestream >> key_value){
            value.push_back(key_value);
          }
          return value;
        }
      }
    }
  }
  return value;
};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string value;
  string line_key;
  std::ifstream filestream(kOSPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      if (linestream >> line_key >> value) {
        if (line_key == "PRETTY_NAME") {
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string memTotal, memFree;//, memAvailable, buffers;
  memTotal = FindValueForKeyInFile("MemTotal",kProcDirectory+kMeminfoFilename);
  memFree = FindValueForKeyInFile("MemFree",kProcDirectory+kMeminfoFilename);
  // memAvailable = FindValueForKeyInFile("MemAvailable",kProcDirectory+kMeminfoFilename);
  // buffers = FindValueForKeyInFile("Buffers",kProcDirectory+kMeminfoFilename);

  return (std::stoi(memTotal) - std::stoi(memFree))/std::stof(memTotal);
}

// DONE: Read and return the system uptime
long int LinuxParser::UpTime() { 
  string line;
  string up_value, idle_value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_value >> idle_value;
  }
  return std::stol(up_value);
};

// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveSeconds(int pid) {
  string line = string();
  string line_token;
  vector<string> line_tokens = {};

  std::ifstream stream(kProcDirectory + std::to_string(pid) +
      "/" + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream >> line_token){
     line_tokens.push_back(line_token);
    }
  }
  // Convert entries
  long int utime = stol(line_tokens[14-1]);
  long int stime = stol(line_tokens[15-1]);
  long int cutime = stol(line_tokens[16-1]);
  long int cstime = stol(line_tokens[17-1]);
  
  // Calculation reference:
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  long int total_time = utime + stime + cutime + cstime;
  return total_time; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveSeconds() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return FindValuesForKeyInFile("cpu",kProcDirectory+kStatFilename);}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  // Retrieve Total Processes informatino from /proc/meminfo
  // Find field processes
  std::string totalProcesses = FindValueForKeyInFile("processes",
   kProcDirectory +  kStatFilename);
  return std::stoi(totalProcesses);
  }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string runningProcesses = FindValueForKeyInFile("procs_running",
   kProcDirectory +  kStatFilename);
  return std::stoi(runningProcesses);
  }

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line = string();
  std::ifstream stream(kProcDirectory + std::to_string(pid) +
      "/" + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return line;
}

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string ram_kb = FindValueForKeyInFile("VmSize",
    kProcDirectory + std::to_string(pid) +
      "/" + kStatusFilename);
  long int ram_mb = std::stol(ram_kb) / 1024;
  return std::to_string(ram_mb); 
  }

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid = FindValueForKeyInFile("Uid",
    LinuxParser::kProcDirectory + std::to_string(pid) +
      "/" + LinuxParser::kStatusFilename);
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);

  string name = string();
  string x, uid_str;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> name >> x >> uid_str;
    if (uid_str == uid){
      return name;
    }
  }
  return name;
}

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line = string();
  long int uptime = -1;

  std::ifstream stream(kProcDirectory + std::to_string(pid) +
      "/" + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_token;
    vector<string> line_tokens = {};

    while(linestream >> line_token){
      line_tokens.push_back(line_token);
    }
    // Convert entries
    long int starttime = std::stol(line_tokens[22 -1]);
    long int Hz = sysconf(_SC_CLK_TCK);

    // Process Uptime = System Uptime - Starttime
    long int cpu_uptime_s = UpTime(); // measured in seconds
    uptime = cpu_uptime_s*Hz - starttime;  
  };
  return uptime;
}