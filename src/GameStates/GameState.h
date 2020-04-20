#pragma once
#include "../scene/Scene.h"
#include "../components/GameResources.h"
#include "../scene/Models/Plane.h"

class GameState : public WindowObservable
{
protected:

  GameResources _mResources;
  Scene _mScene;
  bool _mIsLoaded = false;

  // main loop functions - based on each state
  virtual void _onUpdate(float deltaT) = 0;
  virtual void _onDraw() = 0;

  // resource allocation/deallocation - based on each state
  virtual void _onLoad() = 0;
  virtual void _onDestroy() = 0;

  // for drawing out to the window frame buffer
  ScreenShader* screenShader = nullptr;
  Plane* plane = nullptr;

public:

  GameState(const GameResources& resources);
  virtual ~GameState();

  void load();
  void destroy();

  // returns a non-empty string if the next state should be rendered
  virtual std::string nextState() const = 0;
  virtual bool isLoaded() const { return _mIsLoaded; }

  // main loop functions
  void draw();
  void update(float deltaT);

  // override window's resize function
  void onResize(int width, int height) override;
};