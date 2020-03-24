#include "GameState.h"

GameState::GameState(const GameResources& resources)
  : _mResources(resources)
{
  _mResources.window.addObservable(this);
}

GameState::~GameState()
{}