#include "manual_map.h"
#include <fstream>
#include <vector>

typedef struct BASE_RELOCATION_BLOCK {
    DWORD PageAddress;
    DWORD BlockSize;
} BASE_RELOCATION_BLOCK, * PBASE_RELOCATION_BLOCK;

typedef struct BASE_RELOCATION_ENTRY {
    USHORT Offset : 12;
    USHORT Type : 4;
} BASE_RELOCATION_ENTRY, * PBASE_RELOCATION_ENTRY;

bool ManualMap::MapPE(const char* dllPath, HMODULE* outModule) {
    std::ifstream file(dllPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) return false;

    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)buffer.data();
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(buffer.data() + dos->e_lfanew);

    LPVOID alloc = VirtualAlloc(NULL, nt->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!alloc) return false;

    // Copy headers
    memcpy(alloc, buffer.data(), nt->OptionalHeader.SizeOfHeaders);

    // Copy sections
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        if (sec[i].SizeOfRawData > 0) {
            memcpy((LPBYTE)alloc + sec[i].VirtualAddress, buffer.data() + sec[i].PointerToRawData, sec[i].SizeOfRawData);
        }
    }

    // Process Relocations
    IMAGE_DATA_DIRECTORY* relocDir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    if (relocDir->Size > 0) {
        DWORD delta = (DWORD)((LPBYTE)alloc - nt->OptionalHeader.ImageBase);
        if (delta != 0) {
            DWORD currentLocation = relocDir->VirtualAddress;
            while (currentLocation < relocDir->VirtualAddress + relocDir->Size) {
                PBASE_RELOCATION_BLOCK block = (PBASE_RELOCATION_BLOCK)((LPBYTE)alloc + currentLocation);
                if (block->BlockSize == 0) break;
                
                PBASE_RELOCATION_ENTRY entries = (PBASE_RELOCATION_ENTRY)(block + 1);
                DWORD entriesCount = (block->BlockSize - sizeof(BASE_RELOCATION_BLOCK)) / sizeof(BASE_RELOCATION_ENTRY);
                
                for (DWORD i = 0; i < entriesCount; i++) {
                    if (entries[i].Type == IMAGE_REL_BASED_DIR64) {
                        ULONGLONG* patchAddress = (ULONGLONG*)((LPBYTE)alloc + block->PageAddress + entries[i].Offset);
                        *patchAddress += delta;
                    }
                }
                currentLocation += block->BlockSize;
            }
        }
    }

    // Resolve Imports
    IMAGE_DATA_DIRECTORY* importDir = &nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (importDir->Size > 0) {
        IMAGE_IMPORT_DESCRIPTOR* importDesc = (IMAGE_IMPORT_DESCRIPTOR*)((LPBYTE)alloc + importDir->VirtualAddress);
        while (importDesc->Name != 0) {
            const char* moduleName = (const char*)((LPBYTE)alloc + importDesc->Name);
            HMODULE hModule = LoadLibraryA(moduleName);
            if (!hModule) return false;

            IMAGE_THUNK_DATA* thunk = (IMAGE_THUNK_DATA*)((LPBYTE)alloc + importDesc->FirstThunk);
            IMAGE_THUNK_DATA* originalThunk = (IMAGE_THUNK_DATA*)((LPBYTE)alloc + (importDesc->OriginalFirstThunk ? importDesc->OriginalFirstThunk : importDesc->FirstThunk));

            while (originalThunk->u1.AddressOfData != 0) {
                if (IMAGE_SNAP_BY_ORDINAL(originalThunk->u1.Ordinal)) {
                    thunk->u1.Function = (ULONGLONG)GetProcAddress(hModule, (LPCSTR)IMAGE_ORDINAL(originalThunk->u1.Ordinal));
                } else {
                    PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((LPBYTE)alloc + originalThunk->u1.AddressOfData);
                    thunk->u1.Function = (ULONGLONG)GetProcAddress(hModule, (LPCSTR)importByName->Name);
                }
                thunk++;
                originalThunk++;
            }
            importDesc++;
        }
    }

    // Set memory protections
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        DWORD protect = PAGE_NOACCESS;
        if (sec[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) {
            protect = (sec[i].Characteristics & IMAGE_SCN_MEM_WRITE) ? PAGE_EXECUTE_READWRITE : PAGE_EXECUTE_READ;
        } else if (sec[i].Characteristics & IMAGE_SCN_MEM_WRITE) {
            protect = PAGE_READWRITE;
        } else {
            protect = PAGE_READONLY;
        }
        DWORD oldProtect;
        VirtualProtect((LPBYTE)alloc + sec[i].VirtualAddress, sec[i].Misc.VirtualSize, protect, &oldProtect);
    }

    // Call DllMain
    typedef BOOL(WINAPI* DllMain_t)(HMODULE, DWORD, LPVOID);
    DllMain_t DllMain = (DllMain_t)((LPBYTE)alloc + nt->OptionalHeader.AddressOfEntryPoint);
    if (DllMain) {
        DllMain((HMODULE)alloc, DLL_PROCESS_ATTACH, nullptr);
    }

    *outModule = (HMODULE)alloc;
    return true;
}
