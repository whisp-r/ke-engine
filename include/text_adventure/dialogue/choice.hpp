#ifndef CHOICE_H
#define CHOICE_H
#pragma once

#include "condition.hpp"

#include <string>
#include <vector>

class Choice
{
public:
    Choice(std::string text, std::string targetNodeId);
    const std::string &GetText() const;
    const std::string &GetTargetNodeId() const;

    // conditions
    void AddCondition(ConditionPtr condition);

    bool IsAvailable(const GameState &gameState) const;

private:
    std::string text;
    std::string targetNodeId;
    std::vector<ConditionPtr> conditions;
};

#endif