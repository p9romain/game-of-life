#include <array>
#include <iostream>
#include "SDL2/SDL.h"

#define WIDTH 800
#define HEIGHT 600

#define P_SIZE 30
#define GRID_W int( float(WIDTH) / float(P_SIZE) )
#define GRID_H int( float(HEIGHT) / float(P_SIZE) )



template<std::size_t w, std::size_t h>
void drawPixels( SDL_Renderer* rd, const std::array<std::array<bool, w>, h>* a, const int size = P_SIZE )
{
  

  for ( std::size_t i = 0 ; i < h ; i++ )
  {
    for ( std::size_t j = 0 ; j < w ; j++ )
    {
      if ( (*a).at(i).at(j) )
      {
        SDL_SetRenderDrawColor(rd, 0, 0, 0, 255) ;
        SDL_Rect rect = { int(j-1)*size, int(i-1)*size, size, size } ;
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



// template<std::size_t w, std::size_t h>
// std::ostream& operator<<(std::ostream& out, std::array<std::array<bool, w>, h>& a)
// {
//   for ( std::size_t i = 0 ; i < h ; i++ )
//   {
//     for ( std::size_t j = 0 ; j < w ; j++ )
//     {
//       out << a.at(i).at(j) << " " ;
//     }
//     out << std::endl ;
//   }
//   return out ;
// }



// template<std::size_t N>
// std::ostream& operator<<(std::ostream& out, std::array<bool, N>& a)
// {
//   for ( std::size_t i = 0 ; i < N ; i++ )
//   {
//     out << a.at(i) << " " ;
//   }
//   out << std::endl ;
//   return out ;
// }



int main(int argc, char const **argv)
{
  SDL_Init(SDL_INIT_VIDEO) ;

  SDL_Window* wnd = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE) ;
  SDL_Renderer* rd = SDL_CreateRenderer(wnd, -1, 0) ;

  std::array<std::array<bool, GRID_W>, GRID_H> a ;
  for ( std::size_t i = 0 ; i < GRID_H ; i++ )
  {
    for ( std::size_t j = 0 ; j < GRID_W ; j++ )
    {
      a[i][j] = false ;
    }
  }

  int x, y ;
  bool quit = false ;
  while ( not quit )
  {
    SDL_SetRenderDrawColor(rd, 255, 255, 255, 255) ;
    SDL_RenderClear(rd) ;

    SDL_Event evt ;
    while ( SDL_PollEvent(&evt) )
    {
      if ( evt.type == SDL_QUIT ) quit = true ;
      else if ( evt.type == SDL_MOUSEBUTTONDOWN )
      {
        if ( evt.button.button == SDL_BUTTON_LEFT ) 
        {
          x = evt.button.y/float(P_SIZE)+1 ;
          y = evt.button.x/float(P_SIZE)+1 ;
          a[x][y] = not a[x][y] ;
        }
      }
    }

    drawPixels(rd, &a) ;
    a = update(&a) ;

    SDL_Delay(1000) ;

    SDL_RenderPresent(rd) ;
  }

  SDL_DestroyRenderer(rd) ;
  SDL_DestroyWindow(wnd) ;
  SDL_Quit() ;
 
  return 0 ;
}