#include "DetermineTime.h"
#include <chrono>

long DetermineTime::getCurrentMillisecondsTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return duration.count();
}

long DetermineTime::getMillisecondsPassed(long initTime) {
    return getCurrentMillisecondsTime() - initTime;
}
