// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <string>
#include <windows.h>

namespace Loader {
    struct Config {
        std::string version;
        std::string dllUrl;
        std::string installPath;
        std::string gameExe;
        bool autoInject;
    };

    class Core {
    public:
        static bool LoadConfig(const std::string& path, Config& outConfig);
        static bool DownloadFile(const std::string& url, const std::string& savePath);
        static void LaunchGame(const std::string& exePath);
        static void InjectDLL(const std::string& dllPath, const std::string& processName);
    };
}
