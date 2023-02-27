#ifndef PARAMS_HPP
#define PARAMS_HPP

  #define WIDTH 1080
  #define HEIGHT 720

  #define P_SIZE 20
  #define GRID_W int( float(WIDTH) / float(P_SIZE) ) + 1
  #define GRID_H int( float(HEIGHT) / float(P_SIZE) ) + 1

  extern int DELAY ;

  extern int GRID_COLOR1_R ;
  extern int GRID_COLOR1_G ;
  extern int GRID_COLOR1_B ;

  extern int GRID_COLOR2_R ;
  extern int GRID_COLOR2_G ;
  extern int GRID_COLOR2_B ;

  extern int P_COLOR_R ;
  extern int P_COLOR_G ;
  extern int P_COLOR_B ;

#endif // PARAMS_HPP