#include <SDL2/SDL.h>

#include "params.hpp"

bool Coord::operator==(const Coord& c) const
{
  return c.x == x and c.y == y ;
}


void Game::updateMousePos()
{
  int x, y ;
  SDL_GetMouseState(&x, &y) ;
  this->mouse_pos.x = int(this->origin.x + y/float(this->window.p_size)) + 1 ;
  this->mouse_pos.y = int(this->origin.y + x/float(this->window.p_size)) + 1 ;
}