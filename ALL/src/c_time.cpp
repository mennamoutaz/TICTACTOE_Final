#include <vector>
#include <limits>
#include <string>
#include <iostream>
#include "sqlite3.h"
#include <cstdint>
#include <chrono>
#include <ctime>
#include <thread>
#include <map>
#include "c_time.h"

using namespace std;

std::string timeToString(std::chrono::system_clock::time_point timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    char buffer[TIME_BUFFER_SIZE];
    std::tm* timeInfo = std::localtime(&time);

    if (timeInfo == nullptr) {
        return "Error converting time.";
    }

    std::strftime(buffer, TIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", timeInfo);
    return std::string(buffer);
}
