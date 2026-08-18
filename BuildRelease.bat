@echo off
color 0A
setlocal enabledelayedexpansion

set "KEY=HKLM\SOFTWARE\WOW6432Node\Bethesda Softworks\Skyrim Special Edition"
set "VALUE=Installed Path"

for /f "skip=2 tokens=*" %%a in ('reg query "%KEY%" /v "%VALUE%" 2^>nul') do (
    set "line=%%a"
    set "line=!line:~24!"
    for /f "tokens=* delims= " %%b in ("!line!") do set "BASE_PATH=%%b"
)

set DEST_PATH=%BASE_PATH%\Data\SKSE\Plugins
set BUILD_PATH=build\RelWithDebInfo

echo Deleting contents of %BUILD_PATH%...
rd /s /q "%BUILD_PATH%"

cmake --preset build-release-msvc-msvc
if %ERRORLEVEL% NEQ 0 set /p=Press any key to continue...
cmake --build build --preset release-msvc-msvc
if %ERRORLEVEL% NEQ 0 set /p=Press any key to continue...

xcopy "build\RelWithDebInfo\*.dll" "%DEST_PATH%" /I /Y
xcopy "build\RelWithDebInfo\*.pdb" "%DEST_PATH%" /I /Y

cls
echo.
echo The program has been successfully compiled, and the files have been copied to the destination folder.
echo.

set /p=Press any key to start the game...

cd /d "%BASE_PATH%"
start "" "skse64_loader.exe"

exit
