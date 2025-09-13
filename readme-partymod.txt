PARTYMOD for THPS2 1.1

PARTYMOD is a patch for THPS2 (original release) to modernize the game to be played on today's PCs, as well as restore parts of the game that were lost in translation while porting from Playstation.
The patch is designed to keep the game as original as possible, and leave its files unmodified.

Features and Fixes
- Replaces renderer with new bindless Vulkan renderer
- Replaces input system entirely with new, modern system using the SDL2 library
- Improves window handling allowing for custom resolutions and configurable windowing (as well as supporting high DPI scaling)
- Game resolution is now independent from window resolution, allowing the game to run at original 240p or 480p, in addition to window size
- Replaces the gamma correction that resulted in the game looking washed out, instead using PSX-style texture color blending
- Fixes a number of rendering bugs
- Fixes vehicle sounds constantly resetting in NY City and Philadelphia
- Movement stick now controls menus
- Improves cursor handling, only showing it when relevant and using the system cursor to prevent latency
- Replaces configuration files with new INI-based system (see partymod.ini)
- Custom configurator program to handle new configuration files
- Fixes memory manager to prevent memory corruption and crashes related to it
- Restores player control settings menu, allowing players to toggle vibration and autokick settings
- Fixes loading write-protected saves
- An experimental, optional, THPS1 career mode, converting the game to recreate the THPS1 career with THPS2's mechanics. It can be enabled with a setting or by passing -thps1career to THPS2.exe

Installation
1. Download PARTYMOD from the releases tab
2. Make sure THPS2 (Original, not Activision Value or Korean release) is installed
3. Extract this zip file into your THPS2 installation directory
4. Run partypatcher.exe to create the new, patched THPS2.exe game executable (this will be used to launch the game from now on) (this only needs to be done once)
5. Optionally (highly recommended), configure the game with partyconfig.exe
6. Launch the game from THPS2.exe

NOTE: if the game is installed into the "Program Files" directory, you may need to run each program as administrator. 
Also, if the game is installed into the "Program Files" directory, save files will be saved in the C:\Users\<name>\AppData\Local\VirtualStore directory.  
For more information, see here: https://answers.microsoft.com/en-us/windows/forum/all/please-explain-virtualstore-for-non-experts/d8912f80-b275-48d7-9ff3-9e9878954227



Third Party Licenses:



VulkanMemoryAllocator

Copyright (c) 2017-2025 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



Volk

Copyright (c) 2018-2024 Arseny Kapoulkine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



SDL2

Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>
  
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
  
1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required. 
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
