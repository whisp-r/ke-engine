#ifndef CONDITION_FACTORY_H
#define CONDITION_FACTORY_H
#pragma once

#include "condition.hpp"
#include "json.hpp"

class ConditionFactory
{
public:
    static ConditionPtr CreateFromJSON(const nlohmann::json &data);
};

#endif