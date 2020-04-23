## Description
This is a project intended to further refine my skills as a graphics programmer. It uses glad as the openGL API loader.

[Youtube demo](https://youtu.be/lxAIr3ynAJU): Current version of the application. Will update as I go.

![screenshot]

[screenshot]: https://i.imgur.com/ArgXd4H.png

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

## Running
To run this program, you will need the assets used for testing. Please email me if you need the assets, or you can head over to GameStates/TestTriangle.cpp and swap out the assets loaded with your own assets.

## Done
* Phong shader
* Scene graph
* Simple Camera
* Simple Point Lights and Direct Lights
* Custom SRGB FrameBuffer to render to
* Asset importing pipeline via Assimp
* Skin/Skeletal animation

## Next steps
* Write compiled 'Effects' which the Materials link to (a step between ShaderProgram and Material)
* Queue render draw calls and sort/optimize them (make sure translucent objects are going last)
* Skybox
* Shadow maps
* Hemispheric lighting
* Height mapping
* PBR shaders
* Water shaders
* Colliders
* Frustrum culling
* Morph target animation

## Next next steps
* IMGUI
* Some kind of font rendering system
* DoF and Bloom
* Physics (external engine or custom?)
* Procedural generation
* Store/send bone matrices with a texture
* Particle system
* Runtime-compiled Effects based on certain parameters (number of lights, using skinning matrices, etc), to get rid of conditionals based on Uniforms
* Screenspace effects - blur
* AI controllers
