Windows 10 - Setup
------------------
- Install Windows 10 SDK (10.0.15063.xxx)    (https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)
- Install Visual Studio 2015 + Update 3      (https://www.visualstudio.com/vs/older-downloads/)
- Install Vulkan SDK                         (https://vulkan.lunarg.com/sdk/home)
- Install Qt 5.10 (open-source) to C:\Qt)    (https://www.qt.io/download)
- Set QTDIR environment variable             (QTDIR=C:\Qt\5.10.1\msvc2015_64)
- Clone Rainbow repo
- Run install.bat

Windows 10 - Build with Qt Creator (Option A)
----------------------------------------------
- Run Qt Creator
- Open rainbow-all.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 MSVC2015 64bit"
- Uncheck Profile and change long build paths for Debug and Release to:
```
Debug = build/%{CurrentProject:Name}/Debug
Release = build/%{CurrentProject:Name}/Release
```
- Build and Run

Windows 10 - Build with CMake and Visual Studio 2015 (Option B)
---------------------------------------------------------------
- Install CMake                             (https://cmake.org/download/)
- Open Visual Studio 2015
- Open menu Tools->Extensions and Updates...
- Search Online for "Qt Visual Studio Tools" and install extension.
- Restart Visual Studio 2015
- Open menu Qt VS Tools->Qt Options
- Add Path=C:\Qt\Qt5.10.1\msvc2015_64 and Name=msvc2015_64
- Run generate-projects.bat
- Open build\Rainbow.sln
- Open menu Qt VS Tools->Open Qt Project File(.pro)...
- Select rainbow-editor\rainbow-editor.pro
- Right-click rainbow-editor->Set as Startup Project
- Build and Run

Ubuntu 16.04 - Setup
--------------------
- Install Build SDKs                        (sudo apt-get install build-essential clang gcc g++ curl cmake)
- Install Platform SDKs                     (sudo apt-get install libx11-xcb-dev libgtk2.0-dev libogg-dev libopenal-dev 
libtool libasound2-dev  libpulse-dev libaudio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev libesd0-dev libsndio-dev)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home)
- Add the following to user profile         (vi ~/.profile)
```
export VULKAN_SDK=~/VulkanSDK/1.1.70.0/x86_64 
export PATH=$VULKAN_SDK/bin:$PATH
export LD_LIBRARY_PATH=$VULKAN_SDK/lib:$LD_LIBRARY_PATH
export VK_LAYER_PATH=$VULKAN_SDK/etc/explicit_layer.d
```
- Reboot computer
- Clone Rainbow repo
- Run install.sh

Ubuntu 16.04 - Build with Qt Creator
------------------------------------
- Install Qt 5.10 (open-source) to ~/Qt     (https://www1.qt.io/download)
- Run Qt Creator
- Open rainbow-all.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 GCC 64bit"
- Uncheck Profile and change long build paths for Debug and Release to:
```
Debug   = build/%{CurrentProject:Name}/Debug
Release = build/%{CurrentProject:Name}/Release
```
- Build and Run

MacOS High Sierra - Setup
-------------------------
- Install Xcode 9                           (https://developer.apple.com/xcode/)
- Install Xcode Command Line Tools          (https://developer.apple.com/download/more/)
- Install Vulkan SDK                        (https://vulkan.lunarg.com/sdk/home)
- Extract vulkansdk-macos-1.0.69.0.tar.gz to ~/vulkansdk-macos-1.0.69.0
- Add the following to user profile         (vi ~/.profile)
```
export VULKAN_SDK="~/vulkansdk-macos-1.0.69.0/macOS"
export VK_ICD_FILENAMES=$VULKAN_SDK/etc/vulkan/icd.d/MoltenVK_icd.json
```
- Reboot computer
- Clone Rainbow repo
- Run install.sh

MacOS High Sierra - Build with Qt Creator
-----------------------------------------
- Install Qt 5.10 (open-source) to ~/Qt     (https://www1.qt.io/download)
- Open rainbow-all.pro
- Expand "Configure Project" kits by expanding "Desktop Qt 5.10.1 clang 64bit"
- Uncheck Profile and change long build paths for Debug and Release to:
```
Debug   = build/%{CurrentProject:Name}/Debug
Release = build/%{CurrentProject:Name}/Release
```
- Build and Run
