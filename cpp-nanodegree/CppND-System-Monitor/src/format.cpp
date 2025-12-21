#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  seconds = seconds % 60;

  // convert to string
  std::ostringstream time_stream;
  time_stream << std::setfill('0') << std::setw(2) << hours << ":"
              << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
  return time_stream.str();
}