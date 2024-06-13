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
#include <cstdint>
#include <conio.h>
#include "c_time.h"


using namespace std;



std::string timeToString(std::chrono::system_clock::time_point timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm timeInfo;
    localtime_s(&timeInfo, &time);

    char buffer[TIME_BUFFER_SIZE];
    std::strftime(buffer, TIME_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", &timeInfo);

    return std::string(buffer);
}