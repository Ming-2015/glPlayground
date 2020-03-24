#pragma once
#include "../components/Shader.h"
#include "../components/ShaderProgram.h"
#include "../components/Texture.h"
#include "../components/Primitive.h"
#include "../components/Window.h"
#include "../scene/Scene.h"

struct GameResources {
  ShaderManager& shaderManager;
  ShaderProgramManager& shaderProgramManager;
  TextureManager& textureManager;
  PrimitiveManager& primitiveManager;
  Window& window;

  GameResources(
    ShaderManager& shaderManager,
    ShaderProgramManager& shaderProgramManager,
    TextureManager& textureManager,
    PrimitiveManager& primitiveManager,
    Window& window
  ) : shaderManager(shaderManager),
    shaderProgramManager(shaderProgramManager),
    textureManager(textureManager),
    primitiveManager(primitiveManager),
    window(window)
  {}

  GameResources(const GameResources& other)
    : shaderManager(other.shaderManager),
    shaderProgramManager(other.shaderProgramManager),
    textureManager(other.textureManager),
    primitiveManager(other.primitiveManager),
    window(other.window)
  {}
};

class GameState : public WindowObservable
{
protected:

  GameResources _mResources;
  Scene _mScene;
  bool _mIsLoaded = false;

  virtual void _onUpdate(float deltaT) = 0;
  virtual void _onDraw() = 0;

  // resouce allocation/deallocation
  virtual void _onLoad() = 0;
  virtual void _onDestroy() = 0;

public:

  GameState(const GameResources& resources);
  virtual ~GameState();

  virtual void load() 
  {
    if (_mIsLoaded) return;

    _onLoad();
    _mIsLoaded = true;
  }

  virtual void destroy()
  {
    if (!_mIsLoaded) return;

    _onDestroy();
    _mIsLoaded = false;
  }

  // returns a non-empty string if the next state should be rendered
  virtual const std::string& nextState() const = 0;
  virtual bool isLoaded() const { return _mIsLoaded; }

  // main loop functions
  virtual void draw() 
  { 
    _onDraw(); 
    _mScene.draw(); 
  };

  virtual void update(float deltaT)
  { 
    _onUpdate(deltaT); 
    _mScene.update(deltaT); 
  };
};