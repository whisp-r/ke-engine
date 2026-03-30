#ifndef GAME_STATE_H
#define GAME_STATE_H
#pragma once

#include "item_loader.hpp"
#include "flag_registry.hpp"
#include "variable_registry.hpp"
#include "effect_loader.hpp"

#include <unordered_map>
#include <string>

class CharacterData
{
public:
    void ModifyItem(const std::string &itemId, const int delta);
    bool HasItem(const std::string &itemId, const int minQuantity = 1) const;
    int GetItemCount(const std::string &itemId) const;
    const std::unordered_map<std::string, int> &GetInventory() const;
    void ClearInventory();

    void ModifyEffect(const std::string &effectId, const int delta);
    bool HasEffect(const std::string &effectId, const int minQuantity = 1) const;
    int GetEffectCount(const std::string &effectId) const;
    const std::unordered_map<std::string, int> &GetEffects() const;
    void ClearEffects();

private:
    std::unordered_map<std::string, int> inventory;
    std::unordered_map<std::string, int> effects;
};

class GameState
{
public:
    GameState(FlagRegistry &flagRegistry,
              ItemLoader &itemLoader,
              VariableRegistry &varRegistry,
              EffectLoader &effectLoader);
    const ItemLoader &GetItemLoader() const;
    const EffectLoader &GetEffectLoader() const;

    bool GetFlag(const std::string &flagId) const;
    void SetFlag(const std::string &flagId, const bool value);
    // void ToggleFlag(const std::string &flagId);
    // void LoadInitialFlags(const std::string& filename) ;

    int GetVariable(const std::string &varId) const;
    void SetVariable(const std::string &varId, const int value);
    void ModifyVariable(const std::string &varId, const int delta);

    void ModifyItem(const std::string &itemId, int delta);

    void ModifyEffect(const std::string &effectId, int delta);

    CharacterData &currentCharacter();
    const CharacterData &currentCharacter() const;

    const std::unordered_map<std::string, bool> &GetCurrentFlags() const;
    const std::unordered_map<std::string, int> &GetCurrentVariables() const;

    // LocationInfo &currentLocation();
private:
    FlagRegistry &flagRegistry;
    ItemLoader &itemLoader;
    VariableRegistry &varRegistry;
    EffectLoader &effectLoader;

    std::unordered_map<std::string, bool> currentFlags;
    std::unordered_map<std::string, int> currentVariables;

    // std::unordered_map<std::string, LocationInfo> locations;
    // std::string currentLocationId;

    // std::unordered_map<std::string, CharacterData> characters;
    CharacterData player;
    std::string currentCharacterId;
};

#endif