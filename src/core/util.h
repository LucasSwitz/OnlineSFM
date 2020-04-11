#pragma once
#include <chrono>
#include <string>
#include <iostream>

class PrecisionTimer{
    public:
        PrecisionTimer(const std::string& name);
        ~PrecisionTimer();

    private:
        std::chrono::_V2::high_resolution_clock::time_point start;
        std::string name;
};

std::string GetUUID();
void DeleteIfExists(const std::string& path);
void DeleteDirIfExists(const std::string& path);
void CleanAndMakeDir(const std::string& path);