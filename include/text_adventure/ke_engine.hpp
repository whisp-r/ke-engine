#ifndef KE_ENGINE_H
#define KE_ENGINE_H
#pragma once

#include "item_loader.hpp"
#include "flag_registry.hpp"
#include "variable_registry.hpp"
#include "config_loader.hpp"
#include "effect_loader.hpp"

#include "action_factory.hpp"
#include "choice.hpp"
#include "dialogue_manager.hpp"
#include "dialogue.hpp"

#include "condition_factory.hpp"
#include "condition.hpp"
#include "game_state.hpp"

#include "rpg_text.hpp"
#include "ui_manager.hpp"

#include "game_constants.hpp"
#include "helper.hpp"

#include <raylib-cpp.hpp>
#include "json.hpp"

class KeEngine
{
    ItemLoader itemLoader;
    FlagRegistry flagRegistry;
    ConfigLoader configLoader;
    VariableRegistry varRegistry;
    EffectLoader effectLoader;
    GameState gameState;
    DialogueManager dialogueManager;
    UIManager uiManager;

    bool debug;

public:
    KeEngine(bool debug = false);

    // Initialize with file paths
    void Init(const std::string &itemsFile = GameConsts::files::ITEMS,
              const std::string &flagsFile = GameConsts::files::FLAGS,
              const std::string &variablesFile = GameConsts::files::VARIABLES,
              const std::string &storyFile = GameConsts::files::STORY,
              const std::string &configFile = GameConsts::files::CONFIG,
              const std::string &effectsFile = GameConsts::files::EFFECTS);

    // Getters
    GameState &GetGameState();
    DialogueManager &GetDialogueManager();
    const DialogueManager &GetDialogueManager() const;

    // Convenience methods
    void StartStory(const std::string &startNode);
    void Update();
    void DrawUI(); // You can extract your UI drawing code here

    // Input handling
    void HandleInput(); // Move your input logic here
};

#endif