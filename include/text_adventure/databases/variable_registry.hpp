#ifndef VARIABLE_REGISTRY_H
#define VARIABLE_REGISTRY_H
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>

class VariableRegistry
{
public:
    void LoadFromFile(const std::string &filename);
    bool IsRegistered(const std::string &varId) const;
    int GetDefaultValue(const std::string &varId) const;

private:
    std::unordered_set<std::string> registeredVariables;
    std::unordered_map<std::string, int> defaultValues;
};

#endif