#include "condition.hpp"
#include "game_constants.hpp"
#include "helper.hpp"

#include <sstream>
#include <stdexcept>
#include <iostream>

bool IsValidComparison(const std::string &comp, const std::string &type)
{
    static const std::unordered_set<std::string> numericComparisons =
        {"==", "!=", "<", "<=", ">", ">="};
    static const std::unordered_set<std::string> booleanComparisons =
        {"==", "!="};

    if (type == "int")
    {
        return numericComparisons.find(comp) != numericComparisons.end();
    }
    else if (type == "bool")
    {
        return booleanComparisons.find(comp) != booleanComparisons.end();
    }
    return false;
}

ItemCondition::ItemCondition(const std::string &itemId,
                             int quantity,
                             const std::string &comp)
    : itemId(itemId), quantity(quantity), comparison(comp)
{
    if (itemId.empty())
    {
        throw std::invalid_argument("[ERROR][ItemCondition] itemId cannot be empty");
    }

    if (quantity < 0)
    {
        throw std::invalid_argument("[ERROR][ItemCondition] Quantity cannot be negative");
    }

    if (!IsValidComparison(comp, "int"))
    {
        std::stringstream ss;
        ss << "[ERROR][ItemCondition] Invalid comparison operator '" << comp
           << "' for numeric comparison";
        throw std::invalid_argument(ss.str());
    }
}

bool ItemCondition::Evaluate(const GameState &gameState) const
{
    if (!gameState.GetItemLoader().ItemExists(itemId))
    {
        std::cerr << "[ERROR][ItemCondition] Evaluating non-existent item "
                  << itemId << std::endl;
        throw;
    }

    try
    {
        int current = gameState.currentCharacter().GetItemCount(itemId);
        return Compare<int>(current, quantity, comparison);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR][ItemCondition] Evaluation failed for item "
                  << itemId << ": " << e.what() << std::endl;
        throw; 
    }
}

std::unique_ptr<Condition> ItemCondition::Clone() const
{
    return std::make_unique<ItemCondition>(*this);
}

FlagCondition::FlagCondition(const std::string &flagId, const bool value, std::string comp)
    : flagId(flagId), value(value), comparison(comp)
{
    if (flagId.empty())
    {
        throw std::invalid_argument("[ERROR][FlagCondition] flagId cannot be empty");
    }

    if (!IsValidComparison(comparison, "bool"))
    {
        std::stringstream ss;
        ss << "[ERROR][FlagCondition] Invalid comparison operator '" << comparison
           << "' for boolean comparison. Valid: ==, !=";
        throw std::invalid_argument(ss.str());
    }
}

bool FlagCondition::Evaluate(const GameState &gameState) const
{
    try
    {
        bool current = gameState.GetFlag(flagId);

        // For boolean comparisons, we should only accept == and !=
        {
            using namespace GameConsts::condition::comp;
            if (comparison != EQUAL && comparison != NOT_EQUAL)
            {
                std::cerr << "[WARNING][FlagCondition] Using invalid comparison '"
                          << comparison << "' for boolean value. Defaulting to " << EQUAL << std::endl;
                return current == value;
            }
        }

        return Compare<bool>(current, value, comparison);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR][FlagCondition] Evaluation failed for flag "
                  << flagId << ": " << e.what() << std::endl;
        throw;
    }
}

std::unique_ptr<Condition> FlagCondition::Clone() const
{
    return std::make_unique<FlagCondition>(*this);
}

VariableCondition::VariableCondition(const std::string &varId,
                                     const int value,
                                     const std::string &comp)
    : varId(varId), value(value), comparison(comp)
{
    if (varId.empty())
    {
        throw std::invalid_argument("[ERROR][VariableCondition] varId cannot be empty");
    }

    if (!IsValidComparison(comp, "int"))
    {
        std::stringstream ss;
        ss << "[ERROR][VariableCondition] Invalid comparison operator '" << comp
           << "' for numeric comparison";
        throw std::invalid_argument(ss.str());
    }
}

bool VariableCondition::Evaluate(const GameState &gameState) const
{
    try
    {
        int current = gameState.GetVariable(varId);
        return Compare<int>(current, value, comparison);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR][VariableCondition] Evaluation failed for variable "
                  << varId << ": " << e.what() << std::endl;
        throw;
    }
}

std::unique_ptr<Condition> VariableCondition::Clone() const
{
    return std::make_unique<VariableCondition>(*this);
}

EffectCondition::EffectCondition(const std::string &effectId, const int stacks, const std::string &comp)
    : effectId(effectId), stacks(stacks), comparison(comp)
{
    if (effectId.empty())
    {
        throw std::invalid_argument("[ERROR][EffectCondition] effectId cannot be empty");
    }

    if (stacks < 0)
    {
        throw std::invalid_argument("[ERROR][EffectCondition] Stacks cannot be negative");
    }

    if (!IsValidComparison(comp, "int"))
    {
        std::stringstream ss;
        ss << "[ERROR][EffectCondition] Invalid comparison operator '" << comp
           << "' for numeric comparison";
        throw std::invalid_argument(ss.str());
    }
}

bool EffectCondition::Evaluate(const GameState &gameState) const
{
    if (!gameState.GetEffectLoader().EffectExists(effectId))
    {
        std::cerr << "[ERROR][EffectCondition] Evaluating non-existent effect: "
                  << effectId << std::endl;
        throw;
    }

    try
    {
        int current = gameState.currentCharacter().GetEffectCount(effectId);
        return Compare<int>(current, stacks, comparison);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR][EffectCondition] Evaluation failed for effect "
                  << effectId << ": " << e.what() << std::endl;
        throw; 
    }
}

std::unique_ptr<Condition> EffectCondition::Clone() const
{
    return std::make_unique<EffectCondition>(*this);
}
