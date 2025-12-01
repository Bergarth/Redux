Param()

$ErrorActionPreference = "Stop"

# Ensure external dir exists
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$ext = Join-Path $root "..\external"
$ext = Resolve-Path $ext
Write-Host "Using external dir: $ext"

# JUCE
$juceDir = Join-Path $ext "JUCE"
if (!(Test-Path $juceDir)) {
  git clone --depth 1 https://github.com/juce-framework/JUCE.git $juceDir
} else { Write-Host "JUCE already present." }

# Eigen
$eigenDir = Join-Path $ext "eigen"
if (!(Test-Path $eigenDir)) {
  git clone --depth 1 https://gitlab.com/libeigen/eigen.git $eigenDir
} else { Write-Host "Eigen already present." }

# SQLite amalgamation
$sqliteDir = Join-Path $ext "sqlite"
if (!(Test-Path (Join-Path $sqliteDir "sqlite3.c"))) {
  Invoke-WebRequest -Uri https://www.sqlite.org/2024/sqlite-amalgamation-3460000.zip -OutFile "$sqliteDir\sqlite.zip"
  Expand-Archive "$sqliteDir\sqlite.zip" -DestinationPath $sqliteDir -Force
  $unz = Get-ChildItem $sqliteDir -Directory | Where-Object { $_.Name -like "sqlite-amalgamation*" } | Select-Object -First 1
  Copy-Item (Join-Path $unz.FullName "sqlite3.c") $sqliteDir -Force
  Copy-Item (Join-Path $unz.FullName "sqlite3.h") $sqliteDir -Force
  Copy-Item (Join-Path $unz.FullName "sqlite3ext.h") $sqliteDir -Force
  Remove-Item "$sqliteDir\sqlite.zip"
} else { Write-Host "SQLite amalgamation already present." }

Write-Host "Done. You can now run CMake."
