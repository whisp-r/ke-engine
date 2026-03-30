#ifndef DIALOGUE_NODE_H
#define DIALOGUE_NODE_H
#pragma once

#include "rpg_text.hpp"
#include "action_factory.hpp"
#include "choice.hpp"
#include <vector>
#include <memory>
#include <functional>

class DialogueEntry
{
    RPGText text;
    std::vector<ActionFunc> actions;

public:
    DialogueEntry(RPGText text, std::vector<ActionFunc> actions);
    RPGText &GetText();
    const RPGText &GetText() const;
    std::vector<ActionFunc> &GetActions();
};
class DialogueNode
{

    std::string nodeId;
    // std::vector<std::unique_ptr<RPGText>> texts;
    std::vector<DialogueEntry> entries;
    std::vector<Choice> choices;

    std::string defaultNextNodeId;

    bool visited;

public:
    DialogueNode(const std::string nodeId,
                 const std::string defaultNextNodeId);
    // bool AllTextCompleted() const;
    void AddEntry(RPGText text, std::vector<ActionFunc> actions = {});
    const std::string &GetNodeId() const;

    DialogueEntry &GetEntry(int index);
    const DialogueEntry &GetEntry(int index) const;
    int GetEntryCount() const;

    void AddChoice(Choice choice);
    void AddChoice(const std::string &text, const std::string &targetId);
    const std::vector<Choice> &GetChoices() const;

    const std::string &GetDefaultNext() const;
    bool HasDefaultNext() const;
    // void SetDefaultNext(std::string& nodeId);

    void ResetAllTexts();

    // Actions
    void ExecuteEntryActions(GameState &gameState, const int index);
};

#endif