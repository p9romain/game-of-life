#include <vector>
#include <SDL2/SDL.h>

#include "params.hpp"

/**
 * @fn void drawPixels( SDL_Renderer* rd, const Game* g)
 * Display each pixels in the screen range
 * 
 * @param[out] rd, the window randerer from SDL2
 * @param[out] g, the GAME !! (with all his data)
 *
 * @return void
 */
void drawPixels( SDL_Renderer* rd, const Game* g)
{
  for ( std::size_t i = (*g).origin.x ; i < (*g).origin.x + W_GRID_H ; i++ )
  {
    for ( std::size_t j = (*g).origin.y ; j < (*g).origin.y + W_GRID_W ; j++ )
    {
      // Cell alive
      if ( (*g).grid.at(i).at(j) )
      {
        SDL_SetRenderDrawColor(rd, P_COLOR_R, P_COLOR_G, P_COLOR_B, 255) ;
      }
      // Grid
      else if ( 
             ( ( i + j ) - ( (*g).origin.x + (*g).origin.y ) )%2 == 0 
          or ( ( i - j ) - ( (*g).origin.x - (*g).origin.y ) )%2 == 0 
        )
      {
        SDL_SetRenderDrawColor(rd, GRID_COLOR2_R, GRID_COLOR2_G, GRID_COLOR2_B, 255) ;
      }
      else
      {
        SDL_SetRenderDrawColor(rd, GRID_COLOR1_R, GRID_COLOR1_R, GRID_COLOR1_R, 255) ;
      }
      SDL_Rect rect = { int( j - (*g).origin.y - 1)*P_SIZE, int( i - (*g).origin.x - 1 )*P_SIZE, P_SIZE, P_SIZE } ;
      SDL_RenderFillRect(rd, &rect) ;
    }
  }

  // Eraser
  if ( (*g).mouse.hold and not (*g).mouse.cell_type )
  {
    SDL_SetRenderDrawColor(rd, 255, 0, 0, 255) ;
    SDL_Rect rect = { int( (*g).pos.y - (*g).origin.y - 2)*P_SIZE, int( (*g).pos.x - (*g).origin.x - 2 )*P_SIZE, 3*P_SIZE, 3*P_SIZE } ;
    SDL_RenderDrawRect(rd, &rect) ;
  }
}

/**
 * @fn bool validNeighbour( const std::vector<std::vector<bool>>* grid, const int i, const int j )
 * Check if the point (i, j) is in the grid
 * 
 * @param[out] grid, all the cells
 * @param[in] i, the x coordonate
 * @param[in] j, the y coordonate
 *
 * @return bool
 * @retval true if (i, j) is in the grid, false else
 */
bool validNeighbour( const std::vector<std::vector<bool>>* grid, const int i, const int j )
{
  return not ( i < 0 or i >= (*grid).size() or j < 0 or j >= (*grid).at(0).size() ) ;
}

/**
 * @fn int numberNeighbours( const std::vector<std::vector<bool>>* grid, const int x, const int y )
 * Count all the alive neighbours around the point (x, y)
 * 
 * @param[out] grid, all the cells
 * @param[in] x, the x coordonate
 * @param[in] y, the y coordonate
 *
 * @return int
 * @retval a number between 0 and 8
 */
int numberNeighbours( const std::vector<std::vector<bool>>* grid, const int x, const int y )
{
  int n = 0 ;
  for ( int i = x - 1 ; i < x + 2 ; i++ )
  {
    for ( int j = y - 1 ; j < y + 2 ; j++ )
    {
      if ( i == x and j == y ) continue ;
      if ( validNeighbour(grid, i, j) ) n += (*grid).at(i).at(j) ;
    }
  }
  return n ;
}

Game update( const Game* g )
{
  Game res = *g ;
  bool b ;
  for ( std::size_t i = 0 ; i < ((*g).grid).size() ; i++ )
  {
    for ( std::size_t j = 0 ; j < ((*g).grid).at(i).size() ; j++ )
    {
      int n = numberNeighbours(&(*g).grid, i, j) ;
      if ( ((*g).grid).at(i).at(j) )
      {
        if ( not ( n == 2 or n == 3 ) ) b = false ;
        else b = true ;
      }
      else
      {
        if ( n == 3 ) b = true ;
        else b = false ;
      }
      res.grid[i][j] = b ;
    }
  }
  return res ;
}

void reset(std::vector<std::vector<bool>>* grid)
{
  for ( std::size_t i = 0 ; i < (*grid).size() ; i++ )
  {
    for ( std::size_t j = 0 ; j < (*grid).at(i).size() ; j++ )
    {
      (*grid)[i][j] = false ;
    }
  }
}

bool isEmpty(std::vector<std::vector<bool>>* grid)
{
  for ( std::size_t i = 0 ; i < (*grid).size() ; i++ )
  {
    for ( std::size_t j = 0 ; j < (*grid).at(i).size() ; j++ )
    {
      if ( (*grid).at(i).at(j) ) return false ;
    }
  }
  return true ;
}

// TODO
void actionKeys( Game* g )
{
  SDL_Event evt ;
  while ( SDL_PollEvent(&evt) )
  {
    if ( evt.type == SDL_QUIT ) (*g).quit = true ;
    else if ( evt.type == SDL_KEYDOWN ) 
    {
      if ( evt.key.keysym.sym == SDLK_r or evt.key.keysym.sym == SDLK_F2 or evt.key.keysym.sym == SDLK_BACKSPACE )
      {
        reset(&(*g).grid) ;
        (*g).start = false ;
        (*g).origin.x = SIZE/2*W_GRID_H ;
        (*g).origin.y = SIZE/2*W_GRID_W ;
      }
      if ( evt.key.keysym.sym == SDLK_p or evt.key.keysym.sym == SDLK_ESCAPE ) (*g).start = not (*g).start ;
      else if ( evt.key.keysym.sym == SDLK_SPACE or evt.key.keysym.sym == SDLK_RETURN ) (*g).start = true ;

      if ( evt.key.keysym.sym == SDLK_z or evt.key.keysym.sym == SDLK_UP ) (*g).move.hold_up = true ;
      else if ( evt.key.keysym.sym == SDLK_s or evt.key.keysym.sym == SDLK_DOWN ) (*g).move.hold_down = true ;
      if ( evt.key.keysym.sym == SDLK_q or evt.key.keysym.sym == SDLK_LEFT ) (*g).move.hold_left = true ;
      else if ( evt.key.keysym.sym == SDLK_d or evt.key.keysym.sym == SDLK_RIGHT ) (*g).move.hold_right = true ;
    }
    else if ( evt.type == SDL_KEYUP )
    {
      if ( evt.key.keysym.sym == SDLK_z or evt.key.keysym.sym == SDLK_UP ) (*g).move.hold_up = false ;
      else if ( evt.key.keysym.sym == SDLK_s or evt.key.keysym.sym == SDLK_DOWN ) (*g).move.hold_down = false ;
      if ( evt.key.keysym.sym == SDLK_q or evt.key.keysym.sym == SDLK_LEFT ) (*g).move.hold_left = false ;
      else if ( evt.key.keysym.sym == SDLK_d or evt.key.keysym.sym == SDLK_RIGHT ) (*g).move.hold_right = false ;
    }
    else if ( evt.type == SDL_MOUSEBUTTONDOWN ) 
    {
      (*g).pos.x = int((*g).origin.x + evt.button.y/float(P_SIZE)) + 1 ;
      (*g).pos.y = int((*g).origin.y + evt.button.x/float(P_SIZE)) + 1 ;
      (*g).mouse.cell_type = not (*g).grid[(*g).pos.x][(*g).pos.y] ;
      (*g).mouse.hold = true ;
    }
    else if ( evt.type == SDL_MOUSEBUTTONUP ) 
    {
      (*g).mouse.hold = false ;
      (*g).old_pos.x = -1 ;
      (*g).old_pos.y = -1 ;
    }

    if ( not (*g).start and (*g).mouse.hold )
    {
      (*g).pos.x = int((*g).origin.x + evt.button.y/float(P_SIZE)) + 1 ;
      (*g).pos.y = int((*g).origin.y + evt.button.x/float(P_SIZE)) + 1 ;
      if ( evt.button.button == SDL_BUTTON_LEFT )
      {
        if ( (*g).pos.x != (*g).old_pos.x or (*g).pos.y != (*g).old_pos.y ) 
        {
          if ( (*g).mouse.cell_type ) (*g).grid[(*g).pos.x][(*g).pos.y] = true ;
          else // larger eraser
          {
            for ( int i = (*g).pos.x-1 ; i < (*g).pos.x+2 ; i++ )
            {
              for ( int j = (*g).pos.y-1 ; j < (*g).pos.y+2 ; j++ )
              {
                if ( validNeighbour(&(*g).grid, i, j) ) (*g).grid[i][j] = false ;
              }
            }
          }
        }
      }
      else if ( evt.button.button == SDL_BUTTON_RIGHT )
      {
        if ( (*g).pos.x != (*g).old_pos.x or (*g).pos.y != (*g).old_pos.y ) 
        {
          for ( int i = (*g).pos.x-1 ; i < (*g).pos.x+2 ; i++ )
          {
            for ( int j = (*g).pos.y-1 ; j < (*g).pos.y+2 ; j++ )
            {
              if ( validNeighbour(&(*g).grid, i, j) ) (*g).grid[i][j] = false ;
            }
          }
        }
      }
      (*g).old_pos.x = (*g).pos.x ;
      (*g).old_pos.y = (*g).pos.y ;
    }

    if ( (*g).move.hold_up and (*g).origin.x > W_GRID_H ) (*g).origin.x-- ;
    if ( (*g).move.hold_down and (*g).origin.x < (SIZE-2)*W_GRID_H ) (*g).origin.x++ ;
    if ( (*g).move.hold_left and (*g).origin.y > W_GRID_W ) (*g).origin.y-- ;
    if ( (*g).move.hold_right and (*g).origin.y < (SIZE-2)*W_GRID_W ) (*g).origin.y++ ;
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
  Game g ;

  while ( not g.quit )
  {
    if ( isEmpty(&g.grid) ) g.start = false ;

    SDL_RenderClear(rd) ;

    // Key listener and updater
    actionKeys(&g) ;

    // Displayer
    drawPixels(rd, &g) ;

    // Update the cell life according to their fate (no)
    if ( g.start )
    {
      g = update(&g) ;
      SDL_Delay(DELAY) ;
    }
    SDL_RenderPresent(rd) ;
  }

  // Adieu !
  SDL_DestroyRenderer(rd) ;
  SDL_DestroyWindow(wnd) ;
  SDL_Quit() ;
 
  return 0 ;
}