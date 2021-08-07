
#ifndef Settings_H
#define Settings_H

#define Clear_Brightness 20 /* value 0-255 */
#define Window_Width 800
#define Window_Height 600
#define Window_Scale 5
#define Draw_Window_Width (Window_Width / Window_Scale)
#define Draw_Window_Height (Window_Height / Window_Scale)
#define Window_Title "Super Mario Bros."

// key mapping:

// editor map
//   CONTROL + Arrows --- expand map borders
//   CONTROL + SHIFT + Arrows --- shrink map borders
//   SHIFT + Arrows --- select tile and tileset's tile
//   ALT + Enter --- choose selected tileset's tile 
//   Arrows --- select tile
//   Enter --- place tile
//   Backspace --- erase tile
//   Escape --- to map list

// editor tileset
//   grid mode
//      CONTROL + Enter --- create new grid
//      CONTROL + Backspace --- delete grid
//      ALT + Arrows --- move grid
//      Left / Right --- select grid
//      Enter --- choose grid
//      Escape --- to tileset list
//   tile mode
//      Arrows --- select tile
//      Enter --- choose tile
//      Escape --- to grid list
//      ALT + SHIFT + Arrows --- expand/shrink tile padding
//      ALT + Arrows --- expand/shrink tile dimensions
//      CONTROL + Arrows --- expand/shrink grid dimensions

#endif /* Settings_H */
