#include "ke_engine.hpp"

#include <iostream>

KeEngine::KeEngine(bool debug)
    : gameState(flagRegistry, itemLoader, varRegistry, effectLoader),
      dialogueManager(gameState),
      uiManager(gameState, dialogueManager, debug)
{
}

void KeEngine::Init(const std::string &itemsFile,
                  const std::string &flagsFile,
                  const std::string &variablesFile,
                  const std::string &storyFile,
                  const std::string &configFile,
                  const std::string &effectsFile)
{
    try
    {
        itemLoader.LoadFromFile(itemsFile);
        flagRegistry.LoadFromFile(flagsFile);
        varRegistry.LoadFromFile(variablesFile);
        effectLoader.LoadFromFile(effectsFile);

        configLoader.LoadFromFile(configFile);

        dialogueManager.LoadStory(storyFile, configFile);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Engine initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void KeEngine::StartStory(const std::string &startNode)
{
    dialogueManager.SetStartNode(startNode);
    dialogueManager.GoToNode(startNode);
}

void KeEngine::Update()
{
    dialogueManager.Update();
}

void KeEngine::DrawUI()
{
    uiManager.Draw();
}

void KeEngine::HandleInput()
{
    uiManager.HandleInput();
}

GameState &KeEngine::GetGameState()
{
    return gameState;
}

DialogueManager &KeEngine::GetDialogueManager()
{
    return dialogueManager;
}

const DialogueManager &KeEngine::GetDialogueManager() const
{
    return dialogueManager;
}
