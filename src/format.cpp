#include <string>
#include <math.h>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int time_s) {
    int hours = std::floor(time_s/ 3600.0);
    std::string hours_str = std::to_string(hours);
    if (hours < 10){hours_str = "0" + hours_str;};

    int residual = time_s - (hours * 3600);
    int mins = std::floor(residual / 60.0);
    std::string min_str = std::to_string(mins);
    if (mins < 10){min_str = "0" + min_str;};

    int secs = residual - (mins*60);
    std::string sec_str = std::to_string(secs);
    if (secs < 10){sec_str = "0" + sec_str;};
    
    return  hours_str + ":" + min_str +":" + sec_str;
    }