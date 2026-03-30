#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H
#pragma once

#include <unordered_map>
#include <fstream>

#include "dialogue.hpp"
#include "game_state.hpp"
#include "game_constants.hpp"
#include "config_loader.hpp"
#include "action_factory.hpp"
#include "json.hpp"

class DialogueManager
{
public:
    DialogueManager(GameState &state);

    void LoadStory(const std::string &storyFile,
                   const std::string &configFile);
    void LoadFromFile(const std::string &filename);

    DialogueNode *GetNode(const std::string &nodeId);
    DialogueNode *GetCurrentNode();

    void SetStartNode(const std::string &nodeId);
    void Update(); // Call each frame
    void Advance();

    // Choice handling
    const std::vector<Choice> &GetChoices() const;
    int GetChoicesCount() const;
    void SelectChoice(int choiceIndex);
    void SelectChoice(const std::string &choiceText);

    // getters for ui
    RPGText &GetCurrentText() const;
    int GetCurrentTextIndex() const;
    bool isShowingChoices() const;

    // Navigation
    void GoToNode(const std::string &nodeId);

private:
    ConfigLoader config;
    ActionFactory actionFactory;
    GameState &gameState;

    // Graph storage
    std::unordered_map<std::string, std::unique_ptr<DialogueNode>> nodes;

    DialogueNode *currentNode = nullptr;
    int currentTextIndex = 0;

    DialogueNode *startNode = nullptr;

    bool showingChoices = false;

    std::string currentStoryFile;

    void LoadAllNodes(const nlohmann::json &data);
    void LoadSingleNode(const std::string &nodeId, const nlohmann::json &nodeData);
    void LoadNodeEntries(const std::string &nodeId,
                         const nlohmann::json &nodeData,
                         DialogueNode &node);
    RPGText CreateRPGTextFromJSON(const nlohmann::json &textData);
    std::vector<ActionFunc> LoadActionsFromJSON(const nlohmann::json &textData,
                                                const std::string &nodeId);

    void LoadNodeChoices(const std::string &nodeId,
                         const nlohmann::json &nodeData,
                         DialogueNode &node);
    Choice CreateChoiceFromJSON(const nlohmann::json &choiceData,
                                const std::string &nodeId);

    void LoadChoiceConditions(const nlohmann::json &conditionsData,
                              Choice &choice);
};

#endif