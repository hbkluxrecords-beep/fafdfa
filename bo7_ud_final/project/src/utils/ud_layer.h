#pragma once
// ═══════════════════════════════════════════════════════════════════════════════
//  ud_layer.h  — Ricochet bypass layer (external cheat, no driver)
//
//  Addresses every detection listed in the UC TED TALK dump (Oct 2026):
//    1. Physical memory signature scan  → window class randomisation +
//                                         ImGui RenderFrameBorder patch
//    2. Page table / MBI consistency    → external = R/W only, never exec
//    3. Kernel thread start addresses   → not applicable (external process)
//    4. Kernel IAT checks               → not applicable (external process)
//    5. Hardware breakpoints (DR7)      → clear DRx before attach
//    6. VBS / Hypervisor check          → no action needed from external
//    7. Certificate checks              → rename exe to match a known-clean
//                                         process name on launch
//    8. Memory audit (private exec)     → no injected code, zero exec pages
// ═══════════════════════════════════════════════════════════════════════════════

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include <random>
#include <chrono>
#include <intrin.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

namespace ud {

// ─── 1. Random window class / title ──────────────────────────────────────────
// Ricochet scans process window titles and class names for known cheat strings.
// Generate a random alphanumeric class name and window title each launch.
inline std::string RandomAlphaNum(int len) {
    static const char cs[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::mt19937 rng(static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> d(0, (int)sizeof(cs) - 2);
    std::string s(len, ' ');
    for (auto& c : s) c = cs[d(rng)];
    return s;
}

inline std::string g_wnd_class;
inline std::string g_wnd_title;

inline void RandomiseWindowStrings() {
    g_wnd_class = RandomAlphaNum(12);
    g_wnd_title = RandomAlphaNum(16);
}

// ─── 2. Clear hardware debug registers ───────────────────────────────────────
// Ricochet continuously clears DR7 via kernel. From external userland we do
// the same for our own thread so there is no inconsistency on attach.
inline void ClearOwnDebugRegisters() {
    __try {
        __writecr0(0);  // will fault — catches VM-based debuggers
    } __except(EXCEPTION_EXECUTE_HANDLER) {}
    // Zero our own debug registers cleanly
    // __writedr is not available in userland — use SetThreadContext
    CONTEXT ctx{};
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
    HANDLE hThread = GetCurrentThread();
    if (GetThreadContext(hThread, &ctx)) {
        ctx.Dr0 = ctx.Dr1 = ctx.Dr2 = ctx.Dr3 = ctx.Dr6 = ctx.Dr7 = 0;
        SetThreadContext(hThread, &ctx);
    }
}

// ─── 3. Process name spoof ────────────────────────────────────────────────────
// Ricochet's cert checker runs SeLocateProcessImageName on processes it finds.
// We can't change the on-disk name but we can rename the window and avoid
// running under a suspicious name. Recommended: rename bo7_external.exe to
// something innocuous before launching (see build.bat).
// At runtime, patch the PEB ImagePathName to show a clean name.
inline void SpoofPebImageName(const wchar_t* fakeName) {
    // Access own PEB via TEB
    PPEB peb = reinterpret_cast<PPEB>(__readgsqword(0x60));
    if (!peb) return;

    // RTL_USER_PROCESS_PARAMETERS is at PEB+0x20
    auto* params = reinterpret_cast<PRTL_USER_PROCESS_PARAMETERS>(
        *reinterpret_cast<uintptr_t*>(
            reinterpret_cast<BYTE*>(peb) + 0x20));
    if (!params) return;

    // ImagePathName is a UNICODE_STRING — patch the Buffer pointer
    // to point to our fake name
    static wchar_t fakeBuf[MAX_PATH]{};
    wcscpy_s(fakeBuf, fakeName);

    UNICODE_STRING* imgPath = reinterpret_cast<UNICODE_STRING*>(
        reinterpret_cast<BYTE*>(params) + 0x60);
    if (!imgPath) return;

    DWORD old = 0;
    VirtualProtect(imgPath, sizeof(UNICODE_STRING), PAGE_READWRITE, &old);
    imgPath->Buffer = fakeBuf;
    imgPath->Length = static_cast<USHORT>(wcslen(fakeBuf) * sizeof(wchar_t));
    imgPath->MaximumLength = imgPath->Length + sizeof(wchar_t);
    VirtualProtect(imgPath, sizeof(UNICODE_STRING), old, &old);
}

// ─── 4. Clipboard API guard ───────────────────────────────────────────────────
// UC thread: GlobalAlloc/Clipboard APIs cause instant shadowban on internal.
// External doesn't use these but guard anyway.
inline bool g_clipboard_guard = false;  // set true to block clipboard calls

// ─── 5. Memory allocation discipline ─────────────────────────────────────────
// ALL allocations in this process must be MEM_PRIVATE + non-executable.
// Never call VirtualAlloc with PAGE_EXECUTE_* from outside cod.exe.
// The external only reads game memory — it never writes executable pages
// into its own or the game's address space.
// Verify: no exec pages in our process.
inline bool VerifyNoExecPages() {
    MEMORY_BASIC_INFORMATION mbi{};
    uintptr_t addr = 0;
    while (VirtualQuery(reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi))) {
        if (mbi.State == MEM_COMMIT &&
            (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ |
                            PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
            // Our own .text section will show PAGE_EXECUTE_READ — that's fine
            // because it's MEM_IMAGE backed. flag only MEM_PRIVATE exec.
            if (mbi.Type == MEM_PRIVATE)
                return false;  // unexpected private exec page
        }
        addr += mbi.RegionSize;
        if (addr < reinterpret_cast<uintptr_t>(mbi.BaseAddress))
            break;
    }
    return true;
}

// ─── 6. Anti-analysis: timing checks ─────────────────────────────────────────
// Detect sandbox / analysis environments that slow execution.
inline bool SandboxTimingCheck() {
    LARGE_INTEGER f, t1, t2;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t1);
    Sleep(10);
    QueryPerformanceCounter(&t2);
    double elapsed = static_cast<double>(t2.QuadPart - t1.QuadPart) /
                     static_cast<double>(f.QuadPart) * 1000.0;
    // In a sandbox Sleep(10) often returns in <1ms or >100ms
    return (elapsed >= 8.0 && elapsed <= 60.0);
}

// ─── 7. ETW patch (usermode telemetry) ────────────────────────────────────────
// Ricochet's usermode component reads ETW events. Silence the ETW write path
// in our own process so our activity doesn't appear in kernel telemetry.
inline void PatchEtwUsermode() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) return;
    void* fn = GetProcAddress(ntdll, "EtwEventWrite");
    if (!fn) return;
    DWORD old = 0;
    VirtualProtect(fn, 1, PAGE_EXECUTE_READWRITE, &old);
    *reinterpret_cast<BYTE*>(fn) = 0xC3; // ret
    VirtualProtect(fn, 1, old, &old);
    FlushInstructionCache(GetCurrentProcess(), fn, 1);
}

// ─── 8. AMSI disable ──────────────────────────────────────────────────────────
inline void PatchAmsi() {
    HMODULE amsi = LoadLibraryA("amsi.dll");
    if (!amsi) return;
    void* scan = GetProcAddress(amsi, "AmsiScanBuffer");
    if (!scan) return;
    // xor eax,eax; ret
    static const BYTE patch[] = { 0x33, 0xC0, 0xC3 };
    DWORD old = 0;
    VirtualProtect(scan, sizeof(patch), PAGE_EXECUTE_READWRITE, &old);
    memcpy(scan, patch, sizeof(patch));
    VirtualProtect(scan, sizeof(patch), old, &old);
    FlushInstructionCache(GetCurrentProcess(), scan, sizeof(patch));
}

// ─── 9. Master init ───────────────────────────────────────────────────────────
inline void Init() {
    // Sandbox check — bail early if in analysis environment
    if (!SandboxTimingCheck()) {
        ExitProcess(0);
    }

    // Silence telemetry
    PatchEtwUsermode();
    PatchAmsi();

    // Clear own debug registers
    ClearOwnDebugRegisters();

    // PEB name spoof — appear as a system tool
    SpoofPebImageName(L"C:\\Windows\\System32\\RuntimeBroker.exe");

    // Randomise window strings — used by overlay_setup.h
    RandomiseWindowStrings();
}

} // namespace ud
