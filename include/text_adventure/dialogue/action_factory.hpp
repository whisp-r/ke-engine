#ifndef ACTION_FACTORY_H
#define ACTION_FACTORY_H
#pragma once

#include "json.hpp"
#include "game_state.hpp"

#include <functional>

using ActionFunc = std::function<void(GameState &)>;

class ActionFactory
{
public:
    static ActionFunc Create(const nlohmann::json &actionData);

private:
    static ActionFunc CreateModifyItem(const nlohmann::json &data);
    static ActionFunc CreateSetFlag(const nlohmann::json &data);
    static ActionFunc CreateSetVariable(const nlohmann::json &data);
    static ActionFunc CreateModifyVariable(const nlohmann::json &data);
    static ActionFunc CreateModifyEffect(const nlohmann::json &data);
};

#endif