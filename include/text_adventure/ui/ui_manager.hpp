#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP
#pragma once

#include <raylib-cpp.hpp>
#include "game_state.hpp"
#include "dialogue_manager.hpp"

class UIManager
{
private:
    // References to game systems
    GameState &gameState;
    DialogueManager &dialogueManager;

    // UI state
    bool showDebugInfo;

    // UI layout - as regular member variables
    int textStartX;
    int textStartY;
    int textLineSpacing;

    int choiceStartX;
    int choiceStartY;
    int choiceSpacing;

    int debugStartX;
    int flagsStartY;
    int variablesStartY;
    int inventoryStartX;
    int inventoryStartY;
    int effectsStartX;
    int effectsStartY;
    int debugSpacing;

    float lineSpacingMultiplier; // Multiplier for font size

    // Colors - as regular member variables
    raylib::Color backgroundColor;
    raylib::Color defaultTextColor;
    raylib::Color activeChoiceColor;
    raylib::Color inactiveChoiceColor;
    raylib::Color flagTrueColor;
    raylib::Color flagFalseColor;
    raylib::Color inventoryColor;
    raylib::Color effectsColor;
    raylib::Color variableColor;

public:
    UIManager(GameState &gameState, DialogueManager &dialogueManager, bool debug = false);

    void SetLineSpacingMultiplier(float multiplier) { lineSpacingMultiplier = multiplier; }
    float GetLineSpacingMultiplier() const { return lineSpacingMultiplier; }

    // Main drawing methods
    void Draw();

    // Input handling
    void HandleInput();
    bool HandleChoiceInput();

    // Toggle debug info
    void ToggleDebugInfo() { showDebugInfo = !showDebugInfo; }
    bool IsDebugInfoVisible() const { return showDebugInfo; }

private:
    // Drawing components
    void DrawCurrentDialogue();
    void DrawChoices();
    void DrawDebugPanel();

    // Debug panel components
    void DrawFlags();
    void DrawVariables();
    void DrawInventory();
    void DrawEffects();
};

#endif