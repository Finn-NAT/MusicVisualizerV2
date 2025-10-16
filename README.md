# Music Visualizer V2

## OverView
I used FFmpeg and Lvgl for my Porject

## Required Libraries and How to install and run in Ubuntu
This project does **not** use `sudo apt-get install` for dependencies. All libraries are built and linked manually for portability and compatibility.

### Dependencies
- FFmpeg version 4.4.4 (For getting Audio)
- kissFFT (should be version 131.1.0) (For processing Audio)
- LVGL version 8.3 (For Visualizing Data)


**Note:** Before installing any library, set your project path to the $PROJECT_DIR environment variable.
```bash
export PROJECT_DIR="<your-project-path>" # "/home/bmo/Documents/MusicVisualizerV2"
# You can double check by
echo $PROJECT_DIR
```

### FFmpeg Library
```bash
cd SharedLib/FFmpeg
# wget https://ffmpeg.org/releases/ffmpeg-4.4.4.tar.xz
tar -xf ffmpeg-4.4.4.tar.xz
cd ffmpeg-4.4.4
./configure --enable-shared --enable-static --disable-x86asm --prefix=$PROJECT_DIR/LibProject/ffmpeg-build
make -j$(nproc)
make install
```

### KissFFT Library
```bash
git clone https://github.com/mborgerding/kissfft.git
# Or you could download version 131.1.0 from release in https://github.com/mborgerding/kissfft.git
```

## How to Clone the Project
First, you clone project like normal, and after that:
```bash
cd <project-name>
git submodule update --init --recursive
cd Graphic/lvgl
git checkout v8.3.11
```
Before you run the project, just config the path in lvgl and lv_drivers
```c
// From
#include "lvgl/lvgl.h"
// To
#include "../../lvgl/lvgl.h"     
// In file: sdl.h and sdl_common.h
```

## Run the Application
```bash
# If you haven't installed SDL2, just install it
sudo apt-get install -y libsdl2-dev
# Or 
sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

mkdir build
cd build
cmake .. 
make -j$(nproc)
./musicvisualizer 
```