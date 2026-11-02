// BO7 External — UD main.cpp
// Launch order: spoofer → BO7 → this exe (as admin)
#include "framework/overlay/overlay_setup.h"
#include "utils/kernel/driver.h"
#include "utils/ud_layer.h"
#include "world/engine/engine.h"
#include "world/thread/thread.h"
#include <iostream>

static bool attach_to_game() {
    const wchar_t* targets[] = {
        L"BlackOps7.exe", L"cod.exe", L"BlackOps6.exe", nullptr
    };
    std::cout << "[*] Waiting for BO7";
    for (int tries = 0; tries < 120; ++tries) {
        for (int c = 0; targets[c]; ++c) {
            DWORD pid = request->get_process_pid(targets[c]);
            if (!pid) continue;
            request->m_pid = (int32_t)pid;
            if (!request->attach(pid)) continue;
            request->image_base = request->get_image_base(nullptr);
            if (!request->image_base) continue;
            request->peb = request->get_peb();
            std::cout << "\n[+] Attached  PID:" << pid
                      << "  base:0x" << std::hex << request->image_base << "\n";
            return true;
        }
        std::cout << "." << std::flush;
        Sleep(1000);
    }
    std::cout << "\n[!] Not found\n";
    return false;
}

int main() {
    // ── UD init first — before any window or DX11 ──
    ud::Init();

    // hide console window (no visible process artefact)
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    if (!attach_to_game()) {
        AllocConsole();
        ShowWindow(GetConsoleWindow(), SW_SHOW);
        std::cout << "[!] Failed to attach. Run as admin with game open.\n";
        std::cin.get();
        return 1;
    }

    start_optimized_cache_threads();

    // overlay — blocks until closed (DELETE key = clean exit)
    imguisetup::Overlay();
    imguisetup::SetupImgui();
    imguisetup::GuiLoop();
    return 0;
}
