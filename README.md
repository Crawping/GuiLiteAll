# Noodle Flip UI framework
Licnesed under MIT license.
## Welcome to NoodleFlipUI repo!

As developer, you maybe hate UI, because it has nothing to do with features. But end user can't use your software without UI.
This repository give you choice to create UI yourself, It's tiny and easy to use.

Besides general UI operations, it also support finger flipping, that means you can use this feature in limited resource hardware.

## Fast look
You get a quickly look for this UI by running it in demo folder:

## How it work
This projcet written by C/C++, it's easy to run in Android/Linux/Windows.
So far, we provide Linux sample code, you can find what it can do by this sample. And we will provide Android/Windows sample code to show how to wrap NooldeFlipUI , then you can run it on Android/Windows.

## How to build/run sample
You can build sample in Ubuntu desktop.

## How to build display
Display is a QT APP, it works as a virtual display panel. When you run Noodle.FlipUI.Sample on Ubuntu, this app will be executed automaticly, then you will see the ouput of sample, and can do touch/clik operations on it.

1. 'Open this source project by QT creator'
2. 'build and run'

Note: 'dispay' and 'Noodle.FlipUI.Sample' should be in same directory.

### Software dependency
1. Ubuntu x64 desktop.
2. QT creator for x64. For "display" is a QT APP, it act as a virtual display panel.

### Build and run
1. cmd: `cd Noodle.FlipUI.Sample`
2. cmd: `mkdir build`
3. cmd: `cd build`
4. cmd: `cmake ../CMakeLists.txt`
5. cmd: `make`
6. cmd: `cd bin`
7. cmd: `sudo./Noodle.FlipUI.Sample N` N means: the number 1-6

##FAQ
Known issues:
### run Noodle.FlipUI.Sample failed:
run again: `sudo./Noodle.FlipUI.Sample N`

#How to build Android
1. install toolchain: sudo apt-get install gcc-arm-linux-androideabi
2. git clean -dxf
3. cmd: `cmake -D CMAKE_C_COMPILER="/usr/bin/arm-linux-androideabi-gcc" -D CMAKE_CXX_COMPILER="/usr/bin/arm-linux-androideabi-g++" .`
4. cmd:`make`