@echo off
setlocal EnableDelayedExpansion
chcp 65001 >nul

echo.
echo  =============================================
echo   BO7 External -- UD Build
echo  =============================================
echo.

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [!] Visual Studio 2022 not found.
    pause & exit /b 1
)
for /f "usebackq tokens=*" %%i in (
    `"%VSWHERE%" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`
) do set "VS_PATH=%%i"
call "!VS_PATH!\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
echo [+] MSVC loaded.

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "OUT=%ROOT%out"
if not exist "%OUT%" mkdir "%OUT%"

rem UD compiler flags
rem /O1   = optimise for size (smaller pattern surface)
rem /Ob0  = no inlining (each function unique prologue)
rem /Oi-  = no intrinsics (avoids known byte patterns)
rem /GS-  = no stack cookie (removes __security_check_cookie)
rem /GL   = whole-program optimisation (LTCG changes byte layout)
rem /sdl- = disable SDL checks (removes extra prologues)
set "FLAGS=/nologo /W2 /WX- /std:c++20 /O1 /Ob0 /Oi- /GS- /Gy /GL /sdl-"
set "FLAGS=%FLAGS% /D UNICODE /D _UNICODE /D _CRT_SECURE_NO_WARNINGS"
set "FLAGS=%FLAGS% /D WIN32_LEAN_AND_MEAN /D NDEBUG"

set "INC=/I"%SRC%" /I"%SRC%\framework\imgui" /I"%SRC%\framework\overlay""
set "INC=%INC% /I"%SRC%\utils" /I"%SRC%\world\engine""

set "SOURCES="%SRC%\main.cpp" "%SRC%\utils\kernel\driver.cpp" "%SRC%\framework\imgui\imgui.cpp" "%SRC%\framework\imgui\imgui_draw.cpp" "%SRC%\framework\imgui\imgui_tables.cpp" "%SRC%\framework\imgui\imgui_widgets.cpp" "%SRC%\framework\imgui\imgui_impl_dx11.cpp" "%SRC%\framework\imgui\imgui_impl_win32.cpp" "%SRC%\framework\overlay\edited\imgui_edit.cpp""

set "EXENAME=RuntimeBroker_loader.exe"

echo [*] Building %EXENAME% ...

cl %FLAGS% %INC% %SOURCES% ^
    /Fe:"%OUT%\%EXENAME%" ^
    /link /SUBSYSTEM:WINDOWS /MANIFEST:EMBED ^
    /LTCG /OPT:REF /OPT:ICF ^
    /MANIFESTUAC:"level='requireAdministrator' uiAccess='false'" ^
    /ENTRY:mainCRTStartup ^
    d3d11.lib dxgi.lib d3dcompiler.lib ^
    user32.lib gdi32.lib dwmapi.lib ^
    advapi32.lib iphlpapi.lib ntdll.lib psapi.lib

if errorlevel 1 ( echo [!] FAILED & pause & exit /b 1 )

echo.
echo  =============================================
echo   DONE --  out\%EXENAME%
echo  =============================================
echo.
echo  LAUNCH ORDER:
echo  1. bo7_spoofer.exe  (admin, before game)
echo  2. Launch BO7 on junk account
echo  3. Wait for main menu
echo  4. out\%EXENAME%  (admin)
echo     Console hides itself. Overlay appears.
echo     INSERT = menu    DELETE = exit
echo.
echo  AIMBOT CONTROLLER TUNING:
echo  - Bind = Right Mouse (= LT on Xbox controller)
echo  - Bone = Head
echo  - Smooth = 5  (raise to 8 for multiplayer)
echo  - FOV = 200px default
echo.
pause
