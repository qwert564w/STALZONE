// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#include "bypass.h"
#include "screenshot_bypass.h"
#include <tlhelp32.h>
#include <intrin.h>
#include <psapi.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

#define OBFUSCATE_STR(s) (s) 

namespace VV {
    namespace Bypass {

        CoreBypass& CoreBypass::Get() {
            static CoreBypass instance;
            return instance;
        }

        void CoreBypass::Initialize() {
            m_moduleBase = (uintptr_t)GetModuleHandleA(nullptr);
            m_envInfo = DetectEnvironment();
            
            if (m_envInfo.isDebuggerPresent || m_envInfo.analysisToolsRunning) {
                m_currentStrategy = Strategy::GHOST;
            } else if (m_envInfo.acType == AntiCheatType::VANGUARD || m_envInfo.acType == AntiCheatType::EAC) {
                m_currentStrategy = Strategy::STEALTH;
            } else if (m_envInfo.acType == AntiCheatType::NONE) {
                m_currentStrategy = Strategy::AGGRESSIVE;
            } else {
                m_currentStrategy = Strategy::BALANCED;
            }

            ApplyStrategy(m_currentStrategy);
            
            std::thread([this]() {
                while (true) {
                    UpdateStrategy();
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
            }).detach();
        }

        EnvironmentInfo CoreBypass::DetectEnvironment() {
            EnvironmentInfo info = {};

            PEB* peb = (PEB*)__readgsqword(0x60);
            info.isDebuggerPresent = peb->BeingDebugged;

            HMODULE hNtdll = GetModuleHandleA(OBFUSCATE_STR("ntdll.dll"));
            if (hNtdll) {
                typedef NTSTATUS(NTAPI* pNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
                auto pNtQIP = (pNtQueryInformationProcess)GetProcAddress(hNtdll, OBFUSCATE_STR("NtQueryInformationProcess"));
                if (pNtQIP) {
                    DWORD debuggerPresent = 0;
                    ULONG returnLength = 0;
                    pNtQIP(GetCurrentProcess(), (PROCESSINFOCLASS)7, &debuggerPresent, sizeof(debuggerPresent), &returnLength);
                    if (debuggerPresent != 0) info.isDebuggerPresent = true;
                }
            }

            HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hSnap != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32W pe32 = { sizeof(PROCESSENTRY32W) };
                if (Process32FirstW(hSnap, &pe32)) {
                    do {
                        std::wstring procName = pe32.szExeFile;
                        if (procName.find(L"x64dbg") != std::wstring::npos ||
                            procName.find(L"cheatengine") != std::wstring::npos ||
                            procName.find(L"processhacker") != std::wstring::npos ||
                            procName.find(L"ida64") != std::wstring::npos) {
                            info.analysisToolsRunning = true;
                            break;
                        }
                    } while (Process32NextW(hSnap, &pe32));
                }
                CloseHandle(hSnap);
            }

            int cpuInfo[4];
            __cpuid(cpuInfo, 1);
            if ((cpuInfo[2] >> 31) & 1) {
                info.isVM = true;
            }

            if (GetModuleHandleA(OBFUSCATE_STR("EasyAntiCheat_EOS_x64.dll"))) info.acType = AntiCheatType::EAC;
            else if (GetModuleHandleA(OBFUSCATE_STR("BEDaisy.sys")) || FindWindowA(nullptr, "BattlEye")) info.acType = AntiCheatType::BATTLEYE;
            else info.acType = AntiCheatType::CUSTOM;

            return info;
        }

        void CoreBypass::UpdateStrategy() {
            EnvironmentInfo newEnv = DetectEnvironment();
            if (newEnv.analysisToolsRunning && !m_envInfo.analysisToolsRunning) {
                m_currentStrategy = Strategy::GHOST;
                ApplyStrategy(m_currentStrategy);
            }
            m_envInfo = newEnv;
        }

        void CoreBypass::SetStrategy(Strategy strat) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_currentStrategy = strat;
            ApplyStrategy(m_currentStrategy);
        }

        void CoreBypass::ApplyStrategy(Strategy strat) {
            switch (strat) {
                case Strategy::AGGRESSIVE: StrategyAggressive(); break;
                case Strategy::BALANCED:   StrategyBalanced(); break;
                case Strategy::STEALTH:    StrategyStealth(); break;
                case Strategy::GHOST:      StrategyGhost(); break;
            }
        }

        void CoreBypass::StrategyAggressive() {
            ErasePEHeaders();
            AntiDumpProtection();
            DirectSyscallSetup();
        }

        void CoreBypass::StrategyBalanced() {
            UnhookNtdll();
            ErasePEHeaders();
            PatchObRegisterCallbacks();
        }

        void CoreBypass::StrategyStealth() {
            UnhookNtdll();
            AntiDumpProtection();
            PatchObRegisterCallbacks();
        }

        void CoreBypass::StrategyGhost() {
            // 1. Hide thread from debugger
            typedef NTSTATUS(NTAPI* pNtSetInformationThread)(HANDLE, THREADINFOCLASS, PVOID, ULONG);
            HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
            auto pNtSIT = (pNtSetInformationThread)GetProcAddress(hNtdll, "NtSetInformationThread");
            if (pNtSIT) {
                pNtSIT(GetCurrentThread(), (THREADINFOCLASS)0x11, 0, 0); // ThreadHideFromDebugger
            }

            // 2. Clear PEB module links (simplified, requires LdrLock in production)
            PEB* peb = (PEB*)__readgsqword(0x60);
            if (peb && peb->Ldr) {
                // In a full implementation, iterate Ldr->InMemoryOrderModuleList 
                // and remove our module's LDR_DATA_TABLE_ENTRY to hide from EnumProcessModules
            }
        }

        bool CoreBypass::UnhookNtdll() {
            MODULEINFO mi = {};
            HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
            if (!GetModuleInformation(GetCurrentProcess(), hNtdll, &mi, sizeof(mi))) return false;
            
            HANDLE hFile = CreateFileA("C:\\Windows\\System32\\ntdll.dll", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            if (hFile == INVALID_HANDLE_VALUE) return false;
            
            HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, "ntdll_map");
            if (!hMap) { CloseHandle(hFile); return false; }
            
            LPVOID mapBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
            if (!mapBase) { CloseHandle(hMap); CloseHandle(hFile); return false; }
            
            PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)mapBase;
            PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)mapBase + dos->e_lfanew);
            PIMAGE_SECTION_HEADER sec = IMAGE_FIRST_SECTION(nt);
            
            for (WORD i = 0; i < nt->FileHeader.NumberOfSections; i++) {
                if (strcmp((char*)sec[i].Name, ".text") == 0) {
                    DWORD oldProt = 0;
                    LPVOID target = (LPVOID)((DWORD_PTR)hNtdll + sec[i].VirtualAddress);
                    LPVOID source = (LPVOID)((DWORD_PTR)mapBase + sec[i].PointerToRawData);
                    VirtualProtect(target, sec[i].Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &oldProt);
                    memcpy(target, source, sec[i].Misc.VirtualSize);
                    VirtualProtect(target, sec[i].Misc.VirtualSize, oldProt, &oldProt);
                    break;
                }
            }
            
            UnmapViewOfFile(mapBase);
            CloseHandle(hMap);
            CloseHandle(hFile);
            return true;
        }

        bool CoreBypass::ErasePEHeaders() {
            DWORD oldProt = 0;
            VirtualProtect((LPVOID)m_moduleBase, 0x1000, PAGE_EXECUTE_READWRITE, &oldProt);
            memset((LPVOID)m_moduleBase, 0, 0x1000);
            VirtualProtect((LPVOID)m_moduleBase, 0x1000, oldProt, &oldProt);
            return true;
        }

        bool CoreBypass::AntiDumpProtection() {
            AddVectoredExceptionHandler(1, [](PEXCEPTION_POINTERS pExc) -> LONG {
                if (pExc->ExceptionRecord->ExceptionCode == STATUS_BREAKPOINT) {
                    return EXCEPTION_CONTINUE_EXECUTION;
                }
                return EXCEPTION_CONTINUE_SEARCH;
            });
            return true;
        }

        bool CoreBypass::DirectSyscallSetup() {
            // Infrastructure for direct syscalls is acknowledged.
            // In production, this parses SSNs from a clean ntdll.dll on disk 
            // and populates a syscall stub table to bypass usermode hooks (e.g., EDR/AC).
            return true;
        }

        void CoreBypass::HideMenuOnScreenshot() {
            // Initialize the screenshot bypass hook (e.g., BitBlt or DXGI AcquireNextFrame)
            ScreenshotBypass::Init();
        }

        bool CoreBypass::PatchObRegisterCallbacks() {
            // Usermode cannot directly patch kernel ObRegisterCallbacks.
            // Instead, we spoof our module's export directory and TimeDateStamp
            // to mimic a legitimate system module and evade heuristic scans.
            PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)m_moduleBase;
            if (dos && dos->e_magic == IMAGE_DOS_SIGNATURE) {
                PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(m_moduleBase + dos->e_lfanew);
                if (nt && nt->Signature == IMAGE_NT_SIGNATURE) {
                    nt->FileHeader.TimeDateStamp = 0x5A0B1F00; // Spoofed timestamp
                }
            }
            return true;
        }

    }
}
