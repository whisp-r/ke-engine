#include "condition_factory.hpp"
#include "game_constants.hpp"

#include <iostream>

ConditionPtr ConditionFactory::CreateFromJSON(const nlohmann::json &data)
{
    using namespace GameConsts;

    if (!data.contains(condition::TYPE))
        return nullptr;

    std::string type = data[condition::TYPE];

    try
    {

        if (type == condition::HAS_ITEM)
        {
            return std::make_unique<ItemCondition>(
                data[item::ITEM],
                data.value(item::AMOUNT, item::AMOUNT_DEFAULT),
                data.value(condition::COMP, condition::comp::GREATER_EQUAL));
        }
        else if (type == condition::FLAG_SET)
        {
            return std::make_unique<FlagCondition>(
                data[flag::FLAG],
                data.value(flag::VALUE, flag::VALUE_DEFAULT),
                data.value(condition::COMP, condition::comp::EQUAL));
        }
        else if (type == condition::VAR_COMP)
        {
            return std::make_unique<VariableCondition>(
                data[var::VARIABLE],
                data.value(var::AMOUNT, var::AMOUNT_DEFAULT),
                data.value(condition::COMP, condition::comp::GREATER_EQUAL));
        }
        else if (type == condition::HAS_EFFECT)
        {
            return std::make_unique<EffectCondition>(
                data[effect::EFFECT],
                data.value(effect::AMOUNT, effect::AMOUNT_DEFAULT),
                data.value(condition::COMP, condition::comp::GREATER_EQUAL)
            );
        }
        // Add more types...
    }
    catch (const nlohmann::json::exception &e)
    {
        std::cerr << "[ERROR][ConditionFactory] JSON parsing error in condition: " << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR][ConditionFactory] Creating condition: " << e.what() << std::endl;
    }

    return nullptr; // Unknown condition type
}