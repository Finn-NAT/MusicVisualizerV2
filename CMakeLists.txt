cmake_minimum_required(VERSION 3.10)

# Cross-compile settings for Raspberry Pi (MUST be before project())
set(CROSS_COMPILE_PREFIX "/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/CrossCompiler/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu")
set(PI_SYSROOT "/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot")
set(FFMPEG_ROOT "/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/FFmpeg/ffmpeg-build")

# Set toolchain BEFORE project()
set(CMAKE_C_COMPILER "${CROSS_COMPILE_PREFIX}/bin/aarch64-none-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${CROSS_COMPILE_PREFIX}/bin/aarch64-none-linux-gnu-g++")

project(MusicVisualizer)

# Set standard C
set(CMAKE_C_STANDARD 99)

# Set sysroot and compile flags
set(CMAKE_SYSROOT "${PI_SYSROOT}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${PI_SYSROOT}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdinc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${PI_SYSROOT}/usr/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${PI_SYSROOT}/usr/include/aarch64-linux-gnu")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${CROSS_COMPILE_PREFIX}/lib/gcc/aarch64-none-linux-gnu/11.2.1/include")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isystem ${CROSS_COMPILE_PREFIX}/lib/gcc/aarch64-none-linux-gnu/11.2.1/include-fixed")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${PI_SYSROOT}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PI_SYSROOT}/usr/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PI_SYSROOT}/usr/include/aarch64-linux-gnu")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PI_SYSROOT}/usr/include/c++/10")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PI_SYSROOT}/usr/include/c++/10/aarch64-linux-gnu")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${CROSS_COMPILE_PREFIX}/lib/gcc/aarch64-none-linux-gnu/11.2.1/include")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${CROSS_COMPILE_PREFIX}/lib/gcc/aarch64-none-linux-gnu/11.2.1/include-fixed")

# Set linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${PI_SYSROOT}/usr/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${PI_SYSROOT}/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -B${PI_SYSROOT}/usr/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--rpath-link=${PI_SYSROOT}/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--rpath-link=${PI_SYSROOT}/usr/lib/aarch64-linux-gnu")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--dynamic-linker=${PI_SYSROOT}/lib/ld-linux-aarch64.so.1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--allow-multiple-definition")

message(STATUS "Cross-compiling for Raspberry Pi")
message(STATUS "Using toolchain: ${CROSS_COMPILE_PREFIX}")
message(STATUS "Using sysroot: ${PI_SYSROOT}")

# Add include directories
include_directories(
    ${FFMPEG_ROOT}/include
)

include_directories(Graphic)
include_directories(MusicProcessor)

# For Pi cross-compilation, use SDL2 from sysroot
message(STATUS "Cross-compiling: Using SDL2 from sysroot")
include_directories(${PI_SYSROOT}/usr/include/SDL2)
set(SDL2_LIBRARIES SDL2 SDL2main)


# Add library directories  
link_directories(
    ${FFMPEG_ROOT}/lib
)

# Set direct path to static FFmpeg libraries
find_library(AVFORMAT_LIB avformat PATHS ${FFMPEG_ROOT}/lib NO_DEFAULT_PATH)
find_library(AVCODEC_LIB avcodec PATHS ${FFMPEG_ROOT}/lib NO_DEFAULT_PATH)
find_library(AVUTIL_LIB avutil PATHS ${FFMPEG_ROOT}/lib NO_DEFAULT_PATH)
find_library(AVDEVICE_LIB avdevice PATHS ${FFMPEG_ROOT}/lib NO_DEFAULT_PATH)
find_library(SWRESAMPLE_LIB swresample PATHS ${FFMPEG_ROOT}/lib NO_DEFAULT_PATH)

# Check if libraries exist
if(NOT AVFORMAT_LIB)
    message(FATAL_ERROR "libavformat not found in ${FFMPEG_ROOT}/lib")
endif()

if(NOT AVCODEC_LIB)
    message(FATAL_ERROR "libavcodec not found in ${FFMPEG_ROOT}/lib")
endif()

if(NOT AVUTIL_LIB)
    message(FATAL_ERROR "libavutil not found in ${FFMPEG_ROOT}/lib")
endif()

if(NOT AVDEVICE_LIB)
    message(FATAL_ERROR "libavdevice not found in ${FFMPEG_ROOT}/lib")
endif()


# target_include_directories(kissfft PUBLIC ${KISSFFT_ROOT})

file(GLOB_RECURSE GRAPHIC_SOURCES Graphic/*.c)
list(FILTER GRAPHIC_SOURCES EXCLUDE REGEX ".*/lvgl\\.remote/.*")
list(FILTER GRAPHIC_SOURCES EXCLUDE REGEX ".*/lv_drivers\\.remote/.*")
file(GLOB_RECURSE MUSICPROCESSOR_SOURCES MusicProcessor/*.c)

# Setup executable
add_executable(musicvisualizer main.cpp ${GRAPHIC_SOURCES} ${MUSICPROCESSOR_SOURCES})

# Cross-compile libraries for Pi - use sysroot libraries  
target_link_libraries(musicvisualizer
    ${AVFORMAT_LIB}
    ${AVCODEC_LIB}
    ${AVUTIL_LIB}
    ${AVDEVICE_LIB}
    ${SWRESAMPLE_LIB}
    ${PI_SYSROOT}/usr/lib/aarch64-linux-gnu/libasound.so.2.0.0
    ${PI_SYSROOT}/usr/lib/aarch64-linux-gnu/libSDL2-2.0.so.0.14.0  
    ${PI_SYSROOT}/usr/lib/aarch64-linux-gnu/libSDL2main.a
    ${PI_SYSROOT}/usr/lib/aarch64-linux-gnu/libpthread-2.31.so
    ${PI_SYSROOT}/usr/lib/gcc/aarch64-linux-gnu/10/libstdc++.a
    -Wl,--no-as-needed
    -static-libstdc++
    -Wl,--as-needed
    -Wl,--allow-shlib-undefined
    m
    dl
    z
)


# Compiler flags
target_compile_options(musicvisualizer PRIVATE -Wall -Wextra -O2)

# Print debug information
message(STATUS "FFmpeg root: ${FFMPEG_ROOT}")
message(STATUS "Kiss FFT root: ${KISSFFT_ROOT}")
message(STATUS "AVFormat lib: ${AVFORMAT_LIB}")
message(STATUS "AVCodec lib: ${AVCODEC_LIB}")
message(STATUS "AVUtil lib: ${AVUTIL_LIB}")
message(STATUS "AVDevice lib: ${AVDEVICE_LIB}")
