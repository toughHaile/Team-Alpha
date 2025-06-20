#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

// Handles logging of system activities and errors
class Logger {
private:
    static const string logFilePath;

    // Gets current timestamp for logging
    static string currentDateTime() {
        auto now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        tm localTime = *localtime(&time);
        
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
        return string(buffer);
    }

public:
    // Logs action with optional user context
    static void log(const string& action, const string& user = "system") {
        ofstream logFile(logFilePath, ios::app);
        if (!logFile) return;

        logFile << "[" << currentDateTime() << "] "
                << "User:" << user << " - " 
                << action << "\n";
    }

    // Logs error messages
    static void error(const string& message) {
        log("ERROR: " + message);
    }
};

const string Logger::logFilePath = ".minigit/logs/activity.log"; 
