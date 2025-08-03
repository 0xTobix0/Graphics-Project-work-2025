#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

// Global file stream for logging
extern std::ofstream logFile;

// Function to initialize logging
void initLogging();

// Function to log to both console and file
template<typename T>
void logMessage(const T& message) {
    std::cout << message << std::endl;
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}

#endif // LOGGER_H
