// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "loader.h"
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

// Simple console-based loader UI. 
// Can be upgraded to ImGui later.
int main() {
    SetConsoleTitleA("VV.EXE Loader - STALZONE");
    std::cout << "==========================================" << std::endl;
    std::cout << "   VV.EXE LOADER - STALZONE" << std::endl;
    std::cout << "==========================================" << std::endl;

    Loader::Config config;
    if (!Loader::Core::LoadConfig("config.json", config)) {
        std::cerr << "[-] Failed to load config.json. Creating default..." << std::endl;
        config.dllUrl = "https://example.com/vv.dll";
        config.installPath = "C:\\Temp\\vv.dll";
        config.gameExe = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\STALZONE\\game.exe";
        config.autoInject = true;
    }

    std::cout << "[*] Downloading latest cheat module..." << std::endl;
    if (Loader::Core::DownloadFile(config.dllUrl, config.installPath)) {
        std::cout << "[+] Download successful!" << std::endl;
    } else {
        std::cerr << "[-] Download failed. Using local copy if exists." << std::endl;
    }

    if (config.autoInject) {
        std::cout << "[*] Launching game..." << std::endl;
        Loader::Core::LaunchGame(config.gameExe);
        
        std::cout << "[*] Waiting for game to initialize (10s)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
        std::cout << "[*] Injecting DLL..." << std::endl;
        Loader::Core::InjectDLL(config.installPath, "Stalker.exe");
    }

    std::cout << "[+] Loader finished. Press Enter to exit." << std::endl;
    std::cin.get();
    return 0;
}
