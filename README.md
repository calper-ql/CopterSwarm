# CopterSwarm
This is a project for building swarm systems that lift heavy weights that an idividual agent could not lift by itself.

We aim to make our simulation as realistic as possible to give trained agents the ability to function in real world situations.

## Prerequisites
#### Windows:
We use MinGW compiler  
You need to download and put GLFW binaries into the glfw folder  
You can find them here: https://www.glfw.org/download.html  
Specifically you need to copy the folder named lib-mingw-w64 in glfw  
cmake looks for (project-root)/glfw/lib-mingw-w64/libglfw3.a

#### Ubuntu:
```
sudo apt update
sudo apt install build-essential libglfw3 cmake cmake-curses-gui
```
#### OSX:
Make sure you have homebrew installed
```
brew install glfw cmake
```
## How to compile

```
mkdir build && cd build
cmake ..
make
```
I highly suggest use of ccmake instead of cmake. However, if it comes to it you can use these commands instead.

If you want to compile without the renderer then do:
```
cmake -DBUILD_WITH_GL=OFF ..
```

If you want to compile in release mode (fast LOL) then do:
```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

if you successfully compile, you sould have bunch of executable files in your directory...