/*
 * Game.cpp
 *
 *  Created on: Feb 28, 2020
 *      Author: ycai
 */

#include "Game.h"

Game::Game()
  : clearColor(0.2f, 0.2f, 0.3f, 1.0f) {

}

Game::~Game() {

}

void Game::init() {

}

void Game::render() {
  // set the clear color
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

  // clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);
}

void Game::update(float deltaT) {

}
