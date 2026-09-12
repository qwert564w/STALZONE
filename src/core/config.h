#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
struct ConfigEntry { std::string name; std::string date; bool active = false; nlohmann::json data; };
namespace ConfigManager { extern std::vector<ConfigEntry> configs; extern int activeIndex; void Init(const std::string& dir); void Save(const std::string& name); void Load(int idx); void Delete(int idx); void Rename(int idx, const std::string& newName); void Duplicate(int idx); void Export(const std::string& path); void Import(const std::string& path); void OverwriteActive(); std::string GetTimestamp(); }