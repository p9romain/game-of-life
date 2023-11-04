#ifndef PARAMS_HPP
#define PARAMS_HPP

  #include <vector>
  #include <set>

  // Windows size
  #define W_WIDTH 1080
  #define W_HEIGHT 720
  #define RATIO float(W_WIDTH)/float(W_HEIGHT)

  // Size of "inifite" grid (6 times a grid)
  #define SIZE 6
  // Pixel size
  #define P_SIZE_MIN 4
  extern int P_SIZE ;
  // Grid size on windows
  extern int W_GRID_W ;
  extern int W_GRID_H ;

  // Delay between screen update
  extern float DELAY ;

  // Color of pixel 1
  extern int GRID_COLOR1_R ;
  extern int GRID_COLOR1_G ;
  extern int GRID_COLOR1_B ;

  // Color of pixel 2
  extern int GRID_COLOR2_R ;
  extern int GRID_COLOR2_G ;
  extern int GRID_COLOR2_B ;

  // Color of alive cell
  extern int P_COLOR_R ;
  extern int P_COLOR_G ;
  extern int P_COLOR_B ;

  // A point
  struct Coord
  {
    int x, y ;
   
    bool operator<(const Coord& c) const
    {
       return x < c.x or y < c.y ;
    }
  } ;

  // All the needed information to make the game
  struct Game 
  {
    // Grid indexes of the top-left pixel
    Coord origin = { SIZE/2*W_GRID_H, SIZE/2*W_GRID_W } ;
    // Old indexes of the mouse on screen (need for holding)
    Coord old_pos = { -1, -1 } ;
    // Indexes of the mouse on screen
    Coord pos ;

    // To manage holding to draw or erase
    struct Mouse
    {
      bool hold = false ;
      bool cell_type = false ;
    } mouse ;

    // To manage camera (grid part which is drawn)
    struct Move
    {
      bool hold_up = false ;
      bool hold_down = false ;
      bool hold_left = false ;
      bool hold_right = false ;
    } move ;

    // Grid of pixel (soon depreciated)
    std::vector<std::vector<bool>> grid = 
      std::vector<std::vector<bool>>(SIZE*W_GRID_H, std::vector<bool>(SIZE*W_GRID_W, false)) ;
    // Set of all alive cells
    std::set<Coord> alive_cells ;

    // Booleans for pausing the game or quiting
    bool start = false ;
    bool quit = false ;
  } ;

#endif // PARAMS_HPP