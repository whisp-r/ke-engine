#include "item_loader.hpp"
#include "game_constants.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <unordered_map>

ItemDefinition::ItemDefinition(std::string id, std::string displayName, std::string description)
    : id(id), displayName(displayName), description(description) {}

const std::string &ItemDefinition::GetId() const
{
    return id;
}

const std::string &ItemDefinition::GetDisplayName() const
{
    return displayName;
}

const std::string &ItemDefinition::GetDescription() const
{
    return description;
}

void ItemLoader::LoadFromFile(const std::string &filename)
{
    std::cout << "[ItemLoader] Loading from " << filename << std::endl;

    // Check file exists
    if (!std::filesystem::exists(filename))
    {
        std::string error = "File not found: " + filename;
        std::cerr << "[ERROR][ItemLoader] " << error << std::endl;
        throw std::runtime_error(error);
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::string error = "Cannot open file: " + filename;
        std::cerr << "[ERROR][ItemLoader] " << error << std::endl;
        throw std::runtime_error(error);
    }

    // Parse JSON
    nlohmann::json data;
    try
    {
        data = nlohmann::json::parse(file);
    }
    catch (const nlohmann::json::parse_error &e)
    {
        std::string error = "JSON parse error in " + filename + ": " + e.what();
        std::cerr << "[ERROR][ItemLoader] " << error << std::endl;
        throw std::runtime_error(error);
    }

    // Check structure
    if (!data.contains(GameConsts::state::ITEMS))
    {
        std::cerr << "[WARNING][ItemLoader] items.json missing 'items' key" << std::endl;
        return;
    }

    {
        using namespace GameConsts;
        // Load each item
        int count = 0;
        for (auto &[itemId, itemData] : data[state::ITEMS].items())
        {
            ItemDefinition def(
                itemId,
                itemData.value(item::DISPLAY_NAME, itemId),
                itemData.value(item::DESCRIPTION, item::DESCRIPTION_DEFAULT));

            items.emplace(itemId, std::move(def));
            count++;

            std::cout << '\t'
                      << "[ItemLoader] Loaded: " << itemId << " -> "
                      << items.at(itemId).GetDisplayName()
                      << " ["
                      << items.at(itemId).GetDescription()
                      << "]" << std::endl;
        }

        std::cout << "[ItemLoader] Successfully loaded " << count << " items" << std::endl;
    }
}

const ItemDefinition &ItemLoader::GetItem(const std::string &itemId) const
{
    auto it = items.find(itemId);
    if (it == items.end())
    {
        throw std::runtime_error("[ERROR][ItemLoader] Unknown item: " + itemId);
    }
    return it->second;
}

bool ItemLoader::ItemExists(const std::string &itemId) const
{
    return items.find(itemId) != items.end();
}
