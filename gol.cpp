#include <array>
#include "SDL2/SDL.h"

#define WIDTH 800
#define HEIGHT 600

template<std::size_t w, std::size_t h>
void drawPixels( SDL_Renderer* rd, const std::array<std::array<bool, w>, h>* a, const int size = 20 )
{
  

  for ( std::size_t i = 1 ; i < h-1 ; i++ )
  {
    for ( std::size_t j = 1 ; j < w-1 ; j++ )
    {
      if ( (*a).at(i).at(j) )
      {
        SDL_SetRenderDrawColor(rd, 0, 0, 0, 255) ;
      }
      else
      {
        SDL_SetRenderDrawColor(rd, 255, 255, 255, 255) ;
      }
      SDL_Rect rect = { i*size, j*size, size, size } ;
      SDL_RenderFillRect(rd, &rect) ;
    }
  }
}

template<std::size_t w, std::size_t h>
int numberNeighbours( const std::array<std::array<bool, w>, h>* a, const int i, const int j )
{
  return (*a).at(i-1).at(j-1) + (*a).at(i-1).at(j) + (*a).at(i-1).at(j+1) + (*a).at(i).at(j-1) + (*a).at(i).at(j+1) + (*a).at(i+1).at(j-1) + (*a).at(i+1).at(j) + (*a).at(i+1).at(j+1) ;
}

template<std::size_t w, std::size_t h>
void update( std::array<std::array<bool, w>, h>* a )
{
  for ( std::size_t i = 1 ; i < h-1 ; i++ )
  {
    for ( std::size_t j = 1 ; j < w-1 ; j++ )
    {
      int n = numberNeighbours(a, i, j) ;
      if ( (*a).at(i).at(j) )
      {
        if ( not ( n == 2 or n == 3 ) ) (*a)[i][j] = false ;
      }
      else
      {
        if ( n == 3 ) (*a)[i][j] = true ; 
      }
    }
  }
}

int main(int argc, char const **argv)
{
  SDL_Init(SDL_INIT_VIDEO) ;

  SDL_Window* wnd = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE) ;
  SDL_Renderer* rd = SDL_CreateRenderer(wnd, -1, 0) ;

  SDL_SetRenderDrawColor(rd, 255, 255, 255, 255) ;
  SDL_RenderClear(rd) ;

  const std::size_t n = 10 ;
  std::array<std::array<bool, n+2>, n+2> a ;
  for ( std::size_t i = 0 ; i < n+2 ; i++ )
  {
    for ( std::size_t j = 0 ; j < n+2 ; j++ )
    {
      a[i][j] = false ;
    }
  }

  bool quit = false ;
  while ( not quit )
  {
    SDL_Event evt ;
    while ( SDL_PollEvent(&evt) )
    {
      if ( evt.type == SDL_QUIT ) quit = true ;
    }
    drawPixels(rd, &a) ;
    update(&a) ;

    SDL_RenderPresent(rd) ;
  }

  SDL_DestroyRenderer(rd) ;
  SDL_DestroyWindow(wnd) ;
  SDL_Quit() ;
 
  return 0 ;
}