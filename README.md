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


## How to install App and run in Raspberry Pi 4

### Install necessary libraries in Raspberry Pi 4 and copy to Host
```bash
sudo apt update

sudo apt-get install -y libsdl2-dev
# Or 
sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

sudo apt install alsa-base alsa-utils

cd ShareLib_pi/pi-sysroot

$rsync -avz pi@pi.local:/lib .
$rsync -avz pi@pi.local:/usr/include usr
$rsync -avz pi@pi.local:/usr/lib usr
```

### Install Toolchaine for Cross Compile
```bash
cd SharedLib_Pi/CrossCompiler
wget https://developer.arm.com/-/media/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu.tar.xz
tar -xf gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu.tar.xz
```

### Cross build for FFmpeg-4.4.4 Library
```bash
cd SharedLib_Pi/FFmpeg
# wget https://ffmpeg.org/releases/ffmpeg-4.4.4.tar.xz
tar -xf ffmpeg-4.4.4.tar.xz
cd ffmpeg-4.4.4
./configure \
  --enable-cross-compile \
  --cross-prefix=/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/CrossCompiler/gcc-arm-11.2-2022.02-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu- \
  --arch=aarch64 \
  --target-os=linux \
  --sysroot=/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot \
  --extra-cflags="-I/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/usr/include -I/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/usr/include/aarch64-linux-gnu" \
  --extra-ldflags="-L/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/usr/lib/aarch64-linux-gnu -L/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/lib/aarch64-linux-gnu -B/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/usr/lib/aarch64-linux-gnu -Wl,--rpath-link=/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/lib/aarch64-linux-gnu -Wl,--rpath-link=/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/pi-sysroot/usr/lib/aarch64-linux-gnu" \
  --enable-alsa \
  --enable-static \
  --enable-shared \
  --prefix=/home/bmo/Documents/MusicVisualizerV2/SharedLib_Pi/FFmpeg/ffmpeg-build
make -j$(nproc)
make install
```

### Run the Application in Raspberry Pi 4
```bash
mkdir -p deploy/lib
cp build/musicvisualizer deploy/
cp -a SharedLib_Pi/FFmpeg/ffmpeg-build/lib/*.so* deploy/lib/
scp -r deploy/ pi@172.20.10.2:~/musicvisualizer/

# Testing IP address of Raspberry Pi 4
ping pi.local

ssh pi@pi.local
cd musicvisualizer/deloy
export LD_LIBRARY_PATH="$PWD/lib:$LD_LIBRARY_PATH"
sudo apt install patchelf
patchelf --set-interpreter /lib/ld-linux-aarch64.so.1 musicvisualizer
chmod +x musicvisualizer
./musicvisualizer
```
