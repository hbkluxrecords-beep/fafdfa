# BO7 External — UD Build

## What was done to make this undetected

Based on the UC Ricochet dump (CountryBoi / beck123x, October 2026).

### 1. ImGui RenderFrameBorder signature — PATCHED
Ricochet's physical memory scanner looks for:
```
4C 8B DC 53 48 81 EC [??] [??] [??] [??]
```
The `RenderFrameBorder` function was split into a `__declspec(noinline)` helper
with a `volatile` local. This forces MSVC to emit a different prologue sequence
in both functions. The scanned byte run never assembles contiguously.

### 2. Window class / title randomised — PATCHED
Every launch generates a 12-char random alphanumeric class name and 16-char
window title. No static string to fingerprint.

### 3. PEB ImagePathName spoofed — PATCHED
At startup the process patches its own PEB ImagePathName to show
`RuntimeBroker.exe` (a real Windows system process). Certificate checks
and process name scans see a clean name.

### 4. ETW usermode telemetry silenced — PATCHED
`EtwEventWrite` in ntdll is patched to `ret` in-process. Our activity
doesn't appear in kernel telemetry events.

### 5. AMSI disabled — PATCHED
`AmsiScanBuffer` patched to `xor eax,eax; ret`.

### 6. Hardware debug registers cleared — PATCHED
Own thread's DRx registers zeroed at startup via `SetThreadContext`.

### 7. No executable private pages — CLEAN
External process. No injection. No `VirtualAlloc(PAGE_EXECUTE_*)`.
All our memory is MEM_IMAGE (from the PE) which is the expected type.
Ricochet's page table walk + MBI check finds nothing anomalous.

### 8. Compiler flags break byte patterns — PATCHED
`/O1 /Ob0 /Oi- /GS- /GL /LTCG` produce different byte sequences per
build compared to default MSVC output. Each compile is unique.

### 9. No clipboard APIs — CLEAN
beck123x confirmed GlobalAlloc/Clipboard instant shadowban on internal.
External uses none of these.

---

## Build requirements
- Windows 10/11 x64
- Visual Studio 2022 — "Desktop development with C++"
- No other deps

```
build.bat
```
Output: `out\RuntimeBroker_loader.exe`

---

## Launch order (every session)

1. **bo7_spoofer.exe** (admin) — from the spoofer build, run BEFORE game
2. Launch BO7 on junk account, reach **main menu**
3. **RuntimeBroker_loader.exe** (admin) — run AFTER game is at menu
4. Console window hides automatically
5. DX11 overlay appears over the game

**INSERT** = toggle menu  
**DELETE** = clean exit

---

## Controls

| Section | Control | Notes |
|---|---|---|
| Aimbot | Enable | Toggle on/off |
| Aimbot | Bone | Head (default) |
| Aimbot | FOV | 200px default — lower = tighter |
| Aimbot | Smooth | 5 for private/zombies, 8 for MP |
| Aimbot | Bind | **Right Mouse Button** = LT on Xbox pad |
| Aimbot | Vis check | Only aims at visible targets |
| Visuals | Box | 2D bounding box |
| Visuals | Skeleton | 14-bone full skeleton |
| Visuals | Health | Health bar |
| Visuals | Name | Player name tag |
| Visuals | Distance | Distance in metres |
| Visuals | Snaplines | Line from screen edge to target |

---

## If something breaks after a patch

The only file that changes between game updates is:
```
src/world/engine/offsets_final.h
```
Update the offsets there and rebuild. Everything else is static logic.

---

## Spoofer (from previous build)

Run `bo7_spoofer.exe` before launching the game. It handles:
- Disk serial spoof
- MAC address rotation  
- GPU descriptor hook
- Volume serial + MachineGuid spoof
- AC registry wipe (Activision keys, Ricochet service keys)
