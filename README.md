# VV.EXE - STALZONE Cheat Client

**COPYRIGHT VV.EXE Development Team 2026. All rights reserved.**

## Overview

Advanced IL2CPP cheat client for STALZONE with comprehensive bypass capabilities.

## Features

### Combat Modules
- Aimbot with prediction and humanization
- Silent Aim - override shoot angles
- Triggerbot with random delay
- No Recoil with randomization
- No Spread reduction
- FOV Circle indicator

### Visual Modules
- Player ESP (boxes, names, health, bones)
- Loot ESP with rarity colors
- Corpse ESP with loot list
- Chams with visible-only mode
- Artifact Glow with rarity effects
- Anomaly ESP with pulsing

### Bypass Features
- Multi-method UAC Bypass
- Manual Map Injection
- Screenshot Bypass
- String Obfuscation
- Offset Encryption

## Requirements

- Windows 10/11 x64
- Visual Studio 2022
- CMake 3.20+
- Python 3.8+
- Il2CppDumper

## Installation

1. Clone repository
2. Run tools/build.bat
3. Update offsets with python tools/dump_offsets.py

## Usage

### Method 1: Using Loader
1. Run loader/loader.exe
2. Select option 1
3. Wait for injection

### Method 2: Manual Launch
1. Run tools/start_game.bat
2. Select option 2 (UAC BYPASS)
3. Select option 3 (Inject)

### In-Game Menu
- Right Mouse Button - Toggle menu
- ESC - Close menu

## Server-Side Validation Warnings

**Important:** Some features may be detected:
- Silent Aim - if shoot angles validated server-side
- No Recoil - if recoil patterns checked
- Triggerbot - if reaction times too consistent
- ESP Features - generally safe (client-side only)

**Recommendation:** Use conservatively with randomization.

## Contact

vv.exe.development@protonmail.com

---

*VV.EXE Development Team 2026 - All rights reserved*