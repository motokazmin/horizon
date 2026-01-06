# 🚀 Quick Start Guide

## For First-Time Users

### Step 1: Verify Prerequisites
Before building, run the verification script:

```batch
# Double-click or run from command prompt:
verify_setup.bat
```

This will check if you have:
- ✅ Visual Studio C++ compiler
- ✅ CMake
- ✅ Qt 6.5+
- ✅ Git (optional)

**If any check fails**, follow the detailed installation guide in `BUILD_INSTRUCTIONS.md`

---

### Step 2: Quick Build (Recommended)

**Option A: Using Quick Build Script**

1. Open **"Developer Command Prompt for VS 2022"** from Start Menu
2. Navigate to project directory:
   ```batch
   cd path\to\HorizonUTM_MVP
   ```
3. Edit `quick_build.bat` if your Qt is not at `C:\Qt\6.5.3\msvc2019_64`
4. Run the script:
   ```batch
   quick_build.bat
   ```

The script will automatically:
- Create build directory
- Configure CMake
- Build Release version
- Show executable location

**Option B: Manual Build**

```batch
# 1. Set Qt path (adjust version as needed)
set CMAKE_PREFIX_PATH=C:\Qt\6.5.3\msvc2019_64

# 2. Create and enter build directory
mkdir build
cd build

# 3. Configure
cmake ..

# 4. Build
cmake --build . --config Release

# 5. Executable is at: bin\Release\HorizonUTM.exe
```

---

### Step 3: Deploy Qt DLLs

Before running, you need to copy Qt DLLs:

```batch
cd bin\Release
windeployqt HorizonUTM.exe
```

This copies all required Qt DLLs to the executable directory.

---

### Step 4: Run Application

```batch
HorizonUTM.exe
```

Or double-click `HorizonUTM.exe` in the `bin\Release` folder.

---

## Troubleshooting

### "Qt6 not found" during CMake
→ Set CMAKE_PREFIX_PATH to your Qt installation
```batch
set CMAKE_PREFIX_PATH=C:\Qt\6.5.3\msvc2019_64
```

### "Missing DLL" when running
→ Run windeployqt (see Step 3)

### "MSVC compiler not found"
→ Run from "Developer Command Prompt for VS"

### More issues?
→ See `BUILD_INSTRUCTIONS.md` for detailed troubleshooting

---

## Files Reference

| File | Purpose |
|------|---------|
| `verify_setup.bat` | Check prerequisites |
| `quick_build.bat` | Automated build script |
| `BUILD_INSTRUCTIONS.md` | Detailed setup guide |
| `README.md` | Project overview |
| `PROJECT_STRUCTURE.txt` | Code organization |

---

## What's Next?

After successful build, the application is ready for Phase 2 implementation:
- Domain entities
- Mock hardware driver
- Real-time UI
- Database integration

See `docs/mvp_scope.md` for development roadmap.
