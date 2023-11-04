#include <SDL2/SDL_keycode.h>
#include <optional>
#include <iostream>
#include <unordered_set>
#include <SDL2/SDL.h>

#include "params.hpp"

/**
 * @fn void drawPixels( SDL_Renderer* rd, const Game* g)
 * Display each pixels in the screen range
 * 
 * @param[inout] rd, the window randerer from SDL2
 * @param[inout] g, the GAME !! (with all his data)
 *
 * @return void
 */
void drawPixels( SDL_Renderer* rd, const Game& g)
{
  for ( int i = g.origin.x ; i < g.origin.x + W_GRID_H ; i++ )
  {
    for ( int j = g.origin.y ; j < g.origin.y + W_GRID_W ; j++ )
    {
      // Cell alive
      if ( g.alive_set.contains( {(int) i, (int) j} ) )
      {
        SDL_SetRenderDrawColor(rd, P_COLOR_R, P_COLOR_G, P_COLOR_B, 255) ;
      }
      // Grid
      else if ( 
             ( abs(i) + abs(j) )%2 == ( abs(g.origin.x) + abs(g.origin.y) )%2 
          or ( abs(i) - abs(j) )%2 == ( abs(g.origin.x) - abs(g.origin.y) )%2
        )
      {
        SDL_SetRenderDrawColor(rd, GRID_COLOR2_R, GRID_COLOR2_G, GRID_COLOR2_B, 255) ;
      }
      else
      {
        SDL_SetRenderDrawColor(rd, GRID_COLOR1_R, GRID_COLOR1_R, GRID_COLOR1_R, 255) ;
      }
      SDL_Rect rect = { int( j - g.origin.y - 1)*P_SIZE, int( i - g.origin.x - 1 )*P_SIZE, P_SIZE, P_SIZE } ;
      SDL_RenderFillRect(rd, &rect) ;
    }
  }

  // Eraser
  if ( g.mouse.hold and not g.mouse.button )
  {
    SDL_SetRenderDrawColor(rd, 255, 0, 0, 255) ;
    SDL_Rect rect = { int( g.mouse_pos.y - g.origin.y - 2)*P_SIZE, int( g.mouse_pos.x - g.origin.x - 2 )*P_SIZE, 3*P_SIZE, 3*P_SIZE } ;
    SDL_RenderDrawRect(rd, &rect) ;
  }
}

/**
 * @fn int numberNeighbours( std::unordered_set<Coord>* alive_set, const int x, const int y )
 * Count all the alive neighbours around the point (x, y)
 * 
 * @param[inout] alive_set, all the alive_cells
 * @param[in] x, the x coordonate
 * @param[in] y, the y coordonate
 *
 * @return int
 * @retval a number between 0 and 8
 */
int numberNeighbours( const std::unordered_set<Coord>& alive_set, Coord c )
{
  int n = 0 ;
  for ( int i = c.x - 1 ; i < c.x + 2 ; i++ )
  {
    for ( int j = c.y - 1 ; j < c.y + 2 ; j++ )
    {
      if ( i == c.x and j == c.y ) continue ;
      if ( alive_set.contains( {i, j} ) ) n++ ;
    }
  }
  return n ;
}

/**
 * @fn void updateConway( Game& g )
 * Update the game with the Game of Life rules (check out on Wikipedia if don't know them)
 * 
 * @param[inout] g, the GAME !!
 *
 * @return void
 */
void updateConway( Game& g )
{
  if ( g.alive_set.empty() ) g.start = false ;
  else
  {
    // Create the set of all the cells to check
    std::unordered_set<Coord> update_set ;
    for ( Coord c : g.alive_set )
    {
      for ( int x = c.x - 1 ; x < c.x + 2 ; x++ )
      {
        for ( int y = c.y - 1 ; y < c.y + 2 ; y++ )
        {
          update_set.insert( {x, y} ) ;
        }
      }
    }

    // The new set of alive celles
    std::unordered_set<Coord> new_alive_set ;

    // Check birth or death
    for ( Coord c : update_set )
    {
      int n = numberNeighbours(g.alive_set, c) ;
      if ( n == 3 or ( n == 2 and g.alive_set.contains(c) ) ) new_alive_set.insert(c) ;
    }

    if ( g.alive_set == new_alive_set )
    {
      g.start = false ;
    }

    g.alive_set = new_alive_set ;
  }
}

// TODO
void keyListener( Game& g )
{
  SDL_Event evt ;
  while ( SDL_PollEvent(&evt) )
  {
    switch ( evt.type )
    {
      case SDL_QUIT :
      {
        g.quit = true ;
        break ;
      }
      case SDL_KEYDOWN :
      {
        switch ( evt.key.keysym.sym )
        {
          case SDLK_r :
          case SDLK_F2 :
          case SDLK_BACKSPACE :
          {
            g.alive_set.clear() ;
            g.start = false ;
            g.origin.x = 0 ;
            g.origin.y = 0 ;
            break ;
          }
          case SDLK_p :
          case SDLK_ESCAPE :
          {
            g.start = not g.start ;
            break ;
          }
          case SDLK_SPACE :
          case SDLK_RETURN :
          {
            g.start = true ;
            break ;
          }
          case SDLK_z :
          case SDLK_UP :
          {
            g.move.hold_up = true ;
            break ; 
          }
          case SDLK_q :
          case SDLK_LEFT :
          {
            g.move.hold_left = true ;
            break ; 
          }
          case SDLK_s :
          case SDLK_DOWN :
          {
            g.move.hold_down = true ;
            break ; 
          }
          case SDLK_d :
          case SDLK_RIGHT :
          {
            g.move.hold_right = true ;
            break ; 
          }
        }
        break ;
      }
      case SDL_KEYUP :
      {
        switch ( evt.key.keysym.sym )
        {
          case SDLK_z :
          case SDLK_UP :
          {
            g.move.hold_up = false ;
            break ; 
          }
          case SDLK_q :
          case SDLK_LEFT :
          {
            g.move.hold_left = false ;
            break ; 
          }
          case SDLK_s :
          case SDLK_DOWN :
          {
            g.move.hold_down = false ;
            break ; 
          }
          case SDLK_d :
          case SDLK_RIGHT :
          {
            g.move.hold_right = false ;
            break ; 
          }
        }
        break ;
      }
      case SDL_MOUSEBUTTONDOWN :
      {
        g.mouse.hold = true ;
        g.mouse.button = ( evt.button.button == SDL_BUTTON_LEFT ) ;
        g.mouse.cell_type = g.alive_set.contains( {g.mouse_pos.x, g.mouse_pos.y} ) ;
        break ;
      }
      case SDL_MOUSEBUTTONUP :
      {
        g.mouse.hold = false ;

        g.old_mouse_pos = std::nullopt ;
        break ;
      }
      default :
      {
        break ;
      }
    }
  }
}

void refresh( Game& g )
{
  // Pen manager
  if ( not g.start and g.mouse.hold )
  {
    int x, y ;
    SDL_GetMouseState(&x, &y) ;
    g.mouse_pos.x = int(g.origin.x + y/float(P_SIZE)) + 1 ;
    g.mouse_pos.y = int(g.origin.y + x/float(P_SIZE)) + 1 ;

    if ( (not g.old_mouse_pos.has_value()) or g.mouse_pos.x != g.old_mouse_pos.value().x or g.mouse_pos.y != g.old_mouse_pos.value().y ) 
    {
      // Draw if left-click and cell dead
      if ( g.mouse.button and not g.mouse.cell_type ) g.alive_set.insert( g.mouse_pos ) ;
      // Erase if left-click and cell live, or if right-click
      else if ( (g.mouse.button and not g.mouse.cell_type) or not g.mouse.button )
      {
        for ( int i = g.mouse_pos.x-1 ; i < g.mouse_pos.x+2 ; i++ )
        {
          for ( int j = g.mouse_pos.y-1 ; j < g.mouse_pos.y+2 ; j++ )
          {
            g.alive_set.erase( {i, j} ) ;
          }
        }
      }
    }

    // Keep the old_position to not redo again
    *g.old_mouse_pos = g.mouse_pos ;
  }

  // Change the grid displayed
  if ( g.move.hold_up != g.move.hold_down )
  {
    if ( g.move.hold_up ) g.origin.x-- ;
    if ( g.move.hold_down ) g.origin.x++ ;
  }
  if ( g.move.hold_left != g.move.hold_right )
  {
    if ( g.move.hold_left ) g.origin.y-- ;
    if ( g.move.hold_right ) g.origin.y++ ;
  }
}

int main(int argc, char **argv)
{
  // Init SDL windows
  SDL_Init(SDL_INIT_VIDEO) ;

  SDL_Window* wnd = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN) ;
  SDL_Renderer* rd = SDL_CreateRenderer(wnd, -1, 0) ;

  // Init pixel size
  P_SIZE *= 4 ;

  // The GAME !!
  Game* g = new Game() ;

  while ( not g->quit )
  {
    // Clear screen
    SDL_RenderClear(rd) ;

    // Key listener and updater
    keyListener(*g) ;
    refresh(*g) ;

    // Displayer
    drawPixels(rd, *g) ;

    // Update the cell life according to their fate (no)
    if ( g->start )
    {
      updateConway(*g) ;
      SDL_Delay(DELAY) ;
    }
    else
    {
      SDL_Delay(25) ;
    }

    SDL_RenderPresent(rd) ;
  }

  // Adieu !
  free(g) ;
  SDL_DestroyRenderer(rd) ;
  SDL_DestroyWindow(wnd) ;
  SDL_Quit() ;
 
  return 0 ;
}