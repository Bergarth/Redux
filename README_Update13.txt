GenesisRedux Update 13
----------------------
- Fix build errors: adds `DeviceSnapshot::serial` and `server` fields.
- SSDPDiscoverer result now includes `.server` and compiles against current JUCE.
- Correct `DatagramSocket::read` usage and general header parsing.

Drop-in instructions (PowerShell):
  cd "C:\Users\chris\Documents\GenisisRedux"
  Expand-Archive -Path "$env:USERPROFILE\Downloads\GenesisRedux_Update13.zip" -DestinationPath . -Force
  cmake --build build --config Debug
  & ".\build\GenesisRedux_artefacts\Debug\Genesis Redux.exe"
