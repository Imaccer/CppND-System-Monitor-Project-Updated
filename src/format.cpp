#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"
#include "linux_parser.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  // Create a duration object representing the total number of seconds
  std::chrono::seconds duration(seconds);

  // Extract hours, minutes, and remaining seconds using std::chrono
  auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
  auto remainingMinutes = std::chrono::duration_cast<std::chrono::minutes>(
      duration % std::chrono::hours(1));
  auto remainingSeconds = std::chrono::duration_cast<std::chrono::seconds>(
      duration % std::chrono::minutes(1));

  // Format the result in HH:MM:SS format
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << hours.count() << ":"
     << std::setw(2) << remainingMinutes.count() << ":" << std::setw(2)
     << remainingSeconds.count();

  return ss.str();
}
