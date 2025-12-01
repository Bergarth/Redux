# Genesis Redux (Windows JUCE Starter)

This is a **starter project** for the room correction app we've designed together.
It builds a Windows desktop app with the basic shell and placeholders for Device,
Measurement, Target/Blend, Solver, and Export.

> Install location you requested: `C:\Users\chris\Documents\GenisisRedux` (note the spelling).

## One-time setup (Windows)

1. **Install Visual Studio 2022 Community**
   - During install, select **Desktop development with C++** workload.

2. **Install CMake (if not already)**
   - `winget install Kitware.CMake`

3. **Clone/extract this folder**
   - Put the folder at: `C:\Users\chris\Documents\GenisisRedux`

4. **Open PowerShell in that folder and run:**
   ```powershell
   Set-ExecutionPolicy -Scope CurrentUser RemoteSigned
   .\scripts\setup.ps1
   ```
   This downloads **JUCE**, **Eigen**, and **SQLite amalgamation** into the `external/` folder.

5. **Configure & Build with CMake (from the same PowerShell):**
   ```powershell
   cmake -S . -B build -G "Visual Studio 17 2022" -A x64
   cmake --build build --config Debug
   ```

6. **Run the app:**
   ```powershell
   .\build\Debug\GenesisRedux.exe
   ```

## Project layout

```
GenesisRedux/
  CMakeLists.txt
  external/
    JUCE/           (downloaded by setup.ps1)
    eigen/          (downloaded by setup.ps1)
    sqlite/         (sqlite3.c/.h downloaded by setup.ps1)
    fftw/           (optional: place fftw3.dll/.lib here later)
  src/
    App.h/.cpp
    MainWindow.h/.cpp
    MainComponent.h/.cpp
    ui/ (HomePage, MeasurePage, TargetBlendPage, ReviewExportPage)
    services/DeviceService (Denon/Marantz HTTP discovery & snapshot)
    dsp/ (MeasurementEngine, BlendEngine, Solver)
    store/ProjectStore (SQLite glue)
    util/ResponseMath (DSP helpers)
  scripts/
    setup.ps1
```

## Next steps in code

- **Home tab** already shows Discover/IP buttons and a summary label. `DeviceService` is mocked and will be wired to HTTP endpoints next.
- Measurement/Blend/Solver are stubs; we will fill them incrementally.

If anything fails, send me the error text and I’ll adjust the project files.
