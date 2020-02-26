## Pre-installation
Before compiling the project, you will need to:
1. Install latest drivers for your GPU
1. Install all OpenGL libraries needed for development
1. Install other related libraries, including X11, dl, pthread, GLFW, and basically anything the compiler asks later...

## Project setup
For this to work, you will need to include several library dependencies.
1. Include the 'includes' folder in the root of the directory
1. Add the following libraries to the linker, in this order: 
  1. glfw3
  1. pthread
  1. dl
  1. X11
  1. GL

  * (this is based on Ubuntu dev environment. The library list might change on another platform)

## Compilation
Compile this with a c++ compiler with the libraries listed above. It should run with static libraries (or dynamic, maybe, depending on how the project is set up).