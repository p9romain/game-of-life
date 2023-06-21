#include <array>
#include <iostream>
#include <SDL2/SDL.h>

#include "params.hpp"



template<std::size_t w, std::size_t h>
void drawPixels( SDL_Renderer* rd, const std::array<std::array<bool, w>, h>* a, int ox, int oy)
{
  for ( std::size_t i = ox ; i < ox + GRID_H ; i++ )
  {
    for ( std::size_t j = oy ; j < oy + GRID_W ; j++ )
    {
      if ( (*a).at(i).at(j) )
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



template<std::size_t w, std::size_t h>
bool validNeighbour( const std::array<std::array<bool, w>, h>* a, const int i, const int j )
{
  return not ( i < 0 or i >= int(h) or j < 0 or j >= int(w) ) ;
}

template<std::size_t w, std::size_t h>
int numberNeighbours( const std::array<std::array<bool, w>, h>* a, const int x, const int y )
{
  int n = 0 ;
  for ( int i = x - 1 ; i < x + 2 ; i++ )
  {
    for ( int j = y - 1 ; j < y + 2 ; j++ )
    {
      if ( i == x and j == y ) continue ;
      if ( validNeighbour(a, i, j) ) n += (*a).at(i).at(j) ;
    }
  }
  return n ;
}



template<std::size_t w, std::size_t h>
std::array<std::array<bool, w>, h> update( const std::array<std::array<bool, w>, h>* a )
{
  std::array<std::array<bool, w>, h> res = *a ;
  bool b ;
  for ( std::size_t i = 0 ; i < h ; i++ )
  {
    for ( std::size_t j = 0 ; j < w ; j++ )
    {
      int n = numberNeighbours(a, i, j) ;
      if ( (*a).at(i).at(j) )
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



template<std::size_t w, std::size_t h>
void init(std::array<std::array<bool, w>, h>* a)
{
  for ( std::size_t i = 0 ; i < h ; i++ )
  {
    for ( std::size_t j = 0 ; j < w ; j++ )
    {
      (*a)[i][j] = false ;
    }
  }
}

template<std::size_t w, std::size_t h>
bool isEmpty(std::array<std::array<bool, w>, h>* a)
{
  for ( std::size_t i = 0 ; i < h ; i++ )
  {
    for ( std::size_t j = 0 ; j < w ; j++ )
    {
      if ( (*a).at(i).at(j) ) return false ;
    }
  }
  return true ;
}



int main(int argc, char **argv)
{
  SDL_Init(SDL_INIT_VIDEO) ;

  SDL_Window* wnd = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN) ;
  SDL_Renderer* rd = SDL_CreateRenderer(wnd, -1, 0) ;

  std::array<std::array<bool, SIZE*GRID_W>, SIZE*GRID_H> a ;
  init(&a) ;

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
    if ( isEmpty(&a) ) start = false ;

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
          init(&a) ;
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

      if ( not start and hold_mouse and evt.button.button == SDL_BUTTON_LEFT )
      {
        x = int(ox + evt.button.y/float(P_SIZE)) + 1 ;
        y = int(oy + evt.button.x/float(P_SIZE)) + 1 ;
        if ( x != old_x or y != old_y ) a[x][y] = not a[x][y] ;
        old_x = x ;
        old_y = y ;
      }

      if ( hold_up and ox > GRID_H ) ox-- ;
      if ( hold_down and ox < (SIZE-2)*GRID_H ) ox++ ;
      if ( hold_left and oy > GRID_W ) oy-- ;
      if ( hold_right and oy < (SIZE-2)*GRID_W ) oy++ ;
    }

    drawPixels(rd, &a, ox, oy) ;

    if ( start )
    {
      a = update(&a) ;
      SDL_Delay(DELAY) ;
    }

    SDL_RenderPresent(rd) ;
  }

  SDL_DestroyRenderer(rd) ;
  SDL_DestroyWindow(wnd) ;
  SDL_Quit() ;
 
  return 0 ;
}