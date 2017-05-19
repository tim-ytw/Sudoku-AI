
// LOCAL
#include "graphicdisplay.h"

// CPP
#include <iostream>
#include <string>


using namespace std;

const string FONT_FILE = "Resources/font.ttf";

#define COLOURS \
  COLOUR("White",   0, 255, 255, 255, 255) \
  COLOUR("Purple",  1, 204, 153, 255, 255) \
  COLOUR("Yellow",  2, 255, 255, 153, 255) \
  COLOUR("Red",     3, 255, 153, 153, 255) \
  COLOUR("Green",   4, 153, 255, 153, 255) \
  COLOUR("Blue",    5, 102, 178, 255, 255) \
  COLOUR("Black",   6,   0,   0,   0, 255)


#define COLOUR(name, num, r, g, b, a) {r, g, b, a},
const SDL_Color kColors[] =
{
  COLOURS
};
#undef COLOUR

#define COLOUR(name, num, r, g, b, a) name,
const string kColorNames[] =
{
 COLOURS
};
#undef COLOUR

const int kBackgroundColor = 0;
const SDL_Color kBackgroundColour = kColors[0];
const SDL_Color kMessageColour = kColors[6];

const int WINDOW_X = SDL_WINDOWPOS_UNDEFINED;
const int WINDOW_Y = SDL_WINDOWPOS_UNDEFINED;

const int MESSAGE_PANEL_X = 0;
const int MESSAGE_PANEL_Y = GAME_PANEL_HEIGHT;

const int MESSAGE_PANEL_W = WINDOW_WIDTH;
const int MESSAGE_PANEL_H = WINDOW_HEIGHT - MESSAGE_PANEL_Y;


SDL_Color getColor(char image)
{
  return kColors[image];
}


GraphicDisplay::GraphicDisplay(int n):View(n)
{
  GRIDHEIGHT = GAME_PANEL_HEIGHT/ grid_size_;
  GRIDWIDTH  = GAME_PANEL_WIDTH / grid_size_;

  grids_ = new int*[grid_size_];
  for (int r = 0; r < grid_size_; r++)
  {
    grids_[r] = new int[grid_size_];
    for (int c = 0; c < grid_size_; c++)
    {
      grids_[r][c] = 0;
    }
  }
	InitSDL();
  
	DrawBackground();
}


GraphicDisplay::~GraphicDisplay()
{
  for (int r = 0; r < grid_size_; r++)
  {
    delete[] grids_[r];
  }
	delete[] grids_;

	QuitSDL();
}

void GraphicDisplay::Notify(int row, int column, int new_color)
{
  grids_[row][column]= new_color;
  Render();
}



void GraphicDisplay::DrawBackground()
{
	SDL_SetRenderDrawColor(renderer_, kBackgroundColour.r, kBackgroundColour.g,
                         kBackgroundColour.b, kBackgroundColour.a);
	SDL_RenderClear(renderer_);
}


void GraphicDisplay::Render()
{
	DrawBackground();
	for (int r = 0; r < grid_size_; r++)
	{
		for (int c = 0; c < grid_size_; c++)
		{
			DrawGrid(c, r, grids_[r][c]);
		}
	}
	RenderMessage();
	SDL_RenderPresent(renderer_);
}



int GraphicDisplay::GetColor(int row, int col)
{
	return grids_[row][col];
}



void GraphicDisplay::UpdateMessage(const string& message)
{
	UpdateMessageTexture(message);
	Render();
}


void GraphicDisplay::UpdateMessageTexture(const string& message)
{
	SDL_DestroyTexture(message_texture_);
	SDL_Surface* message_surface = TTF_RenderText_Solid(font_, message.c_str(), kMessageColour);
	
	message_height_ = message_surface->h;
	message_width_ = message_surface->w;
	
	message_texture_ = SDL_CreateTextureFromSurface(renderer_, message_surface);
	SDL_FreeSurface(message_surface);
}



/*----------------------------- SDL Methods --------------------------------*/

void GraphicDisplay::InitSDL(){
	
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
	
	SDL_SetRenderDrawColor( renderer_, 0xFF, 0xFF, 0xFF, 0xFF );
	
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
}



void GraphicDisplay::DrawGrid(int x, int y, int image)
{
  SDL_Color color = getColor(image);
  SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  SDL_Rect position = {x*GRIDWIDTH, y*GRIDHEIGHT, GRIDWIDTH, GRIDHEIGHT};
  SDL_RenderFillRect(renderer_, &position);
}


void GraphicDisplay::RenderMessage()
{
  SDL_Rect clip = {MESSAGE_PANEL_X, MESSAGE_PANEL_Y, MESSAGE_PANEL_W, MESSAGE_PANEL_H};
  SDL_RenderSetViewport(renderer_, &clip);
  SDL_SetRenderDrawColor(renderer_, message_panel_colour_.r, message_panel_colour_.g, message_panel_colour_.b,message_panel_colour_.a);
  
  SDL_Rect MESSAGE_RECT = {(MESSAGE_PANEL_W - message_width_)/2,
    (MESSAGE_PANEL_H - message_height_)/2, message_width_, message_height_};
  
  SDL_RenderCopyEx(renderer_, message_texture_,
                   NULL, &MESSAGE_RECT, 0, NULL, SDL_FLIP_NONE);
  SDL_RenderSetViewport(renderer_, NULL);
}


void GraphicDisplay::QuitSDL()
{
  TTF_CloseFont(font_);
	SDL_DestroyTexture(message_texture_);
	
	SDL_Quit();
	TTF_Quit();
	
	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
}


