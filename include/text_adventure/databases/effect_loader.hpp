#ifndef EFFECT_LOADER_H
#define EFFECT_LOADER_H
#pragma once

#include <string>
#include <unordered_map>

class EffectDefinition
{
public:
    EffectDefinition(std::string id,
                     std::string displayName,
                     std::string description);
    const std::string &GetId() const;
    const std::string &GetDisplayName() const;
    const std::string &GetDescription() const;

private:
    std::string id;
    std::string displayName;
    std::string description;
};

class EffectLoader
{
public:
    void LoadFromFile(const std::string &filename);
    const EffectDefinition &GetEffect(const std::string &effectId) const;
    bool EffectExists(const std::string &effectId) const;

private:
    std::unordered_map<std::string, EffectDefinition> effects;
};

#endif