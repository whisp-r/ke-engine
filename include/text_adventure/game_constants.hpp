#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H
#pragma once

#include <string>

namespace GameConsts
{
    // JSON field names
    namespace node
    {
        const std::string STORY = "story";
        const std::string TEXT = "text";
        const std::string CONTENT = "content";
        const std::string COLOR = "color";
        const std::string FONT_SIZE = "font_size";
        const std::string SPEED = "speed";
        const std::string CHOICES = "choices";
        const std::string TARGET = "target";
        const std::string TEXTS = "texts";
        const std::string DEFAULT_NEXT = "default_next";
        const std::string SPEAKER = "speaker";

    }
    namespace action
    {
        const std::string ACTIONS = "actions";
        const std::string TYPE = "type";

        const std::string MODIFY_ITEM = "modify_item";

        const std::string SET_FLAG = "set_flag";

        const std::string SET_VAR = "set_variable";
        const std::string MODIFY_VAR = "modify_variable";

        const std::string MODIFY_EFFECT = "modify_effect";
    }
    namespace flag
    {
        const std::string FLAG = "flag";
        const std::string VALUE = "value";
        const bool VALUE_DEFAULT = true;
    }
    namespace item
    {
        const std::string ITEM = "item";
        const std::string AMOUNT = "amount";
        const int AMOUNT_DEFAULT = 1;

        const std::string DISPLAY_NAME = "display_name";
        const std::string DESCRIPTION = "description";
        const std::string DESCRIPTION_DEFAULT = "default_description";

    }
    namespace effect{
        const std::string EFFECT = "effect";
        const std::string AMOUNT = "amount";
        const int AMOUNT_DEFAULT = 1;

        const std::string DISPLAY_NAME = "display_name";
        const std::string DESCRIPTION = "description";
        const std::string DESCRIPTION_DEFAULT = "default_description";
    }
    namespace var
    {
        const std::string VARIABLE = "variable";
        const std::string AMOUNT = "amount";
        const int AMOUNT_DEFAULT = 0;
    }
    namespace config
    {
        const std::string CONFIG = "config";
        const std::string TEXT_STYLE = "text_style";
        const std::string DEFAULT = "default";
        const std::string SPEAKERS = "speakers";
    };
    namespace files
    {
        const std::string CONFIG = "./data/config.json";
        const std::string STORY = "./data/story.json";
        const std::string ITEMS = "./data/items.json";
        const std::string FLAGS = "./data/flags.json";
        const std::string VARIABLES = "./data/variables.json";
        const std::string EFFECTS = "./data/effects.json";
    }

    namespace state
    {
        // const std::string STATES = "states";
        const std::string ITEMS = "items";
        const std::string FLAGS = "flags";
        const std::string VARIABLES = "variables";
        const std::string EFFECTS = "effects";
    }
    namespace condition
    {
        const std::string TYPE = "type";

        const std::string HAS_ITEM = "has_item";
        const std::string FLAG_SET = "flag_set";
        const std::string VAR_COMP = "variable_compare";
        const std::string HAS_EFFECT = "has_effect";

        const std::string FLAG = "flag";
        const std::string ITEM = "item";

        const std::string COMP = "comparison";
        namespace comp
        {
            const std::string EQUAL = "==";
            const std::string NOT_EQUAL = "!=";
            const std::string GREATER_THAN = ">";
            const std::string GREATER_EQUAL = ">=";
            const std::string LESS_THAN = "<";
            const std::string LESS_EQUAL = "<=";
        }
    }

}

#endif