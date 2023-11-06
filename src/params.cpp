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
  this->mouse_pos.x = int( this->origin.x - int ( this->window.grid_height() / 2 ) + y / float(this->window.current_p_size) ) ;
  this->mouse_pos.y = int( this->origin.y - int ( this->window.grid_width() / 2 ) + x / float(this->window.current_p_size) ) ;
}