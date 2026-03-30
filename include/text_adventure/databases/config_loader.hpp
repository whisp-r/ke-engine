#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H
#pragma once

#include <raylib-cpp.hpp>
#include <unordered_map>
#include <string>

#include "json.hpp"
using json = nlohmann::json;

class TextStyle
{
public:
    void LoadFromJSON(const json &textData, const TextStyle &fallback);

    const raylib::Color &GetColor() const;
    float GetFontSize() const;
    unsigned short GetSpeed() const;

private:
    raylib::Color color;
    float fontSize;
    unsigned short speed;
};

class ConfigLoader
{
public:
    void LoadFromFile(const std::string &filename);

    const TextStyle &GetDefaultTextStyle() const;
    const TextStyle &GetSpeakerStyle(const std::string &speaker) const;

private:
    TextStyle defaultTextStyle;
    std::unordered_map<std::string, TextStyle> speakers;
};

#endif