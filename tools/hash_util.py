import sys
def fnv1a(s):
    h = 0xcbf29ce484222325
    for c in s:
        h ^= ord(c)
        h = (h * 0x100000001b3) & 0xffffffffffffffff
    return hex(h)
if __name__ == "__main__":
    print(fnv1a(sys.argv[1]))
