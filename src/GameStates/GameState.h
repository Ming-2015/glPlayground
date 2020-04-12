#pragma once
#include "../scene/Scene.h"
#include "../components/GameResources.h"

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
    _mScene.prepShaderPrograms(_mResources.shaderProgramManager);
    _onDraw(); 
    _mScene.draw(); 
  };

  virtual void update(float deltaT)
  { 
    _onUpdate(deltaT); 
    _mScene.update(deltaT);
    _mResources.primitiveManager.update(deltaT);
  };
};