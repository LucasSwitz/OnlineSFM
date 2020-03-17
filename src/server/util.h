#pragma once
#include <string>

std::string GetUUID();

void DeleteIfExists(const std::string& path);
void DeleteDirIfExists(const std::string& path);
void CleanAndMakeDir(const std::string& path);
