import sys
import pefile

def check(path):
    try:
        pe = pefile.PE(path)
    except:
        print("Failed to parse PE")
        return
        
    strings = []
    for section in pe.sections:
        strings += section.get_data().decode('ascii', 'ignore').split('\x00')
        
    bad_words = ['cheat', 'hack', 'inject', 'bypass', 'vv.exe', 'github', 'extreme']
    found = False
    for s in strings:
        for bad in bad_words:
            if bad in s.lower() and len(s) > 3:
                print(f"[!] Suspicious string found: {s}")
                found = True
    if not found:
        print("[+] Binary looks clean!")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python check_strings.py <binary>")
    else:
        check(sys.argv[1])
