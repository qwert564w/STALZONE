// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "loader.h"
#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <urlmon.h>
#include <tlhelp32.h>

#pragma comment(lib, "urlmon.lib")

struct Config {
    std::string dllUrl;
    std::string installPath;
    std::string gameExe;
    std::string gameProcessName;
    int waitTimeBeforeInject;
};

Config LoadConfig() {
    Config config;
    config.dllUrl = "https://example.com/vv.dll";
    config.installPath = "C:\\VV\\vv.dll";
    config.gameExe = "C:\\Games\\STALZONE\\game.exe";
    config.gameProcessName = "game.exe";
    config.waitTimeBeforeInject = 10;
    
    std::ifstream file("config.json");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("\"dll_url\"") != std::string::npos) {
                size_t start = line.find("\"", line.find(":") + 1) + 1;
                size_t end = line.find("\"", start);
                config.dllUrl = line.substr(start, end - start);
            }
            else if (line.find("\"install_path\"") != std::string::npos) {
                size_t start = line.find("\"", line.find(":") + 1) + 1;
                size_t end = line.find("\"", start);
                config.installPath = line.substr(start, end - start);
            }
            else if (line.find("\"game_exe\"") != std::string::npos) {
                size_t start = line.find("\"", line.find(":") + 1) + 1;
                size_t end = line.find("\"", start);
                config.gameExe = line.substr(start, end - start);
            }
            else if (line.find("\"game_process_name\"") != std::string::npos) {
                size_t start = line.find("\"", line.find(":") + 1) + 1;
                size_t end = line.find("\"", start);
                config.gameProcessName = line.substr(start, end - start);
            }
            else if (line.find("\"wait_time\"") != std::string::npos) {
                size_t start = line.find(":", line.find("wait_time")) + 1;
                size_t end = line.find(",", start);
                if (end == std::string::npos) end = line.find("}", start);
                config.waitTimeBeforeInject = std::stoi(line.substr(start, end - start));
            }
        }
        file.close();
    }
    
    return config;
}

bool DownloadDLL(const std::string& url, const std::string& path) {
    std::cout << "[LOADER] Downloading DLL from " << url << std::endl;
    
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), path.c_str(), 0, NULL);
    
    if (SUCCEEDED(hr)) {
        std::cout << "[LOADER] ✓ DLL downloaded: " << path << std::endl;
        return true;
    } else {
        std::cout << "[LOADER] ✗ Download error: " << hr << std::endl;
        return false;
    }
}

DWORD GetProcessId(const std::string& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return 0;
    
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);
    
    if (Process32First(snapshot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));
    }
    
    CloseHandle(snapshot);
    return 0;
}

bool InjectDLL(DWORD processId, const std::string& dllPath) {
    std::cout << "[LOADER] Injecting DLL into process " << processId << std::endl;
    
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!process) {
        std::cout << "[LOADER] ✗ Failed to open process" << std::endl;
        return false;
    }
    
    void* remoteMemory = VirtualAllocEx(process, NULL, dllPath.length() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMemory) {
        std::cout << "[LOADER] ✗ Failed to allocate memory" << std::endl;
        CloseHandle(process);
        return false;
    }
    
    if (!WriteProcessMemory(process, remoteMemory, dllPath.c_str(), dllPath.length() + 1, NULL)) {
        std::cout << "[LOADER] ✗ Failed to write path" << std::endl;
        VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }
    
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC loadLibrary = GetProcAddress(kernel32, "LoadLibraryA");
    
    HANDLE thread = CreateRemoteThread(process, NULL, 0, 
        (LPTHREAD_START_ROUTINE)loadLibrary, remoteMemory, 0, NULL);
    
    if (!thread) {
        std::cout << "[LOADER] ✗ Failed to create thread" << std::endl;
        VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }
    
    WaitForSingleObject(thread, INFINITE);
    
    std::cout << "[LOADER] ✓ DLL injected successfully!" << std::endl;
    
    VirtualFreeEx(process, remoteMemory, 0, MEM_RELEASE);
    CloseHandle(thread);
    CloseHandle(process);
    
    return true;
}

void StartGame(const std::string& gameExe) {
    std::cout << "[LOADER] Starting game: " << gameExe << std::endl;
    
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    
    if (!CreateProcessA(gameExe.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cout << "[LOADER] ✗ Failed to start game" << std::endl;
        return;
    }
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    std::cout << "[LOADER] ✓ Game started!" << std::endl;
}

int main() {
    SetConsoleTitleA("VV.EXE Loader - STALZONE");
    
    std::cout << "=====================================" << std::endl;
    std::cout << "  VV.EXE Loader v2.0" << std::endl;
    std::cout << "  COPYRIGHT VV.EXE Development Team 2026" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;
    
    Config config = LoadConfig();
    
    std::cout << "[LOADER] Configuration:" << std::endl;
    std::cout << "  DLL URL: " << config.dllUrl << std::endl;
    std::cout << "  Install Path: " << config.installPath << std::endl;
    std::cout << "  Game EXE: " << config.gameExe << std::endl;
    std::cout << "  Process Name: " << config.gameProcessName << std::endl;
    std::cout << "  Wait Time: " << config.waitTimeBeforeInject << " sec" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Select action:" << std::endl;
    std::cout << "  1. Download DLL and start game" << std::endl;
    std::cout << "  2. Download DLL only" << std::endl;
    std::cout << "  3. Start game only" << std::endl;
    std::cout << "  4. Inject into running game" << std::endl;
    std::cout << std::endl;
    std::cout << "Your choice: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            if (!DownloadDLL(config.dllUrl, config.installPath)) {
                std::cout << "[LOADER] Download error. Continue? (y/n)" << std::endl;
                char c;
                std::cin >> c;
                if (c != 'y' && c != 'Y') return 1;
            }
            
            StartGame(config.gameExe);
            
            std::cout << "[LOADER] Waiting " << config.waitTimeBeforeInject << " seconds..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(config.waitTimeBeforeInject));
            
            DWORD pid = GetProcessId(config.gameProcessName);
            if (pid == 0) {
                std::cout << "[LOADER] ✗ Game process not found!" << std::endl;
                return 1;
            }
            
            if (!InjectDLL(pid, config.installPath)) {
                std::cout << "[LOADER] ✗ Injection error!" << std::endl;
                return 1;
            }
            
            std::cout << "[LOADER] ✓ Done! Press Enter to exit..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;
        }
        
        case 2: {
            DownloadDLL(config.dllUrl, config.installPath);
            std::cout << "[LOADER] Press Enter to exit..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;
        }
        
        case 3: {
            StartGame(config.gameExe);
            std::cout << "[LOADER] Press Enter to exit..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;
        }
        
        case 4: {
            DWORD pid = GetProcessId(config.gameProcessName);
            if (pid == 0) {
                std::cout << "[LOADER] ✗ Game process not found!" << std::endl;
                std::cout << "[LOADER] Press Enter to exit..." << std::endl;
                std::cin.ignore();
                std::cin.get();
                return 1;
            }
            
            InjectDLL(pid, config.installPath);
            std::cout << "[LOADER] Press Enter to exit..." << std::endl;
            std::cin.ignore();
            std::cin.get();
            break;
        }
        
        default:
            std::cout << "[LOADER] Invalid choice!" << std::endl;
            break;
    }
    
    return 0;
}
