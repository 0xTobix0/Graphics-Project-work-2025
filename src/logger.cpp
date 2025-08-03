#include "logger.h"

// Define the global log file stream
std::ofstream logFile;

void initLogging() {
    // Open the log file in append mode
    logFile.open("graphics_debug.log", std::ios::out | std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
    } else {
        logFile << "\n=== New Session ===\n";
    }
}
