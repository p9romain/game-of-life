#ifndef PARAMS_HPP
#define PARAMS_HPP

  #include <functional>
  #include <optional>
  #include <unordered_set>

  // A point
  struct Coord
  {
    int x, y ;
   
    bool operator==(const Coord& c) const ;
  } ;

  template<>
  struct std::hash<Coord> 
  {
    std::size_t operator()(const Coord& c) const 
    {
      return std::hash<size_t>()( (size_t) c.x | ((size_t) c.y << 32) ) ;
    }
  } ;

  struct Color
  {
    int R, G, B ;
  } ;

  // All the needed information to make the game
  struct Game 
  {
    // Grid indexes of the center pixel
    Coord origin = { 0, 0 } ;

    // Old indexes of the mouse on screen (need for holding)
    // Optional : the old position, std::nullopt if not holding
    std::optional<Coord> old_mouse_pos = std::nullopt ;
    // Indexes of the mouse on screen
    Coord mouse_pos ;

    // To manage holding to draw or erase
    struct Mouse
    {
      bool hold = false ;
      bool button = true ; // true = left, false = right
      bool cell_type = false ;
    } mouse ;

    // To manage displaying
    struct Display
    {
      // To manage camera (grid part which is drawn)
      struct Camera
      {
        bool hold_up = false ;
        bool hold_down = false ;
        bool hold_left = false ;
        bool hold_right = false ;

        float offset = 0.5 ;
      } move ;

      // Grid
      bool display_grid = true ;
      Color c_grid1 = { 255, 255, 255 } ;
      Color c_grid2 = { 240, 240, 240 } ;
      Color c_pixel = { 0, 0, 0 } ;

      // Winodws size
      int width = 1080 ;
      int height = 720 ;

      // Pixel size
      const int p_size = 15 ;
      int current_p_size = p_size ;

      // Zoom
      const float zoom_min = 0.01 ;
      const float zoom_max = 4 ;
      float zoom = 1 ;

      int grid_width() const { return int( float(this->width) / float(this->current_p_size) ) + 1 ; }
      int grid_height() const { return int( float(this->height) / float(this->current_p_size) ) + 1 ; }
    } window ;

    // Set of all alive cells
    std::unordered_set<Coord> alive_set ;

    // Booleans for pausing the game or quiting
    bool start = false ;
    bool quit = false ;

    int update_interval = 50 ;

    void updateMousePos() ;
  } ;

#endif // PARAMS_HPP