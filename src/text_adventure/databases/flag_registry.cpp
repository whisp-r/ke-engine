#include "flag_registry.hpp"
#include "game_constants.hpp"
#include "json.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

void FlagRegistry::LoadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("[ERROR][FileReg] Cannot open flags file: " + filename);
    }

    nlohmann::json data;
    try
    {
        data = nlohmann::json::parse(file);
    }
    catch (const nlohmann::json::exception &e)
    {
        throw std::runtime_error("[ERROR][FileReg] Invalid JSON in flags file: " + std::string(e.what()));
    }

    using namespace GameConsts;

    if (!data.contains(state::FLAGS))
    {
        throw std::runtime_error("[ERROR][FileReg] Missing 'flags' section in " + filename);
    }

    auto &flagsData = data[state::FLAGS];

    for (auto &[flagId, value] : flagsData.items())
    {
        if (flagId.empty())
        {
            std::cerr << "[WARNING][FileReg] Empty flag ID in file, skipping" << std::endl;
            continue;
        }

        if (!value.is_boolean())
        {
            std::cerr << "[WARNING][FileReg] Flag '" << flagId << "' value is not boolean, using false" << std::endl;
        }

        bool flagValue = value.is_boolean() ? value.get<bool>() : false;

        registeredFlags.insert(flagId);
        defaultValues[flagId] = flagValue;

        std::cout << '\t'
                  << "[FileReg] Registered flag: " << flagId
                  << " (default: " << (flagValue ? "true" : "false") << ")" << std::endl;
    }
}

bool FlagRegistry::IsRegistered(const std::string &flagId) const
{
    return registeredFlags.find(flagId) != registeredFlags.end();
}

bool FlagRegistry::GetDefaultValue(const std::string &flagId) const
{
    auto it = defaultValues.find(flagId);
    return it != defaultValues.end() ? it->second : false;
}
