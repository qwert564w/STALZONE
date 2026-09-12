#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
struct ConfigEntry { std::string name; std::string date; bool active = false; nlohmann::json data; };
namespace ConfigManager {
    extern std::vector<ConfigEntry> configs;
    extern int activeIndex;
    void Save(const std::string& name);
    void Load(int idx);
    void Delete(int idx);
}