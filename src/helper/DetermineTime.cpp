#include "DetermineTime.h"
#include <chrono>

// Obtiene el tiempo actual en milisegundos
long long DetermineTime::getCurrentMillisecondsTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return duration.count();
}

// Calcula el tiempo transcurrido en milisegundos desde initTime
long long DetermineTime::getMillisecondsPassed(long long initTime) {
    return getCurrentMillisecondsTime() - initTime;
}
