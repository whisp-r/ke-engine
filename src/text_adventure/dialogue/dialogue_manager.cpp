#include "dialogue_manager.hpp"
#include "helper.hpp"
#include "game_constants.hpp"
#include "json.hpp"
#include "condition_factory.hpp"

#include <iostream>
#include <stdexcept>

DialogueManager::DialogueManager(GameState &state)
    : gameState(state) {}

void DialogueManager::LoadStory(const std::string &storyFile, const std::string &configFile)
{
    config.LoadFromFile(configFile);
    LoadFromFile(storyFile);
}

void DialogueManager::LoadFromFile(const std::string &filename)
{
    currentStoryFile = filename;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "[ERROR][DialogueMngr] Could not open file: " << filename << std::endl;
        throw std::runtime_error("");
        // return;
    }

    try
    {
        nlohmann::json data = nlohmann::json::parse(file);
        LoadAllNodes(data);
    }
    catch (const nlohmann::json::exception &e)
    {
        std::cerr << "[ERROR][DialogueMngr] JSON parsing failed in " << filename << ": " << e.what() << std::endl;
        throw std::runtime_error("");
        // return;
    }
}

void DialogueManager::LoadAllNodes(const nlohmann::json &data)
{
    using namespace GameConsts;

    if (!data.contains(node::STORY))
    {
        std::cerr << "[ERROR][DialogueMngr] JSON missing 'story' section" << std::endl;
        return;
    }

    for (auto &[nodeId, nodeData] : data[node::STORY].items())
    {
        LoadSingleNode(nodeId, nodeData);
    }
}

void DialogueManager::LoadSingleNode(const std::string &nodeId, const nlohmann::json &nodeData)
{
    using namespace GameConsts;

    try
    {
        std::string defaultNext = nodeData.value(node::DEFAULT_NEXT, "");
        auto node = std::make_unique<DialogueNode>(nodeId, defaultNext);

        LoadNodeEntries(nodeId, nodeData, *node);
        LoadNodeChoices(nodeId, nodeData, *node);

        nodes[nodeId] = std::move(node);
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR][DialogueMngr] Loading node '" << nodeId << "': " << e.what() << std::endl;
    }
}

void DialogueManager::LoadNodeEntries(const std::string &nodeId,
                                      const nlohmann::json &nodeData,
                                      DialogueNode &node)
{
    using namespace GameConsts;

    if (!nodeData.contains(node::TEXTS))
    {
        return; // Node can have no texts (just choices)
    }

    for (const auto &textData : nodeData[node::TEXTS])
    {
        try
        {
            RPGText rpgText = CreateRPGTextFromJSON(textData);
            std::vector<ActionFunc> actions = LoadActionsFromJSON(textData, nodeId);
            node.AddEntry(std::move(rpgText), std::move(actions));
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR][DialogueMngr] In node '" << nodeId << "' text entry: " << e.what() << std::endl;
        }
    }
}

RPGText DialogueManager::CreateRPGTextFromJSON(const nlohmann::json &textData)
{
    using namespace GameConsts;

    if (!textData.contains(node::CONTENT))
    {
        throw std::invalid_argument("[ERROR][DialogueMngr] Text entry missing 'content' field");
    }

    std::string speaker = textData.value(node::SPEAKER, "");
    const TextStyle &baseStyle = config.GetSpeakerStyle(speaker);

    TextStyle finalStyle;
    finalStyle.LoadFromJSON(textData, baseStyle);

    return RPGText(
        textData[node::CONTENT].get<std::string>(),
        finalStyle.GetFontSize(),
        finalStyle.GetColor(),
        finalStyle.GetSpeed());
}

std::vector<ActionFunc> DialogueManager::LoadActionsFromJSON(const nlohmann::json &textData,
                                                             const std::string &nodeId)
{
    using namespace GameConsts;
    std::vector<ActionFunc> actions;

    if (!textData.contains(action::ACTIONS))
    {
        return actions;
    }

    for (const auto &actionData : textData[action::ACTIONS])
    {
        try
        {
            auto action = actionFactory.Create(actionData);
            if (action)
            {
                actions.push_back(std::move(action));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR][DialogueMngr] In node '" << nodeId << "' action: " << e.what() << std::endl;
        }
    }

    return actions;
}

void DialogueManager::LoadNodeChoices(const std::string &nodeId,
                                      const nlohmann::json &nodeData,
                                      DialogueNode &node)
{
    using namespace GameConsts;

    if (!nodeData.contains(node::CHOICES))
    {
        return; // Node can have no choices
    }

    for (const auto &choiceData : nodeData[node::CHOICES])
    {
        try
        {
            Choice choice = CreateChoiceFromJSON(choiceData, nodeId);
            node.AddChoice(std::move(choice));
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR][DialogueMngr] In node '" << nodeId << "' choice: " << e.what() << std::endl;
        }
    }
}
Choice DialogueManager::CreateChoiceFromJSON(const nlohmann::json &choiceData,
                                             const std::string &nodeId)
{
    using namespace GameConsts;

    if (!choiceData.contains(node::TEXT))
    {
        throw std::invalid_argument("[ERROR][DialogueMngr] Choice missing 'text' field");
    }
    if (!choiceData.contains(node::TARGET))
    {
        throw std::invalid_argument("[ERROR][DialogueMngr] Choice missing 'target' field");
    }

    std::string text = choiceData[node::TEXT].get<std::string>();
    std::string target = choiceData[node::TARGET].get<std::string>();

    Choice choice(text, target);

    if (choiceData.contains("conditions"))
    {
        LoadChoiceConditions(choiceData["conditions"], choice);
    }

    return choice;
}

void DialogueManager::LoadChoiceConditions(const nlohmann::json &conditionsData,
                                           Choice &choice)
{
    for (const auto &condData : conditionsData)
    {
        try
        {
            auto condition = ConditionFactory::CreateFromJSON(condData);
            if (condition)
            {
                choice.AddCondition(std::move(condition));
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR][DialogueMngr] Loading condition: " << e.what() << std::endl;
        }
    }
}

DialogueNode *DialogueManager::GetNode(const std::string &nodeId)
{
    auto it = nodes.find(nodeId);
    if (it != nodes.end())
    {
        return it->second.get();
    }

    std::cerr << "[ERROR][DialogueMngr] Node '" << nodeId << "' not found" << std::endl;
    return nullptr;
}

DialogueNode *DialogueManager::GetCurrentNode()
{
    return currentNode;
}

void DialogueManager::SetStartNode(const std::string &nodeId)
{
    startNode = GetNode(nodeId);
}

void DialogueManager::Update()
{
    if (!currentNode)
        return;

    if (!showingChoices)
    {
        // Update current text animation
        GetCurrentText().Update();
    }
}

void DialogueManager::Advance()
{
    if (!currentNode)
        return;

    // Get current text entry
    auto &currentEntry = currentNode->GetEntry(currentTextIndex);

    // If text is still animating, skip it
    if (!currentEntry.GetText().IsComplete())
    {
        currentEntry.GetText().Skip();
        return;
    }

    // Text is complete → EXECUTE ITS ACTIONS
    currentNode->ExecuteEntryActions(gameState, currentTextIndex);

    // Move to next text
    currentTextIndex++;

    // Check if we finished all texts
    if (currentTextIndex >= currentNode->GetEntryCount())
    {
        if (currentNode->GetChoices().empty() &&
            currentNode->HasDefaultNext())
        {
            // Auto-advance to next node
            GoToNode(currentNode->GetDefaultNext());
        }
        else
        {
            // Show choices
            showingChoices = true;
        }
    }
}

int DialogueManager::GetChoicesCount() const
{
    if (!currentNode)
        return 0;
    return currentNode->GetChoices().size();
}

const std::vector<Choice> &DialogueManager::GetChoices() const
{
    static const std::vector<Choice> empty;
    if (!currentNode)
        return empty;
    return currentNode->GetChoices();
}

void DialogueManager::SelectChoice(int choiceIndex)
{
    const auto &choices = currentNode->GetChoices();

    if (choiceIndex < 0 || choiceIndex >= choices.size())
    {
        std::cerr << "[ERROR][DialogueMngr] Invalid choice index " << choiceIndex << std::endl;
        return;
    }

    GoToNode(choices[choiceIndex].GetTargetNodeId());
}

RPGText &DialogueManager::GetCurrentText() const
{
    return currentNode->GetEntry(currentTextIndex).GetText();
}

int DialogueManager::GetCurrentTextIndex() const
{
    return currentTextIndex;
}

bool DialogueManager::isShowingChoices() const
{
    return showingChoices;
}

void DialogueManager::GoToNode(const std::string &nodeId)
{
    DialogueNode *newNode = GetNode(nodeId);
    if (!newNode)
        return;

    // Reset for new node
    currentNode = newNode;
    currentTextIndex = 0;
    showingChoices = false;
    currentNode->ResetAllTexts();
}
