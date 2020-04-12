## Description
This is a project intended to further refine my skills as a graphics programmer. It uses glad as the openGL API loader.

[Youtube demo](https://www.youtube.com/watch?v=osjRW0AGpv4): Current version of the application. Will update as I go.

![screenshot]

[screenshot]:https://media.giphy.com/media/S9XS5Cv9XkzZ2sO8IP/giphy.gif

## Pre-installation
Before compiling the project, you will need to:
1. Install latest drivers for your GPU
1. Install all OpenGL libraries needed for development
1. (Linux/Ubuntu) Install other related libraries, including X11, dl, pthread, GLFW, and basically anything the compiler asks later...

## Project setup
For this to work, you will need to include several library dependencies.
* Include the 'includes' folder in the root of the directory
* (Linux/Ubuntu) Add the following libraries to the linker, in this order: 
  1. glfw3
  1. pthread
  1. dl
  1. X11
  1. GL
* (Windows) All library paths for VC++ should already be specified in the Visual Studio solution files.

## Compilation
Compile this with a c++ compiler with the libraries listed above. It should run with static libraries (or dynamic, maybe, depending on how the project is set up).
