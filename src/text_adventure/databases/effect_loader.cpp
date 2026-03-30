#include "effect_loader.hpp"

#include "json.hpp"
#include "game_constants.hpp"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>

EffectDefinition::EffectDefinition(std::string id, std::string displayName, std::string description)
    : id(id), displayName(displayName), description(description) {}

const std::string &EffectDefinition::GetId() const
{
    return id;
}

const std::string &EffectDefinition::GetDisplayName() const
{
    return displayName;
}

const std::string &EffectDefinition::GetDescription() const
{
    return description;
}

void EffectLoader::LoadFromFile(const std::string &filename)
{
    std::cout << "EffectLoader: Loading from " << filename << std::endl;

    // Check file exists
    if (!std::filesystem::exists(filename))
    {
        std::string error = "File not found: " + filename;
        std::cerr << "[ERROR][EffectLoader] " << error << std::endl;
        throw std::runtime_error(error);
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::string error = "Cannot open file: " + filename;
        std::cerr << "[ERROR][EffectLoader] " << error << std::endl;
        throw std::runtime_error(error);
    }

    // Parse JSON
    nlohmann::json data;
    try
    {
        data = nlohmann::json::parse(file);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::string error = "JSON parse error in " + filename + ": " + e.what();
        std::cerr << "[ERROR][EffectLoader] " << error << std::endl;
        throw std::runtime_error(error);
    }

    // Check structure
    if (!data.contains(GameConsts::state::EFFECTS))
    {
        std::cerr << "[WARNING][EffectLoader] effects.json missing 'effects' key" << std::endl;
        return;
    }

    {
        using namespace GameConsts;
        // Load each item
        int count = 0;
        for (auto &[effectId, effectData] : data[state::EFFECTS].items())
        {
            EffectDefinition def(
                effectId,
                effectData.value(effect::DISPLAY_NAME, effectId),
                effectData.value(effect::DESCRIPTION, effect::DESCRIPTION_DEFAULT));

            effects.emplace(effectId, std::move(def));
            count++;

            std::cout << '\t'
                      << "[EffectLoader] Loaded: " << effectId << " -> "
                      << effects.at(effectId).GetDisplayName()
                      << " ["
                      << effects.at(effectId).GetDescription()
                      << "]" << std::endl;
        }

        std::cout << "[EffectLoader] Successfully loaded " << count << " effects" << std::endl;
    }
}

const EffectDefinition &EffectLoader::GetEffect(const std::string &effectId) const
{
    auto it = effects.find(effectId);
    if (it == effects.end())
    {
        throw std::runtime_error("[ERROR][EffectLoader] Unknown effect: " + effectId);
    }
    return it->second;
}

bool EffectLoader::EffectExists(const std::string &effectId) const
{
    return effects.find(effectId) != effects.end();
}
