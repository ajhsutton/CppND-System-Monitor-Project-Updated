#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;                               // DONE: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  Process(int pid): pid_(pid){};
  void Valid(bool);
  bool Valid() const;
  float Utilization() const;

  // TODO: Declare any necessary private members
 private:
    int pid_; 
    bool valid_;
    long prevActive = 0;
    long prevTotal = 0;
    float utilization_ = 0.0;
};

#endif