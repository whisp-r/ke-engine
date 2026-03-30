#include "variable_registry.hpp"
#include "game_constants.hpp"
#include "json.hpp"

#include <fstream>
#include <iostream>

void VariableRegistry::LoadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("[ERROR][VariableReg] Cannot open variables file: " + filename);
    }

    nlohmann::json data;
    try
    {
        data = nlohmann::json::parse(file);
    }
    catch (const nlohmann::json::exception &e)
    {
        throw std::runtime_error("[ERROR][VariableReg] Invalid JSON in variables file: " + std::string(e.what()));
    }

    using namespace GameConsts;

    if (!data.contains(state::VARIABLES))
    {
        throw std::runtime_error("[ERROR][VariableReg] Missing 'variables' section in " + filename);
    }

    auto &varsData = data[state::VARIABLES];

    for (auto &[varId, value] : varsData.items())
    {
        if (varId.empty())
        {
            std::cerr << "[WARNING][VariableReg] Empty variable ID in file, skipping" << std::endl;
            continue;
        }

        if (!value.is_number_integer())
        {
            std::cerr << "[WARNING][VariableReg] Variable '" << varId << "' value is not integer, using 0" << std::endl;
        }

        int varValue = value.is_number_integer() ? value.get<int>() : 0;

        registeredVariables.insert(varId);
        defaultValues[varId] = varValue;

        std::cout << '\t'
                  << "[VariableReg] Registered variable: " << varId
                  << " (default: " << varValue << ")" << std::endl;
    }
}

bool VariableRegistry::IsRegistered(const std::string &varId) const
{
    return registeredVariables.find(varId) != registeredVariables.end();
}

int VariableRegistry::GetDefaultValue(const std::string &varId) const
{
    auto it = defaultValues.find(varId);
    return it != defaultValues.end() ? it->second : 0;
}
