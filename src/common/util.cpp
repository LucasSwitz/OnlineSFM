#include "util.h"

PrecisionTimer::PrecisionTimer(const std::string& name){
    this->start = std::chrono::high_resolution_clock::now();
    this->name = name;
}

PrecisionTimer::~PrecisionTimer(){
    auto end =  std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - this->start).count();
    std::cout << this->name << ": " << duration << std::endl;
}