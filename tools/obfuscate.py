// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#!/usr/bin/env python3
import sys
import os
import random
import re
from pathlib import Path

def generate_random_key():
    return random.randint(1, 255)

def xor_encrypt_string(s, key):
    encrypted = []
    for c in s:
        encrypted.append(ord(c) ^ key)
    return encrypted

def generate_cpp_array(name, data, key):
    lines = []
    lines.append(f"// XOR key: 0x{key:02X}")
    lines.append(f"constexpr unsigned char {name}[] = {{")
    
    # Разбиваем на строки по 16 байт
    for i in range(0, len(data), 16):
        chunk = data[i:i+16]
        hex_str = ", ".join([f"0x{b:02X}" for b in chunk])
        if i + 16 < len(data):
            lines.append(f"    {hex_str},")
        else:
            lines.append(f"    {hex_str}")
    
    lines.append("};")
    lines.append(f"constexpr size_t {name}_len = {len(data)};")
    return "\n".join(lines)

def generate_compile_time_xor():
    lines = []
    lines.append("namespace Obfuscation {")
    lines.append("    template<size_t N, unsigned char Key>")
    lines.append("    struct XORDecryptor {")
    lines.append("        static constexpr std::array<char, N> decrypt(const unsigned char* data) {")
    lines.append("            std::array<char, N> result{};")
    lines.append("            for (size_t i = 0; i < N; ++i) {")
    lines.append("                result[i] = static_cast<char>(data[i] ^ Key);")
    lines.append("            }")
    lines.append("            return result;")
    lines.append("        }")
    lines.append("    };")
    lines.append("}")
    return "\n".join(lines)

def obfuscate_offsets(input_file, output_file):
    print(f"[OBF] Обфускация офсетов: {input_file}")
    
    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Находим все #define OFFSET_NAME 0x...
    pattern = r'#define\s+(\w+)\s+(0x[0-9A-Fa-f]+)'
    matches = re.findall(pattern, content)
    
    if not matches:
        print(f"[OBF] Офсеты не найдены в {input_file}")
        return
    
    key = generate_random_key()
    print(f"[OBF] Ключ шифрования: 0x{key:02X}")
    
    lines = ["// COPYRIGHT VV.EXE Development Team 2026. All rights reserved."]
    lines.append("#pragma once")
    lines.append("#include <array>")
    lines.append("#include <cstdint>")
    lines.append("")
    lines.append(generate_compile_time_xor())
    lines.append("")
    
    for name, value in matches:
        # Преобразуем offset в строку
        offset_str = value
        encrypted = xor_encrypt_string(offset_str, key)
        
        array_name = f"g_{name}_encrypted"
        lines.append(generate_cpp_array(array_name, encrypted, key))
        lines.append(f"constexpr uint64_t {name} = []() {{")
        lines.append(f"    constexpr auto decrypted = Obfuscation::XORDecryptor<{len(encrypted)}, 0x{key:02X}>::decrypt({array_name});")
        lines.append(f"    uint64_t result = 0;")
        lines.append(f"    for (size_t i = 0; i < {len(encrypted)}; ++i) {{")
        lines.append(f"        if (decrypted[i] == 'x') continue;")
        lines.append(f"        result <<= 4;")
        lines.append(f"        char c = decrypted[i];")
        lines.append(f"        if (c >= '0' && c <= '9') result |= (c - '0');")
        lines.append(f"        else if (c >= 'a' && c <= 'f') result |= (c - 'a' + 10);")
        lines.append(f"        else if (c >= 'A' && c <= 'F') result |= (c - 'A' + 10);")
        lines.append(f"    }}")
        lines.append(f"    return result;")
        lines.append(f"}}();")
        lines.append("")
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("\n".join(lines))
    
    print(f"[OBF] ✓ Создан: {output_file}")

def obfuscate_strings(input_file, output_file):
    print(f"[OBF] Обфускация строк: {input_file}")
    
    with open(input_file, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Находим все строковые литералы "..."
    pattern = r'"([^"\\]*(?:\\.[^"\\]*)*)"'
    matches = re.findall(pattern, content)
    
    if not matches:
        print(f"[OBF] Строки не найдены в {input_file}")
        return
    
    key = generate_random_key()
    print(f"[OBF] Ключ шифрования: 0x{key:02X}")
    
    lines = ["// COPYRIGHT VV.EXE Development Team 2026. All rights reserved."]
    lines.append("#pragma once")
    lines.append("#include <array>")
    lines.append("#include <string>")
    lines.append("")
    lines.append(generate_compile_time_xor())
    lines.append("")
    
    string_map = {}
    for i, s in enumerate(set(matches)):
        encrypted = xor_encrypt_string(s, key)
        array_name = f"g_str_{i:03d}_encrypted"
        
        lines.append(generate_cpp_array(array_name, encrypted, key))
        lines.append(f"inline std::string GetString_{i:03d}() {{")
        lines.append(f"    constexpr auto decrypted = Obfuscation::XORDecryptor<{len(encrypted)}, 0x{key:02X}>::decrypt({array_name});")
        lines.append(f"    return std::string(decrypted.data(), {len(encrypted)});")
        lines.append(f"}}")
        lines.append("")
        
        string_map[s] = i
    
    # Заменяем строки в оригинальном файле
    def replace_string(match):
        original = match.group(1)
        idx = string_map.get(original)
        if idx is not None:
            return f"GetString_{idx:03d}().c_str()"
        return match.group(0)
    
    new_content = re.sub(pattern, replace_string, content)
    
    # Сохраняем заголовочный файл с обфусцированными строками
    header_file = output_file.replace('.cpp', '_strings.h')
    with open(header_file, 'w', encoding='utf-8') as f:
        f.write("\n".join(lines))
    
    # Сохраняем модифицированный cpp файл
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(f'#include "{os.path.basename(header_file)}"\n')
        f.write(new_content)
    
    print(f"[OBF] ✓ Создан: {header_file}")
    print(f"[OBF] ✓ Обновлён: {output_file}")

def main():
    if len(sys.argv) < 3:
        print("Использование:")
        print("  python obfuscate.py offsets <input.h> [output.h]")
        print("  python obfuscate.py strings <input.cpp> [output.cpp]")
        sys.exit(1)
    
    mode = sys.argv[1]
    input_file = sys.argv[2]
    
    if mode == "offsets":
        output_file = sys.argv[3] if len(sys.argv) > 3 else input_file.replace('.h', '_obfuscated.h')
        obfuscate_offsets(input_file, output_file)
    elif mode == "strings":
        output_file = sys.argv[3] if len(sys.argv) > 3 else input_file.replace('.cpp', '_obfuscated.cpp')
        obfuscate_strings(input_file, output_file)
    else:
        print(f"Неизвестный режим: {mode}")
        sys.exit(1)

if __name__ == "__main__":
    main()
