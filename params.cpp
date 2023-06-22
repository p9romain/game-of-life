#include "params.hpp"

int P_SIZE = P_SIZE_MIN ;
int GRID_W = int( float(W_WIDTH) / float(P_SIZE) ) + 1 ;
int GRID_H = int( float(W_HEIGHT) / float(P_SIZE) ) + 1 ;

float DELAY = 1/1000000. ;

int GRID_COLOR1_R  = 255 ;
int GRID_COLOR1_G  = 255 ;
int GRID_COLOR1_B  = 255 ;

int GRID_COLOR2_R  = 240 ;
int GRID_COLOR2_G  = 240 ;
int GRID_COLOR2_B  = 240 ;

int P_COLOR_R  = 0 ;
int P_COLOR_G  = 0 ;
int P_COLOR_B  = 0 ;