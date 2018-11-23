cmake_minimum_required(VERSION 3.0)

project(rainbow-deps)

# set cmake default output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
if(WINDOWS)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endif()

# set output directories

set(OUT_DIR_ROOT "${PROJECT_SOURCE_DIR}/../external-deps")

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT ANDROID)
    SET(LINUX 1)
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND NOT ANDROID)
if(NOT ANDROID AND NOT IOS)
    include(${PROJECT_SOURCE_DIR}/cmake/TargetArch.cmake)
    set(ARCH "unknown")
    target_architecture(ARCH)
endif(NOT ANDROID AND NOT IOS)
set(CMAKE_INSTALL_PREFIX "${OUT_DIR_ROOT}")
set(OUT_DIR_INCLUDE "${OUT_DIR_ROOT}/include")
if(NOT ANDROID AND NOT IOS)
    IF(APPLE)
        set(LIBRARY_OUTPUT_PATH "${OUT_DIR_ROOT}/lib/macos/${ARCH}")
    ELSE()
        string(TOLOWER ${CMAKE_SYSTEM_NAME} LOWER_SYSTEM_NAME)
        set(LIBRARY_OUTPUT_PATH "${OUT_DIR_ROOT}/lib/${LOWER_SYSTEM_NAME}/${ARCH}")
    ENDIF(APPLE)
endif(NOT ANDROID AND NOT IOS)
IF(NOT WIN32)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF(NOT WIN32)
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})
message(STATUS "Directory: ${LIBRARY_OUTPUT_PATH}")


# bgfx options
set( BGFX_BUILD_TOOLS OFF CACHE BOOL "Build bgfx tools." FORCE )
set( BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Build bgfx examples." FORCE )
set( BGFX_INSTALL OFF CACHE BOOL "Create installation target." FORCE )
set( BGFX_INSTALL_EXAMPLES OFF CACHE BOOL "Install examples and their runtimes." FORCE )
set( BGFX_CUSTOM_TARGETS OFF CACHE BOOL "Include convenience custom targets." FORCE )
set( BGFX_USE_OVR OFF CACHE BOOL "Build with OVR support." FORCE )
set( BGFX_AMALGAMATED OFF CACHE BOOL "Amalgamated bgfx build for faster compilation" FORCE )
set( BX_AMALGAMATED OFF CACHE BOOL "Amalgamated bx build for faster compilation" FORCE )
set( BGFX_CONFIG_DEBUG ON CACHE BOOL "Enables debug configuration on all builds" FORCE )
set( BGFX_USE_DEBUG_SUFFIX OFF CACHE BOOL "Add 'd' suffix to debug output targets" FORCE )


# add subdirectory

if(LINUX)
    # openal as static
    set(LIBTYPE STATIC)
endif()

if(LINUX OR WINDOWS)    
    set( SDL_SHARED_ENABLED_BY_DEFAULT OFF )    # SDL2
    add_subdirectory(sdl)
    add_subdirectory(openal)
endif()

add_subdirectory(bgfx-cmake)
add_subdirectory(libjson)
include(${PROJECT_SOURCE_DIR}/cmake/imgui.cmake)

set(BUILD_TARGETS
    json
    bgfx
    bimg
    bx
    brtshaderc
    imgui
)

# Emscripten ships with its own implementation of the OpenAL, sdl2, freetype
if(NOT EMSCRIPTEN)
    list(APPEND BUILD_TARGETS SDL2-static)
    list(APPEND BUILD_TARGETS SDL2main)
    list(APPEND BUILD_TARGETS OpenAL)
endif()



# amalgamate libraries

if(LINUX)
    set(RAINBOW_DEPS librainbow-deps.a)
    set(TARGET_CMD ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gnu-amalgamate.sh
        ${CMAKE_AR}
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${LIBRARY_OUTPUT_PATH}
    )
elseif(EMSCRIPTEN)
    set(RAINBOW_DEPS librainbow-deps.a)
    set(TARGET_CMD ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gnu-amalgamate.sh
        $(EMSCRIPTEN)/emar
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${LIBRARY_OUTPUT_PATH}
    )
elseif(WINDOWS)
    set(RAINBOW_DEPS rainbow-deps.lib)
    set(TARGET_CMD ${CMAKE_CURRENT_SOURCE_DIR}/cmake/msvc-amalgamate.bat
        ${CMAKE_AR}
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${LIBRARY_OUTPUT_PATH}
    )
endif()

add_custom_command(
    OUTPUT ${RAINBOW_DEPS}
    COMMAND ${TARGET_CMD}
    #DEPENDS ${BUILD_TARGETS}
    COMMENT "Amalgamate external dependencies..."
)

add_custom_target(rainbow-deps ALL DEPENDS ${RAINBOW_DEPS})
add_dependencies(rainbow-deps ${BUILD_TARGETS})

SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${LIBRARY_OUTPUT_PATH}/${RAINBOW_DEPS})





# helper to copy include files to output directory
FUNCTION(COPY_HEADERS src dst)
    file(COPY ${src}
        DESTINATION ${dst} FILES_MATCHING REGEX "^.*.(h|hpp|inl)$"
        PATTERN ".svn" EXCLUDE
        PATTERN "CMakeFiles" EXCLUDE)
ENDFUNCTION(COPY_HEADERS)


# copy thirdparty headers to output directory

COPY_HEADERS(libjson/ ${OUT_DIR_INCLUDE}/libjson/)
COPY_HEADERS(openal/include/AL/ ${OUT_DIR_INCLUDE}/AL/)
COPY_HEADERS(sdl/include/ ${OUT_DIR_INCLUDE}/sdl/)
COPY_HEADERS(gli/ ${OUT_DIR_INCLUDE}/gli/)
COPY_HEADERS(glm/ ${OUT_DIR_INCLUDE}/glm/)
COPY_HEADERS(imgui/ ${OUT_DIR_INCLUDE}/imgui/)

# copy thirdparty bgfx stuff to output directory

COPY_HEADERS(bgfx-cmake/bgfx/include/ ${OUT_DIR_INCLUDE}/)
COPY_HEADERS(bgfx-cmake/bimg/include/ ${OUT_DIR_INCLUDE}/)
COPY_HEADERS(bgfx-cmake/bx/include/ ${OUT_DIR_INCLUDE}/)
COPY_HEADERS(bgfx-cmake/brtshaderc/brtshaderc.h ${OUT_DIR_INCLUDE}/brtshaderc/)

if(WINDOWS)
    COPY_HEADERS(bgfx-cmake/bx/include/compat/ ${OUT_DIR_INCLUDE}/bx/compat/)
endif()