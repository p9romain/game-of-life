#include "params.hpp"

// Pixel size (for zoom)
int P_SIZE = P_SIZE_MIN ;
// Grid size on windows
int W_GRID_W = int( float(W_WIDTH) / float(P_SIZE) ) + 1 ;
int W_GRID_H = int( float(W_HEIGHT) / float(P_SIZE) ) + 1 ;

// Delay between screen update
float DELAY = 5 ;

// Color of pixel 1
int GRID_COLOR1_R  = 255 ;
int GRID_COLOR1_G  = 255 ;
int GRID_COLOR1_B  = 255 ;
// Color of pixel 2
int GRID_COLOR2_R  = 240 ;
int GRID_COLOR2_G  = 240 ;
int GRID_COLOR2_B  = 240 ;

// Color of alive cell
int P_COLOR_R  = 0 ;
int P_COLOR_G  = 0 ;
int P_COLOR_B  = 0 ;