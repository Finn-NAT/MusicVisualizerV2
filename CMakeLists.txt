cmake_minimum_required(VERSION 3.10)
project(MusicVisualizer)

# Set standard C
set(CMAKE_C_STANDARD 99)

# Path to FFmpeg and Kiss FFT
set(FFMPEG_ROOT "${CMAKE_SOURCE_DIR}/LibProject/ffmpeg-build")
set(KISSFFT_ROOT "${CMAKE_SOURCE_DIR}/kissfft")

# Thêm include directories
include_directories(
    ${FFMPEG_ROOT}/include
    ${KISSFFT_ROOT}
)

include_directories(Graphic)
include_directories(MusicProcessor)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

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
    message(FATAL_ERROR "libavformat không tìm thấy trong ${FFMPEG_ROOT}/lib")
endif()

if(NOT AVCODEC_LIB)
    message(FATAL_ERROR "libavcodec không tìm thấy trong ${FFMPEG_ROOT}/lib")
endif()

if(NOT AVUTIL_LIB)
    message(FATAL_ERROR "libavutil không tìm thấy trong ${FFMPEG_ROOT}/lib")
endif()

if(NOT AVDEVICE_LIB)
    message(FATAL_ERROR "libavdevice không tìm thấy trong ${FFMPEG_ROOT}/lib")
endif()

# Setup Kiss FFT library
add_library(kissfft STATIC
    ${KISSFFT_ROOT}/kiss_fft.c
    ${KISSFFT_ROOT}/kiss_fftr.c
)

target_include_directories(kissfft PUBLIC ${KISSFFT_ROOT})

file(GLOB_RECURSE GRAPHIC_SOURCES Graphic/*.c)
file(GLOB_RECURSE MUSICPROCESSOR_SOURCES MusicProcessor/*.c)

# Setup executable
add_executable(musicvisualizer main.cpp ${GRAPHIC_SOURCES} ${MUSICPROCESSOR_SOURCES})

# Links to libraries
target_link_libraries(musicvisualizer
    kissfft
    ${AVFORMAT_LIB}
    ${AVCODEC_LIB}
    ${AVUTIL_LIB}
    ${AVDEVICE_LIB}
    ${SWRESAMPLE_LIB}
    pthread
    m
    z
    SDL2 
    SDL2main
)

# Set RPATH to find shared libraries
set_target_properties(musicvisualizer PROPERTIES
    INSTALL_RPATH "${FFMPEG_ROOT}/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
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
