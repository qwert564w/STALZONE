// Copyright (c) 2026 STALZONE Team. All rights reserved.
// Go orchestrator for VV.EXE backup injector.

package main

import (
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"unsafe"

	"golang.org/x/sys/windows"
)

type ProcessInfo struct {
	PID  uint32
	Name string
}

func listProcesses() ([]ProcessInfo, error) {
	snapshot, err := windows.CreateToolhelp32Snapshot(windows.TH32CS_SNAPPROCESS, 0)
	if err != nil {
		return nil, err
	}
	defer windows.CloseHandle(snapshot)

	var entry windows.ProcessEntry32
	entry.Size = uint32(unsafe.Sizeof(entry))

	err = windows.Process32First(snapshot, &entry)
	if err != nil {
		return nil, err
	}

	var result []ProcessInfo

	for err == nil {
		name := windows.UTF16ToString(entry.ExeFile[:])
		if name != "" {
			result = append(result, ProcessInfo{
				PID:  entry.ProcessID,
				Name: name,
			})
		}

		err = windows.Process32Next(snapshot, &entry)
	}

	return result, nil
}

func printProcessList() error {
	processes, err := listProcesses()
	if err != nil {
		return err
	}

	for i, process := range processes {
		fmt.Printf("%d|%d|%s\n", i+1, process.PID, process.Name)
	}

	return nil
}

func resolvePID(target string) (uint32, error) {
	if pid, err := strconv.Atoi(target); err == nil && pid > 0 {
		return uint32(pid), nil
	}

	processes, err := listProcesses()
	if err != nil {
		return 0, err
	}

	targetLower := strings.ToLower(target)
	targetNoExe := strings.TrimSuffix(targetLower, ".exe")

	for _, process := range processes {
		nameLower := strings.ToLower(process.Name)

		if nameLower == targetLower || nameLower == targetNoExe || strings.Contains(nameLower, targetNoExe) {
			return process.PID, nil
		}
	}

	return 0, fmt.Errorf("process not found: %s", target)
}

func findInjectorPath() string {
	candidates := []string{
		filepath.Join("output", "vv_injector.exe"),
		filepath.Join("bin", "vv_injector.exe"),
		filepath.Join("loader", "go_injector", "vv_injector.exe"),
		"vv_injector.exe",
	}

	exe, err := os.Executable()
	if err == nil {
		dir := filepath.Dir(exe)

		candidates = append(candidates,
			filepath.Join(dir, "vv_injector.exe"),
			filepath.Join(dir, "..", "..", "output", "vv_injector.exe"),
			filepath.Join(dir, "..", "..", "bin", "vv_injector.exe"),
		)
	}

	for _, candidate := range candidates {
		if _, err := os.Stat(candidate); err == nil {
			abs, err := filepath.Abs(candidate)
			if err == nil {
				return abs
			}
			return candidate
		}
	}

	return ""
}

func runInjector(pid uint32, dll string, method int, stealth bool) error {
	injector := findInjectorPath()
	if injector == "" {
		return fmt.Errorf("vv_injector.exe not found")
	}

	args := fmt.Sprintf(`--pid %d --dll "%s" --method %d`, pid, dll, method)
	if stealth {
		args += " --stealth"
	}

	commandLine := fmt.Sprintf(`"%s" %s`, injector, args)

	appPtr, err := windows.UTF16PtrFromString(injector)
	if err != nil {
		return err
	}

	cmdPtr, err := windows.UTF16PtrFromString(commandLine)
	if err != nil {
		return err
	}

	si := &windows.StartupInfo{
		Cb:          uint32(unsafe.Sizeof(windows.StartupInfo{})),
		Flags:       windows.STARTF_USESHOWWINDOW,
		ShowWindow:  windows.SW_HIDE,
	}

	var pi windows.ProcessInformation

	err = windows.CreateProcess(
		appPtr,
		cmdPtr,
		nil,
		nil,
		false,
		windows.CREATE_NO_WINDOW,
		nil,
		nil,
		si,
		&pi,
	)

	if err != nil {
		return err
	}

	windows.CloseHandle(pi.Process)
	windows.CloseHandle(pi.Thread)

	return nil
}

func main() {
	list := flag.Bool("list", false, "print process list: index|pid|name")
	pid := flag.String("pid", "", "process id or process name")
	dll := flag.String("dll", "", "path to DLL")
	method := flag.Int("method", 2, "injection method: 1 CreateRemoteThread, 2 NtCreateThreadEx, 3 ManualMap, 4 APC")
	stealth := flag.Bool("stealth", false, "use stealth flags for injector")

	flag.Parse()

	if *list {
		if err := printProcessList(); err != nil {
			fmt.Fprintf(os.Stderr, "[-] %v\n", err)
			os.Exit(1)
		}
		return
	}

	if *pid == "" || *dll == "" {
		fmt.Println("[VV.EXE] Go orchestrator")
		fmt.Println("Usage:")
		fmt.Println("  go_injector.exe --list")
		fmt.Println("  go_injector.exe --pid 1234 --dll payload.dll --method 2 --stealth")
		fmt.Println("  go_injector.exe --pid game.exe --dll payload.dll --method 2 --stealth")
		return
	}

	targetPID, err := resolvePID(*pid)
	if err != nil {
		fmt.Fprintf(os.Stderr, "[-] %v\n", err)
		os.Exit(2)
	}

	err = runInjector(targetPID, *dll, *method, *stealth)
	if err != nil {
		fmt.Fprintf(os.Stderr, "[-] %v\n", err)
		os.Exit(3)
	}

	fmt.Println("[+] Injector started hidden.")
}
