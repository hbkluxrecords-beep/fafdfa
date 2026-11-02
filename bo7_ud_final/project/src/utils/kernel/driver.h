#pragma once

// ─────────────────────────────────────────────────────────────────────────────
//  driver.h  — USERLAND REPLACEMENT
//
//  The original UC base expects a kernel driver at
//  \\.\{3c7c414a-62fe-41e3-a33a-a3c164678a8a}
//
//  This file replaces the entire driver interface with a plain
//  ReadProcessMemory / WriteProcessMemory backend.
//  Everything compiles and runs without any driver or IOCTL.
//
//  The public API (request->read<T>, request->write<T>,
//  request->image_base, request->peb) is identical — nothing
//  else in the project needs to change.
// ─────────────────────────────────────────────────────────────────────────────

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <string>
#include <iostream>

#ifndef DRIVER_HPP
#define DRIVER_HPP

typedef unsigned __int64 QWORD;

namespace driver
{
    class communicate_t
    {
    private:
        HANDLE m_proc = INVALID_HANDLE_VALUE;

        // ── Internal helpers ──────────────────────────────────
        static DWORD find_pid(const std::wstring& name) {
            DWORD pid = 0;
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snap == INVALID_HANDLE_VALUE) return 0;
            PROCESSENTRY32W pe{}; pe.dwSize = sizeof(pe);
            if (Process32FirstW(snap, &pe)) {
                do {
                    if (name == pe.szExeFile) { pid = pe.th32ProcessID; break; }
                } while (Process32NextW(snap, &pe));
            }
            CloseHandle(snap);
            return pid;
        }

        static uintptr_t find_module_base(DWORD pid, const char* mod) {
            uintptr_t base = 0;
            HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
            if (snap == INVALID_HANDLE_VALUE) return 0;
            MODULEENTRY32 me{}; me.dwSize = sizeof(me);
            if (Module32First(snap, &me)) {
                do {
                    if (!mod || _stricmp(me.szModule, mod) == 0) {
                        base = reinterpret_cast<uintptr_t>(me.modBaseAddr);
                        break;
                    }
                } while (Module32Next(snap, &me));
            }
            CloseHandle(snap);
            return base;
        }

        static uintptr_t get_remote_peb(HANDLE hProc, DWORD pid) {
            // NtQueryInformationProcess path
            typedef NTSTATUS(NTAPI* fn_t)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
            static auto NtQIP = reinterpret_cast<fn_t>(
                GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess"));
            if (!NtQIP) return 0;

            PROCESS_BASIC_INFORMATION pbi{};
            ULONG ret = 0;
            if (NtQIP(hProc, ProcessBasicInformation, &pbi, sizeof(pbi), &ret) < 0)
                return 0;
            return reinterpret_cast<uintptr_t>(pbi.PebBaseAddress);
        }

    public:
        // Fields the rest of the codebase reads directly
        std::int32_t  m_pid        = 0;
        std::uintptr_t image_base  = 0;
        std::uintptr_t dtb         = 0;   // unused in userland but kept for ABI compat
        std::uintptr_t peb         = 0;

        // ── Lifecycle ─────────────────────────────────────────
        bool initialize_handle() {
            // called by main — just return true, no driver needed
            return true;
        }

        const std::uint32_t get_process_pid(const std::wstring& name) {
            return find_pid(name);
        }

        bool attach(int pid) {
            if (m_proc && m_proc != INVALID_HANDLE_VALUE)
                CloseHandle(m_proc);

            m_pid = pid;
            m_proc = OpenProcess(
                PROCESS_VM_READ | PROCESS_VM_WRITE |
                PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION,
                FALSE, static_cast<DWORD>(pid));

            if (!m_proc || m_proc == INVALID_HANDLE_VALUE) {
                std::cerr << "[!] OpenProcess failed: " << GetLastError() << "\n";
                return false;
            }
            return true;
        }

        const std::uintptr_t get_image_base(const char* mod_name) {
            uintptr_t base = find_module_base(static_cast<DWORD>(m_pid), mod_name);
            image_base = base;
            return base;
        }

        uintptr_t get_peb() {
            peb = get_remote_peb(m_proc, static_cast<DWORD>(m_pid));
            return peb;
        }

        // dtb / cr3 — not needed userland, stub returns true
        bool get_cr3(uintptr_t) { return true; }
        const uintptr_t get_dtb(uint32_t) { return 0; }
        const uintptr_t translate_address(uintptr_t, uintptr_t) { return 0; }
        bool virtual_protect(const uintptr_t, const size_t, const DWORD) { return true; }
        NTSTATUS protect_sprite_content_ex(HWND, uint32_t) { return 0; }

        // ── Memory IO ─────────────────────────────────────────
        bool read_virtual(const uintptr_t addr, void* buf, const size_t sz) {
            if (!m_proc || m_proc == INVALID_HANDLE_VALUE) return false;
            SIZE_T rd = 0;
            return ReadProcessMemory(m_proc,
                reinterpret_cast<LPCVOID>(addr), buf, sz, &rd) && rd > 0;
        }

        bool write_virtual(const uintptr_t addr, void* buf, const size_t sz) {
            if (!m_proc || m_proc == INVALID_HANDLE_VALUE) return false;
            SIZE_T wr = 0;
            return WriteProcessMemory(m_proc,
                reinterpret_cast<LPVOID>(addr), buf, sz, &wr) && wr > 0;
        }

        bool read_kernel(const uintptr_t addr, void* buf, const size_t sz, const uint32_t) {
            return read_virtual(addr, buf, sz);
        }

        // ── Templated helpers (original interface) ────────────
        template <typename T>
        T read(const uintptr_t addr) {
            T val{};
            read_virtual(addr, &val, sizeof(T));
            return val;
        }

        template <typename T>
        bool write(const uintptr_t addr, T val) {
            return write_virtual(addr, &val, sizeof(T));
        }

        template <typename T>
        bool read_array(const uintptr_t addr, T* buf, size_t sz) {
            return read_virtual(addr, buf, sz);
        }
    };
}

// Global singleton — same name as original so all other files compile unchanged
static driver::communicate_t* request = new driver::communicate_t();

#endif // DRIVER_HPP
