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
  this->mouse_pos.x = -this->origin.x - int( 0.5 * this->window.grid_width() ) + int ( x / float(this->window.current_p_size) ) ;
  this->mouse_pos.y = this->origin.y - int( 0.5 * this->window.grid_height() ) + int ( y / float(this->window.current_p_size) ) ;
}