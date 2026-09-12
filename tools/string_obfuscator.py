import sys, random
def obfuscate(s):
    key = random.randint(1, 255)
    enc = ''.join([chr(ord(c) ^ key) for c in s])
    return f"OBF(\"{enc}\", {key})"
if __name__ == "__main__":
    print(obfuscate(sys.argv[1]))
