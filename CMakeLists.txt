cmake_minimum_required(VERSION 3.24)
project(GenesisRedux VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE (cloned by scripts/setup.ps1 into external/JUCE)
add_subdirectory(external/JUCE)

# Eigen (header-only)
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE ${CMAKE_SOURCE_DIR}/external/eigen)

# SQLite amalgamation (place sqlite3.[ch] under external/sqlite)
add_library(sqlite3 STATIC external/sqlite/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${CMAKE_SOURCE_DIR}/external/sqlite)

# FFTW (optional, needed later for fast FFT/RTA). Provide binaries in external/fftw
if (EXISTS "${CMAKE_SOURCE_DIR}/external/fftw/fftw3.dll")
  add_library(fftw3 SHARED IMPORTED)
  set_target_properties(fftw3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/fftw/fftw3.dll"
    IMPORTED_IMPLIB   "${CMAKE_SOURCE_DIR}/external/fftw/fftw3.lib")
else()
  add_library(fftw3 INTERFACE)
endif()

juce_add_gui_app(GenesisRedux
    PRODUCT_NAME "Genesis Redux"
)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)

target_sources(GenesisRedux PRIVATE ${APP_SOURCES})

target_link_libraries(GenesisRedux PRIVATE
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_audio_devices
    juce::juce_audio_basics
    eigen sqlite3 fftw3
)

if (MSVC)
  target_compile_definitions(GenesisRedux PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
