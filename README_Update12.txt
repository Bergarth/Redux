
GenesisRedux Update 12
----------------------
Fixes:
- SSDP discovery implemented with JUCE DatagramSocket (uses URL::getDomain).
- Corrected socket read signature.
- Added optional `serial` field to DeviceSnapshot to resolve compile error.

Files included:
- src/net/SSDPDiscoverer.h
- src/net/SSDPDiscoverer.cpp
- src/services/DeviceSnapshot.h  (safe header, include if you don't already expose 'serial' in your snapshot)

How to apply:
1) Extract into your project root (C:\Users\chris\Documents\GenisisRedux) with -Force overwrite.
2) Make sure your build references src/net/SSDPDiscoverer.cpp in CMake.
3) Rebuild:  cmake --build build --config Debug
