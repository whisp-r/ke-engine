#include "game_state.hpp"
#include "json.hpp"
#include "game_constants.hpp"

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

GameState::GameState(FlagRegistry &flagRegistry, ItemLoader &itemLoader, VariableRegistry &varRegistry, EffectLoader &effectLoader)
    : flagRegistry(flagRegistry), itemLoader(itemLoader), varRegistry(varRegistry), effectLoader(effectLoader)
{
}

const ItemLoader &GameState::GetItemLoader() const
{
    return itemLoader;
}

const EffectLoader &GameState::GetEffectLoader() const
{
    return effectLoader;
}

bool GameState::GetFlag(const std::string &flagId) const
{
    if (flagId.empty())
    {
        std::cerr << "[ERROR][GameState] Empty flag ID requested" << std::endl;
        throw;
    }
    if (!flagRegistry.IsRegistered(flagId))
    {
        std::cerr << "[ERROR][GameState] Requested unregistered flag: " << flagId << std::endl;
        throw;
    }

    auto it = currentFlags.find(flagId);
    if (it != currentFlags.end())
    {
        return it->second;
    }

    // Return default if not set yet
    return flagRegistry.GetDefaultValue(flagId);
}

const std::unordered_map<std::string, bool> &GameState::GetCurrentFlags() const
{
    return currentFlags;
}

const std::unordered_map<std::string, int> &GameState::GetCurrentVariables() const
{
    return currentVariables;
}

void GameState::SetFlag(const std::string &flagId, const bool value)
{
    if (flagId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to set flag with empty ID" << std::endl;
        throw;
    }
    if (!flagRegistry.IsRegistered(flagId))
    {
        std::cerr << "[ERROR][GameState] Attempted to set unknown flag: " << flagId << std::endl;
        throw;
    }

    std::cout << "[GameState] Set flag " << flagId << " to " << value
              << std::endl;
    currentFlags[flagId] = value;
}

CharacterData &GameState::currentCharacter()
{
    return player;
}

const CharacterData &GameState::currentCharacter() const
{
    return player;
}

int GameState::GetVariable(const std::string &varId) const
{
    if (varId.empty())
    {
        std::cerr << "ERROR: Empty variable ID requested" << std::endl;
        throw;
    }
    if (!varRegistry.IsRegistered(varId))
    {
        std::cerr << "ERROR: Requested unregistered variable: " << varId << std::endl;
        throw;
    }

    auto it = currentVariables.find(varId);
    if (it != currentVariables.end())
    {
        return it->second;
    }

    // Return default if not set yet
    return varRegistry.GetDefaultValue(varId);
}

void GameState::SetVariable(const std::string &varId, const int value)
{
    if (varId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to set variable with empty ID" << std::endl;
        throw;
    }

    if (!varRegistry.IsRegistered(varId))
    {
        std::cerr << "[ERROR][GameState] Setting unregistered variable: " << varId << std::endl;
        throw;
    }

    currentVariables[varId] = value;
    std::cout << "[GameState] Set variable " << varId << " to " << value << std::endl;
}

void GameState::ModifyVariable(const std::string &varId, const int delta)
{
    if (varId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to modify variable with empty ID" << std::endl;
        throw;
    }

    int current = GetVariable(varId);

    // Safe overflow check (simplified version)
    if ((delta > 0 && current > INT_MAX - delta) ||
        (delta < 0 && current < INT_MIN - delta))
    {
        std::cerr << "[ERROR][GameState] Arithmetic overflow/underflow modifying variable: "
                  << varId << " (current: " << current << ", delta: " << delta << ")" << std::endl;
        throw;
    }

    SetVariable(varId, current + delta);
    std::cout << "[GameState] Modified " << varId << " by " << delta
              << " (total: " << currentVariables[varId] << ")" << std::endl;
}

void GameState::ModifyItem(const std::string &itemId, int delta)
{
    if (itemId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to add item with empty ID" << std::endl;
        return;
    }
    if (delta == 0)
    {
        std::cerr << "[WARNING][GameState] ModifyItem called with delta of 0 for item: " << itemId << std::endl;
        return;
    }
    if (!itemLoader.ItemExists(itemId))
    {
        std::cerr << "[ERROR][GameState] Attempted to add unknown item: " << itemId << std::endl;
        return;
    }

    player.ModifyItem(itemId, delta);
}

void GameState::ModifyEffect(const std::string &effectId, int delta)
{
    if (effectId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to add effect with empty ID" << std::endl;
        return;
    }
    if (delta == 0)
    {
        std::cerr << "[WARNING][GameState] ModifyEffect called with delta of 0 for item: " << effectId << std::endl;
        return;
    }
    if (!effectLoader.EffectExists(effectId))
    {
        std::cerr << "[ERROR][GameState] Attempted to add unknown effect: " << effectId << std::endl;
        return;
    }

    player.ModifyEffect(effectId, delta);
}

void CharacterData::ModifyItem(const std::string &itemId, const int delta)
{
    if (itemId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to modify item with empty ID" << std::endl;
        return;
    }
    if (delta == 0)
    {

        std::cerr << "[WARNING][GameState] ModifyItem called with delta of 0 for item: " << itemId << std::endl;
        return;
    }

    // Initialize if not present
    if (inventory.find(itemId) == inventory.end())
    {
        if (delta < 0)
        {
            std::cerr << "[ERROR][GameState] Cannot remove " << -delta
                      << " of item " << itemId << " (not in inventory)" << std::endl;
            return;
        }
        inventory[itemId] = 0;
    }
    // Check for overflow
    if (delta > 0 && inventory[itemId] > INT_MAX - delta)
    {
        std::cerr << "[ERROR][GameState] Arithmetic overflow adding item: " << itemId << std::endl;
        return;
    }

    // Check for negative inventory (after removal)
    int newQuantity = inventory[itemId] + delta;
    if (newQuantity < 0)
    {
        inventory.erase(itemId);
        std::cout << "[GameState] Removed item " << itemId << std::endl;
    }
    else
    {
        inventory[itemId] += delta;
        std::cout << "[GameState] Added " << delta << " " << itemId
                  << " (total: " << inventory[itemId] << ")" << std::endl;
    }
}

bool CharacterData::HasItem(const std::string &itemId, const int minQuantity) const
{
    if (itemId.empty())
    {
        std::cerr << "[ERROR][GameState] HasItem called with empty item ID" << std::endl;
        return false;
    }
    if (minQuantity < 0)
    {
        std::cerr << "[ERROR][GameState] HasItem called with negative minQuantity: " << minQuantity << std::endl;
        return false;
    }

    auto it = inventory.find(itemId);
    return it != inventory.end() && it->second >= minQuantity;
}

int CharacterData::GetItemCount(const std::string &itemId) const
{
    if (itemId.empty())
    {
        std::cerr << "[ERROR][GameState] GetItemCount called with empty item ID" << std::endl;
        return 0;
    }

    auto it = inventory.find(itemId);
    return it != inventory.end() ? it->second : 0;
}

const std::unordered_map<std::string, int> &CharacterData::GetInventory() const
{
    return inventory;
}

void CharacterData::ClearInventory()
{
    inventory.clear();
}

void CharacterData::ModifyEffect(const std::string &effectId, const int delta)
{
    if (effectId.empty())
    {
        std::cerr << "[ERROR][GameState] Attempted to modify effect with empty ID" << std::endl;
        return;
    }
    if (delta == 0)
    {

        std::cerr << "[WARNING][GameState] ModifyEffect called with delta of 0 for item: " << effectId << std::endl;
        return;
    }

    // Initialize if not present
    if (effects.find(effectId) == effects.end())
    {
        if (delta < 0)
        {
            std::cerr << "[ERROR][GameState] Cannot remove " << -delta
                      << " of effect " << effectId << " (not in effects)" << std::endl;
            return;
        }
        effects[effectId] = 0;
    }
    // Check for overflow
    if (delta > 0 && effects[effectId] > INT_MAX - delta)
    {
        std::cerr << "[ERROR][GameState] Arithmetic overflow adding effect: " << effectId << std::endl;
        return;
    }

    // Check for negative inventory (after removal)
    int newQuantity = effects[effectId] + delta;
    if (newQuantity < 0)
    {
        effects.erase(effectId);
        std::cout << "[GameState] Removed effect " << effectId << std::endl;
    }
    else
    {
        effects[effectId] += delta;
        std::cout << "[GameState] Inflicted " << delta << " " << effectId
                  << " (total: " << effects[effectId] << ")" << std::endl;
    }
}

bool CharacterData::HasEffect(const std::string &effectId, const int minQuantity) const
{
    if (effectId.empty())
    {
        std::cerr << "[ERROR][GameState] HasEffect called with empty effect ID" << std::endl;
        return false;
    }
    if (minQuantity < 0)
    {
        std::cerr << "[ERROR][GameState] HasEffect called with negative minQuantity: " << minQuantity << std::endl;
        return false;
    }

    auto it = effects.find(effectId);
    return it != effects.end() && it->second >= minQuantity;
}

int CharacterData::GetEffectCount(const std::string &effectId) const
{
    if (effectId.empty())
    {
        std::cerr << "[ERROR][GameState] GetItemCount called with empty item ID" << std::endl;
        return 0;
    }

    auto it = effects.find(effectId);
    return it != effects.end() ? it->second : 0;
}

const std::unordered_map<std::string, int> &CharacterData::GetEffects() const
{
    return effects;
}

void CharacterData::ClearEffects()
{
    effects.clear();
}