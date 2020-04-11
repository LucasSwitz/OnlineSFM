#pragma once

#include <except.hpp>


class WorkersAtMaxCapacityException : public std::runtime_error {
    public:
        WorkersAtMaxCapacityException() : std::runtime_error("All workers at max capacity"){

        }
};