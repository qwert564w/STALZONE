// COPYRIGHT VV.EXE Development Team 2026. All rights reserved.
#pragma once
#include <windows.h>
#include <winternl.h>
#include <string>
#include <vector>
#include <mutex>

namespace VV {
    namespace Bypass {
        enum class Strategy {
            AGGRESSIVE, 
            BALANCED,   
            STEALTH,    
            GHOST       
        };

        enum class AntiCheatType {
            NONE, EAC, BATTLEYE, VANGUARD, CUSTOM
        };

        struct EnvironmentInfo {
            bool isDebuggerPresent;
            bool isVM;
            bool analysisToolsRunning;
            AntiCheatType acType;
        };

        class CoreBypass {
        public:
            static CoreBypass& Get();
            void Initialize();
            void UpdateStrategy();
            Strategy GetCurrentStrategy() const { return m_currentStrategy; }
            void SetStrategy(Strategy strat);

        private:
            CoreBypass() = default;
            ~CoreBypass() = default;
            CoreBypass(const CoreBypass&) = delete;
            CoreBypass& operator=(const CoreBypass&) = delete;

            EnvironmentInfo DetectEnvironment();
            void ApplyStrategy(Strategy strat);

            void StrategyAggressive();
            void StrategyBalanced();
            void StrategyStealth();
            void StrategyGhost();

            bool UnhookNtdll();
            bool ErasePEHeaders();
            bool AntiDumpProtection();
            bool DirectSyscallSetup();
            void HideMenuOnScreenshot();
            bool PatchObRegisterCallbacks();

            Strategy m_currentStrategy = Strategy::BALANCED;
            EnvironmentInfo m_envInfo = {};
            uintptr_t m_moduleBase = 0;
            std::mutex m_mutex;
        };
    }
}
