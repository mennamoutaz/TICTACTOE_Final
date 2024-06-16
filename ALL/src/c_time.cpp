#include <vector>
#include <limits>
#include <string>
#include <iostream>
#include <sqlite3.h>
#include <cstdint>
#include <chrono>
#include <ctime>
#include <thread>
#include <map>
#include "c_time.h"
#include <iomanip>
#include <sstream>

using namespace std;

//std::string timeToString(std::chrono::system_clock::time_point timePoint) {
//   std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
//    char buffer[TIME_BUFFER_SIZE];
//    std::tm* timeInfo = std::localtime(&time);
//
//    if (timeInfo == nullptr) {
//        return "Error converting time.";
//    }
//    std::strftime(buffer, TIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", timeInfo);
//    return std::string(buffer);
//}

std::string timeToString(std::chrono::system_clock::time_point timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm tm;

#ifdef _WIN32
    localtime_s(&tm, &time);  // Use localtime_s on Windows
#else
    localtime_r(&time, &tm);  // Use localtime_r on Linux
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");  // Format the time using put_time
    return oss.str();  // Return the formatted time string
}
