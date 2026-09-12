# VV.EXE Development Team Tools (2026)

## injector.bat
Automated script to compile vv.dll and inject it using Extreme Injector.
Usage: Run from the root directory. Ensure Visual Studio build tools are in PATH.

## check_strings.py
Scans the compiled binary for hardcoded strings that might trigger AV/AC heuristics.
Usage: `python check_strings.py bin/vv.dll`

## hash_util.py
Generates FNV-1a and compile-time XOR hashes for offsets and strings.
