#pragma once

#include <exception>


class WorkersAtMaxCapacityException : public std::runtime_error {
    public:
        WorkersAtMaxCapacityException() : std::runtime_error("All workers at max capacity"){

        }
};