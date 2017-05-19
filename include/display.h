#ifndef display_h
#define display_h

// C++
#include <string>

// SDL
#include <SDL2/SDL.h>



void InitSDL();

void QuitSDL();

void Draw(int grid[9][9], int expanded);


#endif /* display_h */
