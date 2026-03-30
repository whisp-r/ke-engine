#ifndef ITEM_LOADER_H
#define ITEM_LOADER_H
#pragma once

#include <string>
#include <unordered_map>

class ItemDefinition
{
public:
    ItemDefinition(std::string id,
                   std::string displayName,
                   std::string description);
    const std::string &GetId() const;
    const std::string &GetDisplayName() const;
    const std::string &GetDescription() const;

private:
    std::string id;
    std::string displayName;
    std::string description;
};

class ItemLoader
{
public:
    void LoadFromFile(const std::string &filename);
    const ItemDefinition &GetItem(const std::string &itemId) const;
    bool ItemExists(const std::string &itemId) const;

private:
    std::unordered_map<std::string, ItemDefinition> items;
};

#endif