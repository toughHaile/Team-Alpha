#pragma once
#include <stdexcept>
#include <string>
#include "Logger.hpp"

class MiniGitError : public runtime_error {
public:
    MiniGitError(const string& msg) : runtime_error(msg) {
        Logger::error(msg);
    }
};
