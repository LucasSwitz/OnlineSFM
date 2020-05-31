#pragma once

#include <string>
#include <exception>

class CameraIntrinsicNotFoundException : public std::runtime_error
{
public:
    CameraIntrinsicNotFoundException(const std::string &camera_id) : std::runtime_error("Camera intrinsic is unknown: " + camera_id)
    {
    }
};

class FailedToComputeFaeturesException : public std::runtime_error
{
public:
    FailedToComputeFaeturesException(const std::string &image) : std::runtime_error("Failed to compute features for image: " + image)
    {
    }
};