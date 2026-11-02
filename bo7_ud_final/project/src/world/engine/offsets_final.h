#pragma once
#include <cstdint>
#include "../../utils/kernel/driver.h"
#include "../../utils/math/vectors.h"

namespace offsets {
    inline uint64_t ref_def_ptr = 0xE22D720;
    inline uint64_t name_array = 0xE3A0CE8;
    inline uint64_t name_array_pos = 0x3038;
    inline uint64_t name_size = 0xC0;
    inline uint64_t camera_base = 0xDE6FD70;
    inline uint64_t camera_pos = 0x1E4;
    inline uint64_t local_index = 0x15DCD0;
    inline uint64_t local_index_pos = 0x3b4;
    inline uint64_t game_mode = 0xD1DBD48;
    inline uint64_t weapon_definitions = 0xE082990;
    inline uint64_t distribute = 0xAA41F40;
    inline uint32_t visible_bit = 0x1B8F8C;
    inline uint64_t scoreboard = 0x131D8;
    inline uint64_t scoreboard_size = 0x80;
    namespace player {
        inline uint64_t size = 0x2510;
        inline uint64_t stance = 0x924;
        inline uint64_t valid = 0x20E0;
        inline uint64_t pos = 0x1530;
        inline uint64_t team = 0x1538;
        inline uint64_t weapon_index = 0x2368;
    }
    namespace bone {
        inline uint64_t bone_base = 0x15DD80;
        inline uint64_t size = 0x1D0;
        inline uint64_t offset = 0xE0;
    }
}


inline uintptr_t decrypt_client_info()
{
    const uint64_t mb = request->image_base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    r8 = request->read<uintptr_t>(request->image_base + 0xE221788);
    if (!r8)
        return r8;
    rdx = ~request->peb;
    rax = r8;
    rax >>= 0x18;
    r8 ^= rax;
    rax = r8;
    rax >>= 0x30;
    rcx = 0;
    r8 ^= rax;
    rcx = _rotl64(rcx, 0x10);
    rcx ^= request->read<uintptr_t>(request->image_base + 0xCC85A79);
    rax = 0x233F5F4AE79533B1;
    r8 *= rax;
    rax = 0x4FF2ED27F19D575D;
    rcx = ~rcx;
    r8 -= rdx;
    r8 += rax;
    rax = request->image_base;
    r8 ^= rax;
    r8 *= request->read<uintptr_t>(rcx + 0x19);
    return r8;
}

inline uintptr_t decrypt_client_base(uintptr_t client_info)
{
    const uint64_t mb = request->image_base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    r8 = request->read<uintptr_t>(client_info + 0x1e9178);
    if (!r8)
        return r8;
    rbx = request->peb;
    rax = rbx;
    rax <<= 0x23;
    rax = _byteswap_uint64(rax);
    rax &= 0xF;
    switch (rax) {
    case 0:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        rax = request->image_base;
        r8 -= rax;
        rax = r8;
        rax >>= 0x1E;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x3C;
        r8 ^= rax;
        rax = request->image_base;
        r8 -= rax;
        rax = r8;
        rax >>= 0x28;
        r8 ^= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rax = 0x9CC8E0420ADA280D;
        r8 *= rax;
        r8 += rbx;
        rax = r8;
        rax >>= 0x11;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x22;
        r8 ^= rax;
        return r8;
    }
    case 1:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r15 = request->image_base + 0x755F7BDD;
        rax = r8;
        rax >>= 0x9;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x12;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x24;
        r8 ^= rax;
        r8 ^= rbx;
        rcx = 0;
        rcx = _rotl64(rcx, 0x10);
        rcx ^= r10;
        rcx = ~rcx;
        r8 *= request->read<uintptr_t>(rcx + 0x9);
        rcx = request->image_base;
        rax = rbx;
        rax -= rcx;
        rax += 0xFFFFFFFF9F0CFAED;
        r8 += rax;
        rax = 0x40ED86BABDEA8F5B;
        r8 *= rax;
        rax = 0xA7798517B7F399EA;
        r8 ^= rax;
        rax = r15;
        rax = ~rax;
        rax ^= rbx;
        r8 += rax;
        rax = 0x459093E765583ADB;
        r8 *= rax;
        return r8;
    }
    case 2:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r14 = request->image_base + 0xAC81;
        rax = 0xE03443781C6DB26D;
        r8 *= rax;
        rax = 0x26676A6627BAC50C;
        r8 -= rax;
        rax = 0x541ECC7788F37ADE;
        r8 += rax;
        r8 += r14;
        rax = request->image_base + 0x142;
        rax = ~rax;
        rcx = rbx;
        rcx = ~rcx;
        rcx -= rbx;
        rcx += rax;
        r8 += rcx;
        rax = r8;
        rax >>= 0x15;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x2A;
        r8 ^= rax;
        rax = request->image_base;
        r8 -= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        return r8;
    }
    case 3:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r13 = request->image_base + 0xF26D;
        r8 ^= rbx;
        rcx = 0;
        rcx = _rotl64(rcx, 0x10);
        rcx ^= r10;
        rcx = ~rcx;
        r8 *= request->read<uintptr_t>(rcx + 0x9);
        rax = 0x6C5618A3BE4C414;
        r8 -= rax;
        rax = 0xE98709096AD185CC;
        r8 ^= rax;
        rax = r8;
        rcx = request->image_base + 0x5ED318FB;
        rax >>= 0xB;
        rcx = ~rcx;
        r8 ^= rax;
        rcx *= rbx;
        rax = r8;
        rax >>= 0x16;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x2C;
        rcx ^= rax;
        r8 ^= rcx;
        rax = 0x22A1571E2E749CB;
        r8 *= rax;
        rax = rbx;
        rax *= r13;
        r8 += rax;
        return r8;
    }
    case 4:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r15 = request->image_base + 0x72D0A311;
        rax = 0x54EE9012A77B3C0E;
        r8 ^= rax;
        rax = request->image_base;
        rax += 0x432D;
        rax += rbx;
        r8 += rax;
        rax = 0xBE6A84FFF3304C3D;
        r8 *= rax;
        rax = r8;
        rax >>= 0x12;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x24;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x7;
        r8 ^= rax;
        rax = r8;
        rax >>= 0xE;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x1C;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x38;
        r8 ^= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rax = rbx;
        rax *= r15;
        r8 -= rax;
        rax = 0x598660DAA37ACC99;
        r8 ^= rax;
        return r8;
    }
    case 5:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        rcx = 0xC088FB236BE68165;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rax = request->read<uintptr_t>(rax + 0x9);
        rax *= rcx;
        r8 *= rax;
        rax = r8;
        rax >>= 0x5;
        r8 ^= rax;
        rax = r8;
        rax >>= 0xA;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x14;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x28;
        r8 ^= rax;
        rax = r8;
        rax >>= 0xB;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x16;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x2C;
        r8 ^= rax;
        rax = 0xF87FD44152069748;
        r8 ^= rax;
        rax = request->image_base;
        rax += 0x1079;
        rax += rbx;
        r8 ^= rax;
        rcx = request->image_base;
        rax = rbx;
        rax = ~rax;
        rax -= rcx;
        rax += 0xFFFFFFFF968271AB;
        r8 += rax;
        return r8;
    }
    case 6:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r15 = request->image_base + 0x1EE2;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rcx = 0x30DABF93D6E4FB5;
        r8 ^= rcx;
        rax = rbx;
        rax ^= r15;
        r8 -= rax;
        rax = 0xDB8B0AAFA542904;
        r8 -= rbx;
        r8 -= rax;
        rax = r8;
        rax >>= 0x22;
        r8 ^= rax;
        rax = 0xDF170407BBE28DB5;
        r8 *= rax;
        rax = r8;
        rax >>= 0x8;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x10;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x20;
        r8 ^= rax;
        return r8;
    }
    case 7:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r14 = request->image_base + 0xC177;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rax = 0x378CE09B287B2D41;
        r8 ^= rax;
        rax = 0x1C4A7DE2E2F8F68F;
        r8 += rax;
        rax = r8;
        rax >>= 0x23;
        rax ^= r8;
        r8 = rbx + 0x1;
        rax += rbx;
        r8 *= r14;
        r8 += rax;
        rax = 0xEBEA9B8B5714671D;
        r8 *= rax;
        rax = r8;
        rax >>= 0xE;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x1C;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x38;
        r8 ^= rax;
        return r8;
    }
    case 8:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r13 = request->image_base + 0x553;
        r15 = request->image_base + 0x88B9;
        rax = request->image_base;
        r8 ^= rax;
        rax = 0x3169FBDB3B875224;
        r8 += rax;
        rax = r15;
        rax = ~rax;
        rax *= rbx;
        r8 ^= rax;
        r8 ^= rbx;
        r8 ^= r13;
        rax = r8;
        rax >>= 0x13;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x26;
        r8 ^= rax;
        rcx = 0;
        rcx = _rotl64(rcx, 0x10);
        rax = 0x49665D7F2AFA3F6B;
        r8 *= rax;
        rcx ^= r10;
        rax = request->image_base + 0x11D125F7;
        rax = ~rax;
        rcx = ~rcx;
        rax *= rbx;
        r8 += rax;
        r8 *= request->read<uintptr_t>(rcx + 0x9);
        return r8;
    }
    case 9:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r11 = request->image_base + 0x7C81;
        rax = rbx;
        rax *= r11;
        r8 -= rax;
        rax = request->image_base;
        r8 -= rax;
        rax = rbx;
        rax -= request->image_base; // -- didn't find trace -> use base
        rax += 0xFFFFFFFFFFFF4D38;
        r8 += rax;
        rax = 0xB294869EA09D48AA;
        r8 ^= rax;
        rax = 0xDA6A9700AB4D27FD;
        r8 *= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rax = 0x38632CDC13FD78A5;
        r8 += rax;
        rax = r8;
        rax >>= 0x1D;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x3A;
        r8 ^= rax;
        return r8;
    }
    case 10:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r8 += rbx;
        rax = 0x36164EFD786890C1;
        r8 *= rax;
        rax = 0x6F993F33D7A49418;
        r8 += rax;
        rax = r8;
        rax >>= 0x8;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x10;
        r8 ^= rax;
        rcx = 0;
        rcx = _rotl64(rcx, 0x10);
        rax = r8;
        rcx ^= r10;
        rax >>= 0x20;
        r8 ^= rax;
        rcx = ~rcx;
        r8 *= request->read<uintptr_t>(rcx + 0x9);
        rax = 0xE88B55E25B8B057C;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x1A;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x34;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x4;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x8;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x10;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x20;
        r8 ^= rax;
        return r8;
    }
    case 11:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rax = rbx;
        uintptr_t RBP_0xFFFFFFFFFFFFFFB8;
        RBP_0xFFFFFFFFFFFFFFB8 = request->image_base + 0xA1FD; // Original value from RBP+0xFFFFFFFFFFFFFFB8
        rax *= RBP_0xFFFFFFFFFFFFFFB8;
        r8 += rax;
        rax = r8;
        rax >>= 0x26;
        r8 ^= rax;
        rax = r8;
        rax >>= 0xA;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x14;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x28;
        r8 ^= rax;
        rax = 0xC6A8E21F37CF3675;
        r8 *= rax;
        rax = request->image_base;
        rax += rbx;
        r8 -= rax;
        rax = request->image_base;
        r8 ^= rax;
        return r8;
    }
    case 12:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        rax = 0x5D2901AC55739352;
        r8 -= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        rax = request->image_base;
        r8 += rax;
        rax = 0x156D71AB28FBFAFF;
        r8 *= rax;
        rax = r8;
        rax >>= 0x27;
        r8 ^= rax;
        r8 -= rbx;
        rax = r8;
        rax >>= 0x17;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x2E;
        r8 ^= rax;
        rax = request->image_base;
        r8 ^= rax;
        return r8;
    }
    case 13:
    {
        r11 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        rax = r8;
        rax >>= 0x1F;
        r8 ^= rax;
        rcx = r8;
        rax = request->image_base;
        rcx >>= 0x3E;
        rcx ^= r8;
        rdx = 0;
        rdx = _rotl64(rdx, 0x10);
        r8 = rbx;
        r8 = ~r8;
        rdx ^= r11;
        r8 += rcx;
        rdx = ~rdx;
        r8 -= rax;
        r8 -= 0x6929AFAC;
        r8 *= request->read<uintptr_t>(rdx + 0x9);
        rax = r8;
        rax >>= 0x18;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x30;
        r8 ^= rax;
        rax = 0x69651B1AF033619B;
        r8 += rbx;
        r8 *= rax;
        rax = 0x29BBD1B30DFD9417;
        r8 *= rax;
        rax = 0xA7B8F15C4FABBB6C;
        r8 ^= rax;
        return r8;
    }
    case 14:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r13 = request->image_base + 0x8D0;
        r8 += rbx;
        rax = r13;
        rax -= rbx;
        r8 += rax;
        rax = 0xBC0AAA7E98B1663A;
        r8 ^= rax;
        rax = 0x54D1F9305B205B45;
        r8 *= rax;
        rax = r8;
        rax >>= 0xA;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x14;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x28;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x12;
        r8 ^= rax;
        rax = r8;
        rax >>= 0x24;
        r8 ^= rax;
        rax = 0xFFFFFFFFDE23E20A;
        rax -= rbx;
        rax -= request->image_base; // -- didn't find trace -> use base
        r8 += rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        r8 *= request->read<uintptr_t>(rax + 0x9);
        return r8;
    }
    case 15:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85AAA);
        r15 = request->image_base + 0x76BB;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rax = request->read<uintptr_t>(rax + 0x9);
        uintptr_t RBP_0xFFFFFFFFFFFFFFB0;
        RBP_0xFFFFFFFFFFFFFFB0 = 0x3A27415DA31CA989; // Original value from RBP+0xFFFFFFFFFFFFFFB0
        rax *= RBP_0xFFFFFFFFFFFFFFB0;
        r8 *= rax;
        rax = 0x6F6A3BE0CADE4A54;
        r8 -= rax;
        r8 -= rbx;
        rax = r8;
        rax >>= 0x13;
        rcx = rbx;
        r8 ^= rax;
        rcx = ~rcx;
        rax = r15;
        rax = ~rax;
        rcx *= rax;
        rax = r8;
        rax >>= 0x26;
        r8 ^= rax;
        r8 += rcx;
        rax = r8;
        rax >>= 0x28;
        r8 ^= rax;
        rax = 0x3224CE0A9BEB6A6E;
        r8 -= rax;
        return r8;
    }
    }
}

inline uintptr_t decrypt_bone_base()
{
    const uint64_t mb = request->image_base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rdx = request->read<uintptr_t>(request->image_base + 0x13613F88);
    if (!rdx)
        return rdx;
    r11 = request->peb;
    rax = r11;
    rax >>= 0x13;
    rax &= 0xF;
    switch (rax) {
    case 0:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0x8C93;
        rax = r15;
        rax -= r11;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x15;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x2A;
        rdx ^= rax;
        rdx += r11;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = request->image_base;
        rdx ^= rax;
        rax = 0x860534C8C01FEA7B;
        rdx *= rax;
        rax = 0xEE334BF3EC572D68;
        rdx ^= rax;
        return rdx;
    }
    case 1:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0xDF5D;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = 0x8A4B98169395E686;
        rdx ^= rax;
        rax = 0xC3957EB9F84EC5AF;
        rdx *= rax;
        rax = rdx;
        rax >>= 0xE;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x1C;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x38;
        rdx ^= rax;
        rax = request->image_base + 0x31CB;
        rax -= r11;
        rdx += rax;
        rax = rdx;
        rax >>= 0xD;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x1A;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x34;
        rdx ^= rax;
        rax = r15;
        rax = ~rax;
        rax ^= r11;
        rdx ^= rax;
        return rdx;
    }
    case 2:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0x47C2AE1B;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = r15;
        rax = ~rax;
        rax ^= r11;
        rax += r11;
        rdx += rax;
        rax = 0x94073D91C803188D;
        rdx ^= rax;
        rax = 0x2EEA8A0831CE333B;
        rdx *= rax;
        rdx += r11;
        rax = rdx;
        rax >>= 0x13;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x26;
        rdx ^= rax;
        rax = 0xD4E2CCE5B7959CA0;
        rdx ^= rax;
        return rdx;
    }
    case 3:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0x114B;
        rax = rdx;
        rax >>= 0x13;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x26;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x24;
        rdx ^= rax;
        rax = 0x764F15DD269101D3;
        rdx *= rax;
        rax = 0x34E81942B113C230;
        rdx -= rax;
        rax = 0x13805FC46F4FC36A;
        rdx += rax;
        rax = r11;
        rax -= request->image_base; // -- didn't find trace -> use base
        rax += 0xFFFFFFFFFFFF85F3;
        rdx += rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = r15;
        rax ^= r11;
        rdx ^= rax;
        return rdx;
    }
    case 4:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        rax = rdx;
        rax >>= 0x11;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x22;
        rdx ^= rax;
        rax = 0x2CFB6FB2F3BAD3C;
        rdx -= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = 0xEED0F28134CE8447;
        rdx *= rax;
        rax = 0x52D4170A67BFFCB2;
        rdx ^= rax;
        rdx += r11;
        rax = rdx;
        rax >>= 0x16;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x2C;
        rdx ^= rax;
        rdx ^= r11;
        return rdx;
    }
    case 5:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0x19B7DBCB;
        r12 = request->image_base + 0x654BDD13;
        rax = r12;
        rax = ~rax;
        rax++;;
        rax += r11;
        rdx += rax;
        rax = 0x4A2AFA53025C5181;
        rdx *= rax;
        rax = rdx;
        rax >>= 0x28;
        rdx ^= rax;
        rcx = request->image_base + 0xA045;
        rcx += r11;
        rax = r11 + r15 * 1;
        rcx ^= rax;
        rdx ^= rcx;
        rax = 0x574A3A5B7408079B;
        rdx *= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        return rdx;
    }
    case 6:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0x4951;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rdx += r11;
        rax = r15;
        rax *= r11;
        rdx += rax;
        rax = request->image_base + 0x1D37B933;
        rax += r11;
        rdx += rax;
        rax = rdx;
        rax >>= 0x9;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x12;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x24;
        rdx ^= rax;
        rax = 0x6C2A29044A40E4C7;
        rdx *= rax;
        rax = request->image_base;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x3;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x6;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0xC;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x18;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x30;
        rdx ^= rax;
        return rdx;
    }
    case 7:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r12 = request->image_base + 0xCEFB;
        rax = request->image_base;
        rdx += rax;
        rax = 0x5F80490A38DB3901;
        rdx ^= rax;
        rax = 0x4EC9DC6A5902297D;
        rdx -= rax;
        rax = rdx;
        rax >>= 0x25;
        rdx ^= rax;
        rax = r12;
        rax ^= r11;
        rdx += rax;
        rax = 0x92B34BC27C367071;
        rdx *= rax;
        rdx -= r11;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        return rdx;
    }
    case 8:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r12 = request->image_base + 0x5723;
        r13 = request->image_base + 0xFAB2;
        rax = 0xE62DA6375F493113;
        rdx *= rax;
        rax = request->image_base;
        rdx -= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rdx -= r11;
        rax = rdx;
        rax >>= 0xF;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x1E;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x3C;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x3;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x6;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0xC;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x18;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x30;
        rdx ^= rax;
        rax = r11 + r13 * 1;
        rdx ^= rax;
        rax = r12;
        rax ^= r11;
        rdx ^= rax;
        return rdx;
    }
    case 9:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r12 = request->image_base + 0xF1EC;
        r13 = request->image_base + 0x5304B0E6;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rcx = request->image_base + 0x429D;
        rcx += r11;
        rdx += rcx;
        rax = r11 + r12 * 1;
        rdx ^= rax;
        rdx ^= r13;
        rdx ^= r11;
        rax = rdx;
        rax >>= 0x22;
        rdx ^= rax;
        rax = 0xEE899EDDAF56550;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0xE;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x1C;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x38;
        rdx ^= rax;
        rax = 0x39D515C223A57391;
        rdx *= rax;
        return rdx;
    }
    case 10:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r12 = request->image_base + 0x1A3D;
        rax = rdx;
        rax >>= 0xF;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x1E;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x3C;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x13;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x26;
        rdx ^= rax;
        rax = r11;
        rax = ~rax;
        rax *= r12;
        rdx ^= rax;
        rax = 0x8330B389343DA675;
        rdx *= rax;
        rax = 0x5A325A7184C15E55;
        rdx -= rax;
        rax = 0xE28957C95B7E497;
        rdx += rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rdx -= r11;
        return rdx;
    }
    case 11:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r14 = request->image_base + 0x67B591A2;
        rax = rdx;
        rax >>= 0x22;
        rdx ^= rax;
        rax = r14;
        rax = ~rax;
        rax ^= r11;
        rax += r11;
        rdx -= rax;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rdx ^= r11;
        rax = 0x112AEF7CBA9BEDF1;
        rdx *= rax;
        rax = 0x792205E77EAA6797;
        rdx ^= rax;
        return rdx;
    }
    case 12:
    {
        r9 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r14 = request->image_base + 0x70E4B3E1;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r9;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = 0x33BF00DD8A073650;
        rdx -= rax;
        rax = rdx;
        rax >>= 0xA;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x14;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x28;
        rdx ^= rax;
        rax = request->image_base;
        rdx ^= rax;
        rax = r14;
        rax = ~rax;
        rdx += rax;
        rax = 0x37300D9E69A77B2F;
        rdx *= rax;
        rdx -= r11;
        return rdx;
    }
    case 13:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r14 = request->image_base + 0x666C9DA0;
        rax = r14;
        rax ^= r11;
        rdx -= rax;
        rax = 0x124569EA4125D98;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x5;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0xA;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x14;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x28;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x1A;
        rdx ^= rax;
        rcx = 0;
        rcx = _rotl64(rcx, 0x10);
        rax = rdx;
        rcx ^= r10;
        rax >>= 0x34;
        rcx = ~rcx;
        rdx ^= rax;
        rdx *= request->read<uintptr_t>(rcx + 0x17);
        rdx ^= r11;
        rax = 0xD83F30F92C64DF4F;
        rdx ^= rax;
        rax = 0xB69AFD2628432A9D;
        rdx *= rax;
        return rdx;
    }
    case 14:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r15 = request->image_base + 0x5113;
        rax = rdx;
        rax >>= 0x1B;
        rdx ^= rax;
        rax = rdx;
        rcx = 0;
        rax >>= 0x36;
        rcx = _rotl64(rcx, 0x10);
        rdx ^= rax;
        rcx ^= r10;
        rcx = ~rcx;
        rdx *= request->read<uintptr_t>(rcx + 0x17);
        rax = 0xDC4274449EFE767B;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x6;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0xC;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x18;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x30;
        rdx ^= rax;
        rax = r15;
        rax ^= r11;
        rdx -= rax;
        rax = 0x4480AA60A21867F9;
        rdx *= rax;
        rax = request->image_base + 0xD03A;
        rax += r11;
        rdx += rax;
        return rdx;
    }
    case 15:
    {
        r10 = request->read<uintptr_t>(request->image_base + 0xCC85B9D);
        r13 = request->image_base + 0x642A39AC;
        r12 = request->image_base + 0x6744783A;
        rdx += r11;
        rax = r11;
        rax = ~rax;
        rax ^= r13;
        rdx -= rax;
        rdx ^= r12;
        rdx ^= r11;
        rax = 0;
        rax = _rotl64(rax, 0x10);
        rax ^= r10;
        rax = ~rax;
        rdx *= request->read<uintptr_t>(rax + 0x17);
        rax = 0x54750E0E4638841A;
        rdx += rax;
        rax = 0x17257FE07A931EB4;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x4;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x8;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x10;
        rdx ^= rax;
        rax = rdx;
        rax >>= 0x20;
        rdx ^= rax;
        rax = 0x7493CCED6314B08B;
        rdx *= rax;
        return rdx;
    }
    }
}

inline uint16_t get_bone_index(uint32_t bone_index)
{
    const uint64_t mb = request->image_base;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rbx = bone_index;
    rcx = rbx * 0x13C8;
    rax = 0xCB182C584BD5193;
    r11 = request->image_base;
    rax = _umul128(rax, rcx, (uintptr_t*)&rdx);
    rax = rcx;
    r10 = 0xD6FB75C08B670E5B;
    rax -= rdx;
    rax >>= 0x1;
    rax += rdx;
    rax >>= 0xC;
    rax = rax * 0x1E7D;
    rcx -= rax;
    rax = 0x4078E2A8FCDA18EF;
    r8 = rcx * 0x1E7D;
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);
    rdx >>= 0xB;
    rax = rdx * 0x1FC4;
    r8 -= rax;
    rax = 0xF0F0F0F0F0F0F0F1;
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);
    rax = 0x624DD2F1A9FBE77;
    rdx >>= 0x6;
    rcx = rdx * 0x44;
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);
    rax = r8;
    rax -= rdx;
    rax >>= 0x1;
    rax += rdx;
    rax >>= 0x6;
    rcx += rax;
    rax = rcx * 0xFA;
    rcx = r8 * 0xFC;
    rcx -= rax;
    rax = request->read<uint16_t>(rcx + r11 * 1 + 0xBD683B0);
    r8 = rax * 0x13C8;
    rax = r10;
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);
    rax = r10;
    rdx >>= 0xD;
    rcx = rdx * 0x261B;
    r8 -= rcx;
    r9 = r8 * 0x2F75;
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);
    rdx >>= 0xD;
    rax = rdx * 0x261B;
    r9 -= rax;
    rax = 0x8FB823EE08FB823F;
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);
    rax = 0x579D6EE340579D6F;
    rdx >>= 0x5;
    rcx = rdx * 0x39;
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);
    rdx >>= 0x6;
    rcx += rdx;
    rax = rcx * 0x176;
    rcx = r9 * 0x178;
    rcx -= rax;
    r14 = request->read<uint16_t>(rcx + r11 * 1 + 0xBD6C620);
    return r14;
}

struct ref_def_view {
    Vector2 tan_half_fov;
    char pad[0xC];
    Vector3 axis[3];
    unsigned int vieworigin[3];
    unsigned int refdefViewOrg_aab; //: 0x70
    unsigned int refdefViewOrg_set_aab; //: 0x74
}; ref_def_view ref_def_view_s;

struct ref_def_t {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    Vector2 tan_half_fov;
    char pad1[8];  // 8-byte padding
    char pad2[4];  // 4-byte padding
    Vector3 axis[3];
};
class refdef_class {

public:
    struct key {
        int32_t ref0;
        int32_t ref1;
        int32_t ref2;
    };

    auto get_retrieve_ref_def_origin() -> Vector3 {
        int outData[3]{ 0 };
        auto aab = ref_def_view_s.refdefViewOrg_aab;
        uintptr_t encptr[3]{ (uintptr_t)(&ref_def_view_s.vieworigin[0]), (uintptr_t)(&ref_def_view_s.vieworigin[1]), (uintptr_t)(&ref_def_view_s.vieworigin[2]) };


        outData[0] = static_cast<int>(ref_def_view_s.vieworigin[0] ^ ((encptr[0] ^ aab) * ((encptr[0] ^ aab) + 2)));
        outData[1] = static_cast<int>(ref_def_view_s.vieworigin[1] ^ ((encptr[1] ^ aab) * ((encptr[1] ^ aab) + 2)));
        outData[2] = static_cast<int>(ref_def_view_s.vieworigin[2] ^ ((encptr[2] ^ aab) * ((encptr[2] ^ aab) + 2)));

        return { *reinterpret_cast<float*>(&outData[0]), *reinterpret_cast<float*>(&outData[1]), *reinterpret_cast<float*>(&outData[2]) };
    }


    auto retrieve_ref_def() -> uintptr_t {
        uint64_t ref_def_ptr = offsets::ref_def_ptr;

        key encrypted = request->read<key>(request->image_base + ref_def_ptr);

        DWORD lowerref = encrypted.ref0 ^ (encrypted.ref2 ^ (uint64_t)(request->image_base + ref_def_ptr)) *
            ((encrypted.ref2 ^ (uint64_t)(request->image_base + ref_def_ptr)) + 2);

        DWORD upperref = encrypted.ref1 ^ (encrypted.ref2 ^ (uint64_t)(request->image_base + ref_def_ptr + 0x4)) *
            ((encrypted.ref2 ^ (uint64_t)(request->image_base + ref_def_ptr + 0x4)) + 2);

        return (uint64_t)upperref << 32 | lowerref;
    }
    ref_def_t ref_def_nn;
};
static refdef_class* decrypt_refdef = new refdef_class();

