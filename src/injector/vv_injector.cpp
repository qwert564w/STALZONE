// Copyright (c) 2026 STALZONE Team. All rights reserved.
// VV.EXE native injector.

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cwctype>

#pragma comment(lib, "advapi32.lib")

#ifdef VV_USE_MANUAL_MAP
#include "../core/manual_map.h"
#endif

namespace vv {

struct Options {
    std::wstring target;
    std::wstring dll;
    int method = 2;
    bool stealth = false;
};

bool IsNumeric(const std::wstring& s) {
    if (s.empty()) {
        return false;
    }

    return std::all_of(s.begin(), s.end(), [](wchar_t c) {
        return std::iswdigit(static_cast<wint_t>(c));
    });
}

bool EnableDebugPrivilege() {
    HANDLE token = nullptr;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
        return false;
    }

    LUID luid{};
    if (!LookupPrivilegeValueW(nullptr, SE_DEBUG_NAME, &luid)) {
        CloseHandle(token);
        return false;
    }

    TOKEN_PRIVILEGES privileges{};
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Luid = luid;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL ok = AdjustTokenPrivileges(
        token,
        FALSE,
        &privileges,
        sizeof(privileges),
        nullptr,
        nullptr
    );

    DWORD error = GetLastError();
    CloseHandle(token);

    return ok && error == ERROR_SUCCESS;
}

DWORD GetPidByName(const std::wstring& name) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);

    DWORD result = 0;

    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, name.c_str()) == 0) {
                result = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return result;
}

DWORD ResolveTarget(const std::wstring& target) {
    if (IsNumeric(target)) {
        return static_cast<DWORD>(_wtoi(target.c_str()));
    }

    DWORD pid = GetPidByName(target);
    if (pid != 0) {
        return pid;
    }

    std::wstring withExe = target + L".exe";
    return GetPidByName(withExe);
}

LPVOID WriteRemoteWideString(HANDLE process, const std::wstring& text) {
    SIZE_T size = (text.size() + 1) * sizeof(wchar_t);

    LPVOID memory = VirtualAllocEx(
        process,
        nullptr,
        size,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!memory) {
        return nullptr;
    }

    if (!WriteProcessMemory(process, memory, text.c_str(), size, nullptr)) {
        VirtualFreeEx(process, memory, 0, MEM_RELEASE);
        return nullptr;
    }

    return memory;
}

LPTHREAD_START_ROUTINE GetLoadLibraryWAddress() {
    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    if (!kernel32) {
        return nullptr;
    }

    return reinterpret_cast<LPTHREAD_START_ROUTINE>(
        GetProcAddress(kernel32, "LoadLibraryW")
    );
}

HANDLE OpenTargetProcess(DWORD pid) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (process) {
        return process;
    }

    process = OpenProcess(
        PROCESS_CREATE_THREAD |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE |
        PROCESS_VM_READ |
        PROCESS_QUERY_INFORMATION,
        FALSE,
        pid
    );

    return process;
}

bool InjectCreateRemoteThread(HANDLE process, LPVOID argument, LPTHREAD_START_ROUTINE routine) {
    HANDLE thread = CreateRemoteThread(
        process,
        nullptr,
        0,
        routine,
        argument,
        0,
        nullptr
    );

    if (!thread) {
        return false;
    }

    WaitForSingleObject(thread, 5000);

    DWORD exitCode = 0;
    GetExitCodeThread(thread, &exitCode);

    CloseHandle(thread);

    return exitCode != 0;
}

bool InjectNtCreateThreadEx(
    HANDLE process,
    LPVOID argument,
    LPTHREAD_START_ROUTINE routine,
    bool stealth
) {
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
    if (!ntdll) {
        return false;
    }

    using NtCreateThreadEx_t = NTSTATUS(NTAPI*)(
        PHANDLE,
        ACCESS_MASK,
        PVOID,
        HANDLE,
        LPTHREAD_START_ROUTINE,
        LPVOID,
        ULONG,
        ULONG_PTR,
        ULONG_PTR,
        ULONG_PTR,
        PVOID
    );

    auto NtCreateThreadEx = reinterpret_cast<NtCreateThreadEx_t>(
        GetProcAddress(ntdll, "NtCreateThreadEx")
    );

    if (!NtCreateThreadEx) {
        return false;
    }

    HANDLE thread = nullptr;

    ULONG flags = 0;

    if (stealth) {
        flags = 0x00000004;
    }

    NTSTATUS status = NtCreateThreadEx(
        &thread,
        THREAD_ALL_ACCESS,
        nullptr,
        process,
        routine,
        argument,
        flags,
        0,
        0,
        0,
        nullptr
    );

    if (status != 0 || !thread) {
        return false;
    }

    WaitForSingleObject(thread, 5000);

    DWORD exitCode = 0;
    GetExitCodeThread(thread, &exitCode);

    CloseHandle(thread);

    return exitCode != 0;
}

bool InjectApcQueue(DWORD pid, LPVOID argument, LPTHREAD_START_ROUTINE routine) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    THREADENTRY32 entry{};
    entry.dwSize = sizeof(entry);

    bool queued = false;

    if (Thread32First(snapshot, &entry)) {
        do {
            if (entry.th32OwnerProcessID != pid) {
                continue;
            }

            HANDLE thread = OpenThread(
                THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION,
                FALSE,
                entry.th32ThreadID
            );

            if (!thread) {
                continue;
            }

            if (QueueUserAPC(reinterpret_cast<PAPCFUNC>(routine), thread, reinterpret_cast<ULONG_PTR>(argument))) {
                ResumeThread(thread);
                queued = true;
            }

            CloseHandle(thread);

        } while (Thread32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return queued;
}

void PrintUsage() {
    std::wcout << L"[VV.EXE] injector usage:\n";
    std::wcout << L"  vv_injector.exe --pid <pid|process.exe> --dll <path.dll> --method <1|2|3|4> [--stealth]\n\n";
    std::wcout << L"Methods:\n";
    std::wcout << L"  1 - CreateRemoteThread\n";
    std::wcout << L"  2 - NtCreateThreadEx\n";
    std::wcout << L"  3 - Manual Map, if compiled with VV_USE_MANUAL_MAP\n";
    std::wcout << L"  4 - APC Queue\n";
}

} // namespace vv

int wmain(int argc, wchar_t* argv[]) {
    vv::Options options;

    for (int i = 1; i < argc; ++i) {
        std::wstring arg = argv[i];

        if (arg == L"--pid" && i + 1 < argc) {
            options.target = argv[++i];
        } else if (arg == L"--dll" && i + 1 < argc) {
            options.dll = argv[++i];
        } else if (arg == L"--method" && i + 1 < argc) {
            options.method = _wtoi(argv[++i]);
        } else if (arg == L"--stealth") {
            options.stealth = true;
        }
    }

    if (options.target.empty() || options.dll.empty()) {
        vv::PrintUsage();
        return 1;
    }

    vv::EnableDebugPrivilege();

    DWORD pid = vv::ResolveTarget(options.target);
    if (pid == 0) {
        std::wcerr << L"[-] Target process not found: " << options.target << L"\n";
        return 2;
    }

    std::wcout << L"[+] Target PID: " << pid << L"\n";
    std::wcout << L"[+] DLL: " << options.dll << L"\n";
    std::wcout << L"[+] Method: " << options.method << L"\n";

    HANDLE process = vv::OpenTargetProcess(pid);
    if (!process) {
        std::wcerr << L"[-] OpenProcess failed. Error: " << GetLastError() << L"\n";
        return 3;
    }

    bool success = false;

    if (options.method == 3) {
#ifdef VV_USE_MANUAL_MAP
        success = ::manual_map::inject(pid, options.dll);
#else
        std::wcerr << L"[-] Method 3 disabled.\n";
        std::wcerr << L"[-] Compile with /DVV_USE_MANUAL_MAP and include manual_map header.\n";
        CloseHandle(process);
        return 4;
#endif
    } else {
        LPVOID remoteDllPath = vv::WriteRemoteWideString(process, options.dll);
        if (!remoteDllPath) {
            std::wcerr << L"[-] Failed to write DLL path into target.\n";
            CloseHandle(process);
            return 5;
        }

        LPTHREAD_START_ROUTINE loadLibrary = vv::GetLoadLibraryWAddress();
        if (!loadLibrary) {
            std::wcerr << L"[-] Failed to resolve LoadLibraryW.\n";
            CloseHandle(process);
            return 6;
        }

        switch (options.method) {
            case 1:
                success = vv::InjectCreateRemoteThread(process, remoteDllPath, loadLibrary);
                break;

            case 2:
                success = vv::InjectNtCreateThreadEx(process, remoteDllPath, loadLibrary, options.stealth);
                break;

            case 4:
                success = vv::InjectApcQueue(pid, remoteDllPath, loadLibrary);
                break;

            default:
                std::wcerr << L"[-] Unknown method.\n";
                break;
        }
    }

    CloseHandle(process);

    if (success) {
        std::wcout << L"[+] Injection completed successfully.\n";
        return 0;
    }

    std::wcerr << L"[-] Injection failed.\n";
    return 7;
}
