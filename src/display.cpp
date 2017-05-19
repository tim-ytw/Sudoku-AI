// C++
#include <string>
#include <iostream>

// SDL
#include <sdl2/sdl.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>


#define COLOURS \
COLOUR(White,   0, 255, 255, 255, 255) \
COLOUR(Purple,  1, 204, 153, 255, 255) \
COLOUR(Yellow,  2, 255, 255, 153, 255) \
COLOUR(Red,     3, 255, 153, 153, 255) \
COLOUR(Green,   4, 153, 255, 153, 255) \
COLOUR(Blue,    5, 102, 178, 255, 255) \
COLOUR(Black,   6,   0,   0,   0, 255)


#define COLOUR(name, num, r, g, b, a) {r, g, b, a},
const SDL_Color kColors[] =
{
  COLOURS
};
#undef COLOUR


#define COLOUR(name, num, r, g, b, a) name,
enum kColorIndex
{
  COLOURS
};
#undef COLOUR

using namespace std;

const int FONT_SIZE = 30;

const int WINDOW_WIDTH = 360;
const int WINDOW_HEIGHT = 360;

const int TEXT_PANEL_HEIGHT = 0;

const int CELL_SIZE = WINDOW_WIDTH / 9;

const int MARGIN_SIZE = CELL_SIZE / 3;

const int WINDOW_X = SDL_WINDOWPOS_UNDEFINED;
const int WINDOW_Y = SDL_WINDOWPOS_UNDEFINED;

const string FONT_FILE = "Resources/font.ttf";

const int VERTICAL_LINE_GAP = WINDOW_WIDTH / 9;
const int TEXT_PANEL_X = WINDOW_HEIGHT - TEXT_PANEL_HEIGHT;
const int HORIZONTAL_LINE_GAP = (WINDOW_HEIGHT - TEXT_PANEL_HEIGHT) / 9;


TTF_Font* font_;
SDL_Rect digit_rect;
SDL_Window* window_;
SDL_Renderer* renderer_;
SDL_Texture* message_texture_;
SDL_Color message_panel_colour_;
SDL_Texture* number_textures[10];

const SDL_Color kNumberColour = kColors[kColorIndex::Blue];


inline void SetColor(int index)
{
  SDL_SetRenderDrawColor(renderer_, kColors[index].r, kColors[index].g, kColors[index].g, kColors[index].a);
}



inline void DrawGrid()
{
  SetColor(kColorIndex::Blue);

  int x = 0;
  int y = 0;
  int count = 1;

  while (count ++ <= 8)
  {
    x += VERTICAL_LINE_GAP;
    y += HORIZONTAL_LINE_GAP;
    SDL_RenderDrawLine(renderer_, 0, y, WINDOW_WIDTH, y);
    SDL_RenderDrawLine(renderer_, x, 0, x, TEXT_PANEL_X);
  }
  SDL_RenderDrawLine(renderer_, 0, TEXT_PANEL_X, WINDOW_WIDTH, TEXT_PANEL_X);
}


inline void DrawNumbers(int grid[9][9])
{
  for (int row = 0; row < 9; row ++)
  {
    for (int col = 0; col < 9; col ++)
    {
      if (grid[row][col] < 0 || grid[row][col] > 9) continue;

      digit_rect.x = col * CELL_SIZE + MARGIN_SIZE;
      digit_rect.y = row * CELL_SIZE + MARGIN_SIZE;

      SDL_RenderCopy(renderer_, number_textures[grid[row][col]], NULL, &digit_rect);
    }
  }
}

inline void DrawBackground()
{
  SetColor(kColorIndex::White);

  SDL_RenderClear(renderer_);
}


void Draw(int grid[9][9], int nodeExpanded)
{
  DrawBackground();

  DrawGrid();

  DrawNumbers(grid);

  SDL_RenderPresent(renderer_);
}


void InitSDL()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw "SDL Initialization Failed: " + string(SDL_GetError());
  }

  window_ = SDL_CreateWindow("Sudoku", WINDOW_X, WINDOW_Y,
                             WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

  if (window_ == NULL)
  {
    throw "SDL Creating Window failed: " + string(SDL_GetError());
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

  if (renderer_ == NULL)
  {
    throw "SDL Creating Renderer failed: " + string(SDL_GetError());
  }

  // White by default
  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);

  SDL_RenderClear(renderer_);

  if (TTF_Init() == -1)
  {
    throw "TTF Initialization failed " + string(TTF_GetError());
  }

  font_ = TTF_OpenFont(FONT_FILE.c_str(), FONT_SIZE);

  if(font_ == NULL)
  {
    throw "Open font_ failed: " + string(TTF_GetError());
  }
  message_texture_ = NULL;

  // Prepare digit textures
  for (int digit = 0; digit < 10; digit ++)
  {
    SDL_Surface* surface = TTF_RenderText_Solid(font_, to_string(digit).c_str(), kNumberColour);

    number_textures[digit] = SDL_CreateTextureFromSurface(renderer_, surface);

    digit_rect.w = surface->w;
    digit_rect.h = surface->h;

    SDL_FreeSurface(surface);
  }
}



void QuitSDL()
{
  TTF_CloseFont(font_);
  SDL_DestroyTexture(message_texture_);

  SDL_Quit();
  TTF_Quit();

  SDL_DestroyWindow(window_);
  SDL_DestroyRenderer(renderer_);
}

