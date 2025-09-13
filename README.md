# PARTYMOD for THPS2
This is a patch for THPS2 (original release) to improve its input handling as well as smooth out a few other issues in its PC port.
The patch is designed to keep the game as original as possible, and leave its files unmodified.

## 🎉 PARTYMOD 🎉
PARTYMOD is a series of patches that provide various fixes and modernizations for the THPS series and other games on their engines.
[Other PARTYMOD Releases Available Here](https://partymod.newnet.city/)

## Features and Fixes
* Replaces renderer with new bindless Vulkan renderer
* Replaces input system entirely with new, modern system using the SDL2 library
* Improves window handling allowing for custom resolutions and configurable windowing (as well as supporting high DPI scaling)
* Game resolution is now independent from window resolution, allowing the game to run at original 240p or 480p, in addition to window size
* Replaces the gamma correction that resulted in the game looking washed out, instead using PSX-style texture color blending
* Fixes a number of rendering bugs
* Fixes vehicle sounds constantly resetting in NY City and Philadelphia
* Movement stick now controls menus
* Improves cursor handling, only showing it when relevant and using the system cursor to prevent latency
* Replaces configuration files with new INI-based system (see partymod.ini)
* Custom configurator program to handle new configuration files
* Fixes memory manager to prevent memory corruption and crashes related to it
* Restores player control settings menu, allowing players to toggle vibration and autokick settings
* Fixes loading write-protected saves
* Experimental: An optional THPS1 career mode, converting the game to recreate the THPS1 career with THPS2's mechanics, which can be enabled with a setting or by passing `-thps1career` to `THPS2.exe`

## Installation
1. Download PARTYMOD from the releases tab
2. Make sure THPS2 (Original, not Activision Value or Korean release) is installed
3. Extract this zip file into your THPS2 installation directory
4. Run partypatcher.exe to create the new, patched THPS2.exe game executable (this will be used to launch the game from now on) (this only needs to be done once)
5. Optionally (highly recommended), configure the game with partyconfig.exe
6. Launch the game from THPS2.exe

NOTE: if the game is installed into the "Program Files" directory, you may need to run each program as administrator. 
Also, if the game is installed into the "Program Files" directory, save files will be saved in the C:\Users\<name>\AppData\Local\VirtualStore directory.  
For more information, see here: https://answers.microsoft.com/en-us/windows/forum/all/please-explain-virtualstore-for-non-experts/d8912f80-b275-48d7-9ff3-9e9878954227

## Building
The build requires CMake, SDL2 (I install it via vspkg), and the Vulkan SDK (available from [LunarG](https://www.lunarg.com/vulkan-sdk/)).  Create the project file like so from the partymod-thps2/build directory:
```
cmake .. -A win32 -DCMAKE_TOOLCHAIN_FILE=C:/[vcpkg directory]/scripts/buildsystems/vcpkg.cmake
```

Maybe set the optimization optimization for the partymod dll to O0 (disable optimization) because MSVC occasionally seems to break certain functions when optimization is enabled.
Additionally, set the SubSystem to "Windows (/SUBSYSTEM:WINDOWS)" in the partyconfig project.

Shaders can be built with [GLSLang](https://github.com/KhronosGroup/glslang). the download is somewhere in there. seems odd that the download to the actual command line utility is so hidden. oh well!
