#include "config.h"
#include <fstream>
// Requires nlohmann::json header in real build
namespace Config {
    void Save(const std::string& filename) {}
    void Load(const std::string& filename) {}
}
