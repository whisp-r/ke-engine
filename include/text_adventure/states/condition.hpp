#ifndef CONDITION_H
#define CONDITION_H
#pragma once

#include "game_state.hpp"
// #include "game_constants.hpp"

#include <memory>
#include <string>

class Condition
{
public:
    virtual ~Condition() = default;
    virtual bool Evaluate(const GameState &gameState) const = 0;
    virtual std::unique_ptr<Condition> Clone() const = 0;
};

using ConditionPtr = std::unique_ptr<Condition>;

class ItemCondition : public Condition
{
public:
    ItemCondition(const std::string &itemId,
                  int quantity,
                  const std::string &comp);

    bool Evaluate(const GameState &gameState) const override;
    std::unique_ptr<Condition> Clone() const override;

private:
    std::string itemId;
    int quantity;
    std::string comparison;
};

class FlagCondition : public Condition
{
public:
    FlagCondition(const std::string &flagId,
                  const bool value,
                  std::string comp);

    bool Evaluate(const GameState &gameState) const override;
    std::unique_ptr<Condition> Clone() const override;

private:
    std::string flagId;
    bool value;
    std::string comparison;
};

class VariableCondition : public Condition
{
public:
    VariableCondition(const std::string &varId,
                      const int value,
                      const std::string &comp);

    bool Evaluate(const GameState &gameState) const override;
    std::unique_ptr<Condition> Clone() const override;

private:
    std::string varId;
    int value;
    std::string comparison;
};

class EffectCondition : public Condition
{
public:
    EffectCondition(const std::string &effectId,
                    const int stacks,
                    const std::string &comp);

    bool Evaluate(const GameState &gameState) const override;
    std::unique_ptr<Condition> Clone() const override;

private:
    std::string effectId;
    int stacks;
    std::string comparison;
};

#endif