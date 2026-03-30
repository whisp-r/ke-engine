#include "config_loader.hpp"

#include "json.hpp"
#include "game_constants.hpp"
#include "helper.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

void ConfigLoader::LoadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("[ERROR][ConfigLoader] Cannot open config file: " + filename);
    }
    
    nlohmann::json data;
    try {
        data = nlohmann::json::parse(file);
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("[ERROR][ConfigLoader] Invalid JSON in config file: " + std::string(e.what()));
    }
    
    // load text styles
    using namespace GameConsts;
    
    // Check if config section exists
    if (!data.contains(config::CONFIG)) {
        throw std::runtime_error("[ERROR][ConfigLoader] Missing 'config' section in " + filename);
    }
    
    auto& config = data[config::CONFIG];
    
    // Check if text_style section exists
    if (!config.contains(config::TEXT_STYLE)) {
        throw std::runtime_error("[ERROR][ConfigLoader] Missing 'text_style' section in " + filename);
    }
    
    auto& textStyles = config[config::TEXT_STYLE];
    
    // Check if default style exists
    if (!textStyles.contains(config::DEFAULT)) {
        throw std::runtime_error("[ERROR][ConfigLoader] Missing 'default' text style in " + filename);
    }
    
    // Load default style
    defaultTextStyle.LoadFromJSON(textStyles[config::DEFAULT], TextStyle());
    
    // Load speaker styles (optional)
    if (textStyles.contains(config::SPEAKERS)) {
        auto& speakersData = textStyles[config::SPEAKERS];
        
        for (auto &[speakerId, speakerData] : speakersData.items())
        {
            // Skip empty speaker IDs
            if (speakerId.empty()) {
                std::cerr << "[WARNING][ConfigLoader] Empty speaker ID in config, skipping" << std::endl;
                continue;
            }
            
            try {
                TextStyle speakerStyle;
                speakerStyle.LoadFromJSON(speakerData, defaultTextStyle);
                speakers[speakerId] = speakerStyle;
            } catch (const std::exception& e) {
                std::cerr << "[WARNING][ConfigLoader] Failed to load style for speaker '" 
                          << speakerId << "': " << e.what() << std::endl;
                // Continue loading other speakers
            }
        }
    }
}

const TextStyle &ConfigLoader::GetDefaultTextStyle() const
{
    return defaultTextStyle;
}

const TextStyle &ConfigLoader::GetSpeakerStyle(const std::string &speaker) const
{
    auto it = speakers.find(speaker);
    if (it != speakers.end())
    {
        return it->second;
    }
    return GetDefaultTextStyle();
}

void TextStyle::LoadFromJSON(const json &textData, const TextStyle &fallback)
{
    color = fallback.color;
    fontSize = fallback.fontSize;
    speed = fallback.speed;

    // override defaults
    if (textData.contains(GameConsts::node::COLOR))
    {
        color = GetColorFromJSON(textData[GameConsts::node::COLOR]);
    }
    if (textData.contains(GameConsts::node::FONT_SIZE))
    {
        fontSize = textData[GameConsts::node::FONT_SIZE];
    }
    if (textData.contains(GameConsts::node::SPEED))
    {
        speed = textData[GameConsts::node::SPEED];
    }
}

const raylib::Color &TextStyle::GetColor() const
{
    return color;
}

float TextStyle::GetFontSize() const
{
    return fontSize;
}

unsigned short TextStyle::GetSpeed() const
{
    return speed;
}
