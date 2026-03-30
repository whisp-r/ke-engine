#ifndef FLAG_REGISTRY_H
#define FLAG_REGISTRY_H
#pragma once

#include <unordered_map>
#include <string>
#include <unordered_set>

class FlagRegistry
{
public:
    void LoadFromFile(const std::string &filename);

    bool IsRegistered(const std::string &flagId) const;
    bool GetDefaultValue(const std::string &flagId) const;

private:
    std::unordered_set<std::string> registeredFlags;
    std::unordered_map<std::string, bool> defaultValues;
};

#endif