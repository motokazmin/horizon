# Build Instructions for Horizon UTM

---

## 🚀 Quick Start for First-Time Users

### Complete Setup Guide (Follow in order)

**Time estimate: 1-2 hours for complete setup**

1. ✅ **Install Visual Studio** (30-45 min)
   - See "Step 1" below for detailed instructions
   - Download VS 2022 Community (free)
   - Select "Desktop development with C++"

2. ✅ **Install Qt** (30-45 min)
   - See "Step 3" below for detailed instructions
   - Download Qt 6.5+ Online Installer
   - Select MSVC 2019 64-bit with Charts, SQL, Widgets

3. ✅ **Configure Environment** (5 min)
   - Add Qt to PATH (see "Step 4")
   - Verify installations (see verification steps)

4. ✅ **Build Project** (5-10 min)
   - Extract HorizonUTM_MVP.zip
   - Follow "Building the Project" section below
   - Run application

---

## Prerequisites Installation Guide

### Minimum Requirements
- **OS**: Windows 10 (64-bit) or later
- **RAM**: 4 GB minimum, 8 GB recommended
- **Disk Space**: 10 GB free space (for Qt, tools, and build)
- **Display**: 1920x1080 or higher recommended

## Prerequisites Installation Guide

### Step 1: Install Visual Studio 2019 or 2022

**Download:**
- Visual Studio 2022 Community (Free): https://visualstudio.microsoft.com/downloads/
- Or Visual Studio 2019 Community: https://visualstudio.microsoft.com/vs/older-downloads/

**Installation Steps:**
1. Run the installer
2. Select "Desktop development with C++"
3. In the "Individual components" tab, ensure these are checked:
   - ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools (or v142 for VS 2019)
   - ✅ C++ CMake tools for Windows
   - ✅ Windows 10 SDK (10.0.19041.0 or later)
   - ✅ C++ core features
4. Click Install (will take 15-30 minutes)

**Verify Installation:**
```batch
# Open "Developer Command Prompt for VS 2022" and run:
cl
# Should show Microsoft C/C++ compiler version
```

---

### Step 2: Install CMake 3.20+

**Option A: Via Visual Studio (Already included if you selected CMake tools)**
- CMake is automatically installed with Visual Studio

**Option B: Standalone Installation**
1. Download CMake from: https://cmake.org/download/
2. Get "Windows x64 Installer" (cmake-3.XX.X-windows-x86_64.msi)
3. During installation, select "Add CMake to system PATH for all users"

**Verify Installation:**
```batch
cmake --version
# Should show: cmake version 3.20.0 or higher
```

---

### Step 3: Install Qt 6.5+

**Download Qt:**
1. Go to: https://www.qt.io/download-open-source
2. Click "Download the Qt Online Installer"
3. Download "Qt Online Installer for Windows"

**Installation Steps:**
1. Run the installer (requires Qt account - free to create)
2. Login or create a free Qt account
3. Accept license agreements (Open Source)
4. Select installation directory (default: C:\Qt)
5. **IMPORTANT:** In component selection, select:
   ```
   Qt 6.5.3 (or later) for desktop development
   └── MSVC 2019 64-bit (or MSVC 2022 64-bit)
       ├── ✅ Qt Charts
       ├── ✅ Qt SQL
       ├── ✅ Qt Widgets
       ├── Qt Core (automatically included)
       └── ✅ Qt5 Compatibility Module (if needed)
   
   Developer and Designer Tools
   └── ✅ CMake (optional if already installed)
   ```
6. Install (will take 20-40 minutes, ~5-8 GB)

**Verify Installation:**
```batch
# Check Qt installation directory exists:
dir C:\Qt\6.5.3\msvc2019_64\bin
# Should list qmake.exe, Qt6Core.dll, etc.
```

---

### Step 4: Set Environment Variables

**Add Qt to System PATH (Required for runtime):**

1. Open "Environment Variables":
   - Press `Win + R`, type `sysdm.cpl`, press Enter
   - Click "Advanced" tab → "Environment Variables"

2. Under "System variables", find "Path", click "Edit"

3. Click "New" and add:
   ```
   C:\Qt\6.5.3\msvc2019_64\bin
   ```
   *(Adjust version and compiler as needed)*

4. Click "OK" on all dialogs

**Verify:**
```batch
# Close and reopen Command Prompt, then run:
qmake --version
# Should show Qt version information
```

---

### Step 5: Install Git (Optional but recommended)

**Download:**
- Git for Windows: https://git-scm.com/download/win

**Installation:**
1. Run installer with default options
2. Select "Use Git from the Windows Command Prompt"

**Verify:**
```batch
git --version
```

---

### Step 6: Additional Dependencies (Included)

**Good news!** The following are automatically included and require no installation:

✅ **SQLite** - Included with Qt6 Sql module
- No separate installation needed
- Driver (QSQLITE) comes with Qt

✅ **C++ Standard Library** - Included with Visual Studio
- C++20 features used in project
- No additional setup required

✅ **Qt Core Libraries** - Included with Qt installation
- Qt6Core, Qt6Gui automatically included
- Network, Concurrent modules available if needed

---

## Prerequisites Verification Script

**Save this as `verify_setup.bat` and run it:**

```batch
@echo off
echo ========================================
echo Horizon UTM - Prerequisites Verification
echo ========================================
echo.

echo [1/5] Checking Visual Studio...
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Visual Studio C++ compiler found
    cl 2>&1 | findstr "Version"
) else (
    echo [ERROR] Visual Studio compiler not found
    echo Please run this from "Developer Command Prompt for VS"
)
echo.

echo [2/5] Checking CMake...
where cmake >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] CMake found
    cmake --version | findstr "version"
) else (
    echo [ERROR] CMake not found in PATH
)
echo.

echo [3/5] Checking Qt...
where qmake >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Qt found
    qmake --version
) else (
    echo [ERROR] Qt not found in PATH
    echo Add Qt bin directory to PATH: C:\Qt\6.5.3\msvc2019_64\bin
)
echo.

echo [4/5] Checking Git (optional)...
where git >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Git found
    git --version
) else (
    echo [SKIP] Git not found (optional)
)
echo.

echo [5/5] Checking disk space...
for /f "tokens=3" %%a in ('dir /-c C:\ ^| findstr "bytes free"') do set FREE=%%a
echo [INFO] Free disk space on C:\ (at least 10GB recommended)
echo.

echo ========================================
echo Verification complete!
echo If all checks passed, you're ready to build.
echo ========================================
pause
```

**Run verification:**
```batch
# From Developer Command Prompt:
verify_setup.bat
```

---

Before building, verify you have:
- [ ] Visual Studio 2019/2022 with C++ tools installed
- [ ] CMake 3.20+ installed and in PATH
- [ ] Qt 6.5+ installed (MSVC variant with Charts, SQL, Widgets)
- [ ] Qt bin directory added to system PATH
- [ ] 10 GB free disk space
- [ ] Windows 10 64-bit or later

## Building the Project

### Option 1: Command Line (Windows)

```batch
# Set Qt path (adjust version and path as needed)
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build
cmake --build . --config Release

# Run
bin\Release\HorizonUTM.exe
```

### Option 2: Visual Studio

1. Open Visual Studio
2. File → Open → CMake → Select `CMakeLists.txt`
3. Select configuration (x64-Release)
4. Build → Build All
5. Run from `out\build\x64-Release\bin\HorizonUTM.exe`

### Option 3: Qt Creator

1. Open Qt Creator
2. File → Open File or Project → Select `CMakeLists.txt`
3. Configure project with Qt kit
4. Build → Build Project
5. Run

## Troubleshooting

### Problem: "CMake Error: Could not find Qt6"

**Solution 1:** Set CMAKE_PREFIX_PATH
```batch
set CMAKE_PREFIX_PATH=C:\Qt\6.5.3\msvc2019_64
cmake ..
```

**Solution 2:** Add to CMake command directly
```batch
cmake -DCMAKE_PREFIX_PATH=C:\Qt\6.5.3\msvc2019_64 ..
```

**Solution 3:** Set permanent environment variable
```batch
setx CMAKE_PREFIX_PATH "C:\Qt\6.5.3\msvc2019_64"
# Restart terminal
```

---

### Problem: "Qt6Charts NOT found", "Qt6Sql NOT found"

**Cause:** Missing Qt components during installation

**Solution:** Re-run Qt Maintenance Tool
1. Open Qt Maintenance Tool: `C:\Qt\MaintenanceTool.exe`
2. Select "Add or remove components"
3. Check these under your Qt version:
   - ✅ Qt Charts
   - ✅ Qt SQL
   - ✅ Qt Widgets
4. Install missing components

---

### Problem: "MSVC compiler not found" or "No suitable compiler found"

**Cause:** Visual Studio not properly installed or CMake can't find it

**Solution 1:** Use Developer Command Prompt
- Open "Developer Command Prompt for VS 2022" from Start Menu
- Navigate to project directory
- Run cmake commands

**Solution 2:** Manually specify generator
```batch
cmake -G "Visual Studio 17 2022" -A x64 ..
# For VS 2019 use: "Visual Studio 16 2019"
```

---

### Problem: "Missing DLL" when running application

**Symptoms:**
- `Qt6Core.dll not found`
- `Qt6Widgets.dll not found`
- `Qt6Charts.dll not found`

**Solution 1:** Deploy Qt DLLs automatically (Recommended)
```batch
cd bin\Release
windeployqt HorizonUTM.exe
```

This will copy all required Qt DLLs to the executable directory.

**Solution 2:** Add Qt to PATH (for development only)
```batch
set PATH=C:\Qt\6.5.3\msvc2019_64\bin;%PATH%
HorizonUTM.exe
```

**Solution 3:** Manual DLL copy (not recommended)
Copy these DLLs from `C:\Qt\6.5.3\msvc2019_64\bin\` to your exe directory:
- Qt6Core.dll
- Qt6Gui.dll
- Qt6Widgets.dll
- Qt6Charts.dll
- Qt6Sql.dll

---

### Problem: CMake version too old

**Error:** `CMake 3.5 or higher is required. You are running version X.XX`

**Solution:** Update CMake
1. Uninstall old CMake
2. Download latest from: https://cmake.org/download/
3. Install with "Add to PATH" option

---

### Problem: Build fails with "cannot open file 'Qt6Core.lib'"

**Cause:** Wrong compiler selected or Qt MSVC version mismatch

**Solution:** Ensure matching compilers
- If using VS 2022 → Install Qt with MSVC 2019 64-bit (compatible)
- If using VS 2019 → Install Qt with MSVC 2019 64-bit

**Check your Qt installation:**
```batch
dir C:\Qt\6.5.3
# Should see: msvc2019_64 (or msvc2022_64)
```

---

### Problem: "This application failed to start because no Qt platform plugin could be initialized"

**Cause:** Missing Qt platform plugins

**Solution:** Run windeployqt (see "Missing DLL" section above)

Or manually copy `platforms` folder:
```batch
xcopy /E /I C:\Qt\6.5.3\msvc2019_64\plugins\platforms bin\Release\platforms
```

---

### Problem: Slow build times

**Solution 1:** Use parallel builds
```batch
cmake --build . --config Release -- /maxcpucount
```

**Solution 2:** Use Ninja generator (faster than MSVC)
```batch
# Install Ninja: choco install ninja (requires Chocolatey)
# Or download from: https://github.com/ninja-build/ninja/releases

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

---

### Getting Help

If you encounter issues not covered here:

1. **Check CMake output** for specific error messages
2. **Verify prerequisites** using the checklist above
3. **Check Qt version compatibility** with your Visual Studio version
4. **Search Qt forums**: https://forum.qt.io/
5. **Check project issues**: Review project documentation in `/docs`

**Common Compatibility Matrix:**
| Visual Studio | Qt MSVC Variant | Compatible |
|---------------|-----------------|------------|
| VS 2022       | msvc2019_64    | ✅ Yes     |
| VS 2019       | msvc2019_64    | ✅ Yes     |
| VS 2022       | msvc2022_64    | ✅ Yes     |
| VS 2017       | msvc2019_64    | ⚠️ Maybe   |

## Development Build

For faster iteration during development:
```
cmake --build . --config Debug
```

Debug executable: `bin\Debug\HorizonUTM.exe`
