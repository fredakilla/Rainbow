cmake_minimum_required(VERSION 3.5)

set(RAINBOW_APP_SRC
    src/App.cpp
    src/App.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(RAINBOW_APP_PLATFORM_SRC
        src/main-windows.cpp
        rainbow-app.rc
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(RAINBOW_APP_PLATFORM_SRC
        src/main-linux.cpp
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(RAINBOW_APP_PLATFORM_SRC
        src/main-macos.mm
    )
endif()

include_directories( 
    ../rainbow/src
    ../external-deps/include
)

add_executable(rainbow-app ${RAINBOW_APP_SRC} ${RAINBOW_APP_PLATFORM_SRC})

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
    include_directories( 
        $ENV{VULKAN_SDK}/Include
    )
    target_link_libraries(rainbow-app
        imm32.lib
        version.lib
        imagehlp.lib
        winmm.lib
        xinput.lib
        rainbow
        $<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Debug/rainbow-deps.lib>
        $<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Release/rainbow-deps.lib>
        $ENV{VULKAN_SDK}/Lib/vulkan-1.lib
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DSDL_VIDEO_DRIVER_X11)
    add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    include_directories( 
        $ENV{VULKAN_SDK}/include
    )
    target_link_libraries(rainbow-app
        rainbow
        $ENV{VULKAN_SDK}/lib/libvulkan.so
        ${PROJECT_SOURCE_DIR}/external-deps/lib/linux/x86_64/librainbow-deps.a
        m
        GL
        rt
        dl
        X11
        pthread
        sndio
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
    include_directories( 
        $ENV{HOME}/vulkansdk-macos-1.0.69.0/macOS/include
    )
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(CARBON_LIBRARY Carbon)
    find_library(QUARTZ_LIBRARY Quartz)
    find_library(IOSURFACE_LIBRARY IOSurface)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FORCEFEEDBACK_LIBRARY ForceFeedback)
    find_library(METAL_LIBRARY Metal)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(GAMEKIT_LIBRARY GameKit)
    find_library(OPENAL_LIBRARY OpenAL)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)

    set(FRAMEWORK_LIBRARIES
        ${FOUNDATION_LIBRARY}
        ${CARBON_LIBRARY}
        ${QUARTZ_LIBRARY}
        ${IOSURFACE_LIBRARY}
        ${IOKIT_LIBRARY}
        ${FORCEFEEDBACK_LIBRARY}
        ${METAL_LIBRARY}
        ${METALKIT_LIBRARY}
        ${GAMEKIT_LIBRARY}
        ${OPENAL_LIBRARY}
        ${COREAUDIO_LIBRARY}
        ${AUDIOTOOLBOX_LIBRARY}
    )

    find_library(RAINBOW_DEPS_LIBRARY NAMES rainbow-deps
        HINTS ${PROJECT_SOURCE_DIR}/external-deps/lib/macos/x86_64
    )
    target_link_libraries(rainbow-app
        rainbow
        ${RAINBOW_DEPS_LIBRARY}
        m
        dl
        pthread
        iconv
        ${FRAMEWORK_LIBRARIES}
        $ENV{HOME}/vulkansdk-macos-1.0.69.0/MoltenVK/macOS/MoltenVK.framework
    )
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set_target_properties(rainbow-app PROPERTIES
    OUTPUT_NAME "rainbow-app"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${RAINBOW_APP_SRC} ${RAINBOW_APP_PLATFORM_SRC})
