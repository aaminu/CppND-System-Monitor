
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    
    std::ostringstream oss;
    std::time_t uptimeSeconds{seconds};
    std::tm *tm = std::localtime(&uptimeSeconds);

    oss << std::put_time(tm, "%T"); // equivalent to "%H:%M:%S" (the ISO 8601 time format) 
    return oss.str();


    // string timeFormatted = ""; 
    // uint8_t _hours = seconds/3600l; 
    // uint8_t _minutes = (seconds - _hours*3600)/60;
    // uint8_t _seconds = seconds - (_hours*3600) - (_minutes*60);

    // timeFormatted += (_hours < 10)? "0"+ std::to_string(_hours) : std::to_string(_hours);
    // timeFormatted += ":";
    // timeFormatted += (_minutes < 10)? "0"+ std::to_string(_minutes) : std::to_string(_minutes);
    // timeFormatted += ":";
    // timeFormatted += (_seconds < 10)? "0"+ std::to_string(_seconds) : std::to_string(_seconds);
    // return timeFormatted; 
    
    }