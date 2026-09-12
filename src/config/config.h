#pragma once
#include <string>
namespace Config {
    void Save(const std::string& filename);
    void Load(const std::string& filename);
}
