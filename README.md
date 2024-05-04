# PARTYMOD for THPS2
This is a patch for THPS2 (original release) to improve its input handling as well as smooth out a few other parts of the PC port.
The patch is designed to keep the game as original as possible, and leave its files unmodified.

### Features and Fixes
* Replaced renderer with new bindless Vulkan renderer
* Replaced input system entirely with new, modern system using the SDL2 library
* Improved window handling allowing for custom resolutions and configurable windowing
* Game resolution is now independent from window resolution, allowing the game to run at original 240p or 480p, in addition to window size
* Replaced the gamma correction that resulted in the game looking washed out, instead using PSX-style texture color blending
* Fixed a number of rendering bugs
* Movement stick now controls menus
* Improved cursor handling, only showing it when relevant and using the system cursor to prevent latency
* Replaced configuration files with new INI-based system (see partymod.ini)
* Custom configurator program to handle new configuration files
* Fixed memory manager to prevent memory corruption and crashes related to it
* Added setting for autokick, allowing it to be turned off easily
* Fixed loading saves

### Installation
1. Download PARTYMOD from the releases tab
2. Make sure THPS2 (Original, not Activision Value or Korean release) is installed
3. Extract this zip file into your THPS2 installation directory
4. Run partypatcher.exe to create the new, patched THPS2.exe game executable (this will be used to launch the game from now on) (this only needs to be done once)
5. Optionally (highly recommended), configure the game with partyconfig.exe
6. Launch the game from THPS2.exe

NOTE: if the game is installed into the "Program Files" directory, you may need to run each program as administrator. 
Also, if the game is installed into the "Program Files" directory, save files will be saved in the C:\Users\<name>\AppData\Local\VirtualStore directory.  
For more information, see here: https://answers.microsoft.com/en-us/windows/forum/all/please-explain-virtualstore-for-non-experts/d8912f80-b275-48d7-9ff3-9e9878954227

### Building
The build requires CMake, SDL2 (I install it via vspkg), and the Vulkan SDK (available from [LunarG](https://www.lunarg.com/vulkan-sdk/)).  Create the project file like so from the partymod-thps2/build directory:
```
cmake .. -A win32 -DCMAKE_TOOLCHAIN_FILE=C:/[vcpkg directory]/scripts/buildsystems/vcpkg.cmake
```

Maybe set the optimization optimization for the partymod dll to O0 (disable optimization) because MSVC occasionally seems to break certain functions when optimization is enabled.
Additionally, set the SubSystem to "Windows (/SUBSYSTEM:WINDOWS)" in the partyconfig project.

Shaders can be built with [GLSLang](https://github.com/KhronosGroup/glslang). the download is somewhere in there. seems odd that the download to the actual command line utility is so hidden. oh well!
