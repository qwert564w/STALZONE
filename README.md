# VV.EXE - STALZONE Cheat Client

**COPYRIGHT VV.EXE Development Team 2026. All rights reserved.**

## QUICK START (NO ADMIN REQUIRED)

### Step 1: Auto Setup
Run `auto_setup.bat` - it will automatically:
- Download CMake (portable)
- Download MinHook
- Download ImGui
- Download Il2CppDumper
- Compile vv.dll

### Step 2: Launch Game with UAC Bypass
Run `bypass_and_run.bat` - it will:
- Launch game WITHOUT UAC prompt
- No admin password needed
- Uses computerdefaults.exe method

### Step 3: Inject with Extreme Injector v3
1. Wait for game to reach main menu
2. Open Extreme Injector v3 (NO ADMIN NEEDED)
3. Select game process (STALKER.exe or similar)
4. Add `output\vv.dll`
5. Click INJECT

### Step 4: Use Cheat
- **Right Shift** - Toggle menu
- **Insert** - Toggle menu (backup)
- **ESC** - Close menu

## ENGINE SUPPORT

This cheat supports:
- X-Ray Engine (S.T.A.L.K.E.R. series)
- Unreal Engine 4/5
- Unity (IL2CPP)

The code automatically detects engine type and uses appropriate hooks.

## ANTI-CHEAT BYPASS

For EXBO / EAC / BattlEye:
- Use Manual Map injection in Extreme Injector
- Enable "Stealth Inject" option
- Wait 10 seconds after game start before injecting

## TROUBLESHOOTING

**Menu doesn't open:**
- Press Right Shift or Insert
- Check if DLL injected successfully

**Game crashes on inject:**
- Disable Steam Overlay
- Use Manual Map injection
- Inject after reaching main menu

**UAC bypass not working:**
- Try running bypass_and_run.bat as admin manually
- Some Windows builds block computerdefaults method

## FILE STRUCTURE

STALZONE/
- auto_setup.bat       (Auto download and compile)
- bypass_and_run.bat   (Launch game without UAC)
- output/vv.dll        (Compiled cheat)
- src/                 (Source code)
- libs/                (Downloaded libraries)
- tools/               (Build tools)

## CONTACT

vv.exe.development@protonmail.com

---

*VV.EXE Development Team 2026 - All rights reserved*
