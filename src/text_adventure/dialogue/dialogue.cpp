#include "dialogue.hpp"
#include <iostream>

DialogueNode::DialogueNode(const std::string nodeId,
                           const std::string defaultNextNodeId)
    : nodeId(nodeId), defaultNextNodeId(defaultNextNodeId)
{
    if (nodeId.empty())
    {
        std::cerr << "[WARNING][DialogueNode] Created DialogueNode with empty nodeId" << std::endl;
    }
}

void DialogueNode::AddEntry(RPGText text, std::vector<ActionFunc> actions)
{
    entries.emplace_back(std::move(text), std::move(actions));
}

void DialogueNode::AddChoice(const std::string &text, const std::string &target)
{
    if (text.empty())
    {
        std::cerr << "[WARNING][DialogueNode] Adding choice with empty text in node: " << nodeId << std::endl;
    }
    if (target.empty())
    {
        std::cerr << "[WARNING][DialogueNode] Adding choice with empty target in node: " << nodeId << std::endl;
    }

    choices.emplace_back(text, target);
}

const std::string &DialogueNode::GetNodeId() const
{
    return nodeId;
}

DialogueEntry &DialogueNode::GetEntry(int index)
{
    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::string error = "[ERROR][DialogueNode] GetEntry index out of bounds: " + std::to_string(index) +
                            " (size: " + std::to_string(entries.size()) +
                            ") in node: " + nodeId;
        throw std::out_of_range(error);
    }

    return entries.at(index);
}

int DialogueNode::GetEntryCount() const
{
    return static_cast<int>(entries.size());
}

void DialogueNode::AddChoice(Choice choice)
{
    if (choice.GetText().empty())
    {
        std::cerr << "[WARNING][DialogueNode] Adding choice with empty text in node: " << nodeId << std::endl;
    }
    if (choice.GetTargetNodeId().empty())
    {
        std::cerr << "[WARNING][DialogueNode] Adding choice with empty target in node: " << nodeId << std::endl;
    }

    choices.push_back(std::move(choice));
}

const std::vector<Choice> &DialogueNode::GetChoices() const
{
    return choices;
}

bool DialogueNode::HasDefaultNext() const
{
    return !defaultNextNodeId.empty();
}
void DialogueNode::ResetAllTexts()
{
    for (auto &text : entries)
        text.GetText().Reset();
}

void DialogueNode::ExecuteEntryActions(GameState &gameState, const int index)
{
    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::cerr << "[ERROR][DialogueNode] ExecuteEntryActions index out of bounds: "
                  << index << " in node: " << nodeId << std::endl;
        return;
    }

    for (auto &action : GetEntry(index).GetActions())
    {
        if (action)
        { // Check if valid function
            try
            {
                action(gameState); // execute action
            }
            catch (const std::exception &e)
            {
                std::cerr << "[ERROR][DialogueNode] Action execution failed in node "
                          << nodeId << " entry " << index << ": "
                          << e.what() << std::endl;
            }
        }
    }
}

const std::string &DialogueNode::GetDefaultNext() const
{
    return defaultNextNodeId;
}

DialogueEntry::DialogueEntry(RPGText text, std::vector<ActionFunc> actions)
    : text(std::move(text)), actions(std::move(actions)) {}

RPGText &DialogueEntry::GetText()
{
    return text;
}

const RPGText &DialogueEntry::GetText() const
{
    return text;
}

std::vector<ActionFunc> &DialogueEntry::GetActions()
{
    return actions;
}

// Optional: Add bounds-checked version for const access
const DialogueEntry &DialogueNode::GetEntry(int index) const
{
    if (index < 0 || index >= static_cast<int>(entries.size()))
    {
        std::string error = "[ERROR][DialogueNode] GetEntry index out of bounds: " + std::to_string(index) +
                            " (size: " + std::to_string(entries.size()) +
                            ") in node: " + nodeId;
        throw std::out_of_range(error);
    }

    return entries.at(index);
}