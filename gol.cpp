#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

#include "params.hpp"



void drawPixels( SDL_Renderer* rd, const std::vector<std::vector<bool>>* v, int ox, int oy)
{
  for ( std::size_t i = ox ; i < ox + GRID_H ; i++ )
  {
    for ( std::size_t j = oy ; j < oy + GRID_W ; j++ )
    {
      if ( (*v).at(i).at(j) )
      {
        SDL_SetRenderDrawColor(rd, P_COLOR_R, P_COLOR_G, P_COLOR_B, 255) ;
        SDL_Rect rect = { int(j-oy-1)*P_SIZE, int(i-ox-1)*P_SIZE, P_SIZE, P_SIZE } ;
        SDL_RenderFillRect(rd, &rect) ;
      }
      else if ( (i+j-ox-oy)%2 == 0 or (i-j-ox+oy)%2 == 0 )
      {
        SDL_SetRenderDrawColor(rd, GRID_COLOR2_R, GRID_COLOR2_G, GRID_COLOR2_B, 255) ;
        SDL_Rect rect = { int(j-oy-1)*P_SIZE, int(i-ox-1)*P_SIZE, P_SIZE, P_SIZE } ;
        SDL_RenderFillRect(rd, &rect) ;
      }
    }
  }
}



bool validNeighbour( const std::vector<std::vector<bool>>* v, const int i, const int j )
{
  return not ( i < 0 or i >= SIZE*GRID_H or j < 0 or j >= SIZE*GRID_W ) ;
}

int numberNeighbours( const std::vector<std::vector<bool>>* v, const int x, const int y )
{
  int n = 0 ;
  for ( int i = x - 1 ; i < x + 2 ; i++ )
  {
    for ( int j = y - 1 ; j < y + 2 ; j++ )
    {
      if ( i == x and j == y ) continue ;
      if ( validNeighbour(v, i, j) ) n += (*v).at(i).at(j) ;
    }
  }
  return n ;
}



std::vector<std::vector<bool>> update( const std::vector<std::vector<bool>>* v )
{
  std::vector<std::vector<bool>> res = *v ;
  bool b ;
  for ( std::size_t i = 0 ; i < SIZE*GRID_H ; i++ )
  {
    for ( std::size_t j = 0 ; j < SIZE*GRID_W ; j++ )
    {
      int n = numberNeighbours(v, i, j) ;
      if ( (*v).at(i).at(j) )
      {
        if ( not ( n == 2 or n == 3 ) ) b = false ;
        else b = true ;
      }
      else
      {
        if ( n == 3 ) b = true ;
        else b = false ;
      }
      res[i][j] = b ;
    }
  }
  return res ;
}



void reset(std::vector<std::vector<bool>>* v)
{
  for ( std::size_t i = 0 ; i < SIZE*GRID_H ; i++ )
  {
    for ( std::size_t j = 0 ; j < SIZE*GRID_W ; j++ )
    {
      (*v)[i][j] = false ;
    }
  }
}

bool isEmpty(std::vector<std::vector<bool>>* v)
{
  for ( std::size_t i = 0 ; i < SIZE*GRID_H ; i++ )
  {
    for ( std::size_t j = 0 ; j < SIZE*GRID_W ; j++ )
    {
      if ( (*v).at(i).at(j) ) return false ;
    }
  }
  return true ;
}

int main(int argc, char **argv)
{
  SDL_Init(SDL_INIT_VIDEO) ;

  SDL_Window* wnd = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN) ;
  SDL_Renderer* rd = SDL_CreateRenderer(wnd, -1, 0) ;

  std::vector<std::vector<bool>> v(SIZE*GRID_H, std::vector<bool>(SIZE*GRID_W, false)) ;

  P_SIZE *= 4 ;

  int ox = SIZE/2*GRID_H ;
  int oy = SIZE/2*GRID_W ;

  int x, y ;
  int old_x = -1 ;
  int old_y = -1 ;

  bool hold_mouse = false ;
  bool hold_up = false ;
  bool hold_down = false ;
  bool hold_left = false ;
  bool hold_right = false ;

  bool start = false ;
  bool quit = false ;

  while ( not quit )
  {
    if ( isEmpty(&v) ) start = false ;

    SDL_SetRenderDrawColor(rd, GRID_COLOR1_R, GRID_COLOR1_G, GRID_COLOR1_B, 255) ;
    SDL_RenderClear(rd) ;

    SDL_Event evt ;
    while ( SDL_PollEvent(&evt) )
    {
      if ( evt.type == SDL_QUIT ) quit = true ;
      else if ( evt.type == SDL_KEYDOWN ) 
      {
        if ( evt.key.keysym.sym == SDLK_r or evt.key.keysym.sym == SDLK_F2 or evt.key.keysym.sym == SDLK_BACKSPACE )
        {
          reset(&v) ;
          start = false ;
        }
        if ( evt.key.keysym.sym == SDLK_p or evt.key.keysym.sym == SDLK_ESCAPE ) start = not start ;
        else if ( evt.key.keysym.sym == SDLK_SPACE or evt.key.keysym.sym == SDLK_RETURN ) start = true ;

        if ( evt.key.keysym.sym == SDLK_z or evt.key.keysym.sym == SDLK_UP ) hold_up = true ;
        else if ( evt.key.keysym.sym == SDLK_s or evt.key.keysym.sym == SDLK_DOWN ) hold_down = true ;
        if ( evt.key.keysym.sym == SDLK_q or evt.key.keysym.sym == SDLK_LEFT ) hold_left = true ;
        else if ( evt.key.keysym.sym == SDLK_d or evt.key.keysym.sym == SDLK_RIGHT ) hold_right = true ;
      }
      else if ( evt.type == SDL_KEYUP )
      {
        if ( evt.key.keysym.sym == SDLK_z or evt.key.keysym.sym == SDLK_UP ) hold_up = false ;
        else if ( evt.key.keysym.sym == SDLK_s or evt.key.keysym.sym == SDLK_DOWN ) hold_down = false ;
        if ( evt.key.keysym.sym == SDLK_q or evt.key.keysym.sym == SDLK_LEFT ) hold_left = false ;
        else if ( evt.key.keysym.sym == SDLK_d or evt.key.keysym.sym == SDLK_RIGHT ) hold_right = false ;
      }
      else if ( evt.type == SDL_MOUSEBUTTONDOWN ) hold_mouse = true ;
      else if ( evt.type == SDL_MOUSEBUTTONUP ) 
      {
        hold_mouse = false ;
        old_x = -1 ;
        old_y = -1 ;
      }
      else if ( evt.type == SDL_MOUSEWHEEL )
      {
        if ( evt.wheel.y > 0 and GRID_H > 5 ) P_SIZE++ ;
        else if ( evt.wheel.y < 0 and P_SIZE > P_SIZE_MIN ) P_SIZE-- ;

        GRID_W = int( float(W_WIDTH) / float(P_SIZE) ) + 1 ;
        GRID_H = int( float(W_HEIGHT) / float(P_SIZE) ) + 1 ;
      }

      if ( not start and hold_mouse and evt.button.button == SDL_BUTTON_LEFT )
      {
        x = int(ox + evt.button.y/float(P_SIZE)) + 1 ;
        y = int(oy + evt.button.x/float(P_SIZE)) + 1 ;
        if ( x != old_x or y != old_y ) v[x][y] = not v[x][y] ;
        old_x = x ;
        old_y = y ;
      }

      if ( hold_up and ox > GRID_H ) ox-- ;
      if ( hold_down and ox < (SIZE-2)*GRID_H ) ox++ ;
      if ( hold_left and oy > GRID_W ) oy-- ;
      if ( hold_right and oy < (SIZE-2)*GRID_W ) oy++ ;
    }

    drawPixels(rd, &v, ox, oy) ;

    if ( start )
    {
      v = update(&v) ;
      SDL_Delay(DELAY) ;
    }

    SDL_RenderPresent(rd) ;
  }

  SDL_DestroyRenderer(rd) ;
  SDL_DestroyWindow(wnd) ;
  SDL_Quit() ;
 
  return 0 ;
}