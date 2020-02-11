#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> cpu_utilization =  LinuxParser::CpuUtilization();
  // Parse CPU Utilization data from: 
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long int usertime    = std::stol(cpu_utilization[0]);
  long int nicetime    = std::stol(cpu_utilization[1]);
  long int systemtime  = std::stol(cpu_utilization[2]);
  long int idletime    = std::stol(cpu_utilization[3]);
  long int ioWait      = std::stol(cpu_utilization[4]);
  long int irqtime     = std::stol(cpu_utilization[5]);
  long int softirqtime = std::stol(cpu_utilization[6]);
  long int stealtime   = std::stol(cpu_utilization[7]);
  long int guesttime   = std::stol(cpu_utilization[8]);
  long int guestnice   = std::stol(cpu_utilization[9]);

  // Utlization
  // Guest time is already accounted in usertime
  usertime = usertime - guesttime; // As you see here, it subtracts guest from user time
  nicetime = nicetime - guestnice; // and guest_nice from nice time
  auto systemalltime = systemtime + irqtime + softirqtime;
  auto virtalltime = guesttime + guestnice;
  
  auto idlealltime = idletime + ioWait; // ioWait is added in the idleTime
  auto nonidletime = usertime + nicetime + systemalltime + stealtime;
  auto totaltime = usertime + nicetime + systemalltime + idlealltime + stealtime + virtalltime;

  long idle_diff = idlealltime - prevIdle;
  auto prevTotal = prevIdle + prevNonIdle;

  // Update internal state  
  prevIdle = idlealltime; 
  prevNonIdle = nonidletime;

  // differentiate: actual value minus the previous one
  auto totald = totaltime - prevTotal;
  float cpu_pct = float(totald - idle_diff)/totald;
  return cpu_pct;
};
