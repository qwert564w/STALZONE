#include "manual_map.h"
#include <fstream>
#include <vector>
bool ManualMap::MapPE(const char* dllPath, HMODULE* outModule) {
    std::ifstream file(dllPath, std::ios::binary);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), {});
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)buffer.data();
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(buffer.data() + dos->e_lfanew);
    LPVOID alloc = VirtualAlloc(NULL, nt->OptionalHeader.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    memcpy(alloc, buffer.data(), nt->OptionalHeader.SizeOfHeaders);
    IMAGE_SECTION_HEADER* sec = IMAGE_FIRST_SECTION(nt);
    for (int i = 0; i < nt->FileHeader.NumberOfSections; i++) {
        memcpy((LPBYTE)alloc + sec[i].VirtualAddress, buffer.data() + sec[i].PointerToRawData, sec[i].SizeOfRawData);
    }
    *outModule = (HMODULE)alloc;
    return true;
}
