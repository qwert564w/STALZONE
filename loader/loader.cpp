// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "loader.h"
#include <fstream>
#include <iostream>
#include <urlmon.h>
#include <shellapi.h>
#pragma comment(lib, "urlmon.lib")

namespace Loader {

    bool Core::LoadConfig(const std::string& path, Config& outConfig) {
        std::ifstream file(path);
        if (!file.is_open()) return false;
        
        // Simple JSON parsing (assuming flat structure for this example)
        // In production, use nlohmann/json
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("\"version\":") != std::string::npos) {
                // parse version
            } else if (line.find("\"dllUrl\":") != std::string::npos) {
                size_t start = line.find('\"') + 1;
                size_t end = line.find('\"', start);
                outConfig.dllUrl = line.substr(start, end - start);
            } else if (line.find("\"installPath\":") != std::string::npos) {
                size_t start = line.find('\"') + 1;
                size_t end = line.find('\"', start);
                outConfig.installPath = line.substr(start, end - start);
            } else if (line.find("\"gameExe\":") != std::string::npos) {
                size_t start = line.find('\"') + 1;
                size_t end = line.find('\"', start);
                outConfig.gameExe = line.substr(start, end - start);
            } else if (line.find("\"autoInject\":") != std::string::npos) {
                outConfig.autoInject = (line.find("true") != std::string::npos);
            }
        }
        return true;
    }

    bool Core::DownloadFile(const std::string& url, const std::string& savePath) {
        std::wcout << L"[*] Downloading " << url.c_str() << L" to " << savePath.c_str() << std::endl;
        HRESULT hr = URLDownloadToFileW(NULL, std::wstring(url.begin(), url.end()).c_str(), 
                                        std::wstring(savePath.begin(), savePath.end()).c_str(), 0, NULL);
        return SUCCEEDED(hr);
    }

    void Core::LaunchGame(const std::string& exePath) {
        ShellExecuteA(NULL, "open", exePath.c_str(), NULL, NULL, SW_SHOW);
    }

    void Core::InjectDLL(const std::string& dllPath, const std::string& processName) {
        // Call Extreme Injector via CLI
        std::string cmd = "tools\\ExtremeInjector.exe -n " + processName + " -d " + dllPath + " -i 2 -auto";
        system(cmd.c_str());
    }
}
