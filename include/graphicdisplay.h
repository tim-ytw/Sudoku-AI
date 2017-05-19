#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H

// CPP
#include <iostream>
#include <sstream>
#include <string>

// SDL2
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"

// LOCAL
#include "view.h"

extern const int kBackgroundColor;
extern const std::string kColorNames[];


class GraphicDisplay : public View
{
public:
	GraphicDisplay(int n);
	~GraphicDisplay();
	
  void Render();
  int GetColor(int row, int col);
  void UpdateMessage(const std::string&);
  void Notify(int row, int column, int change);
  
private:
  void InitSDL();
  void QuitSDL();
  void DrawBackground();
  void RenderMessage();
  void DrawGrid(int x, int y, int colour);
  void UpdateMessageTexture(const std::string& message);
  
  TTF_Font* font_;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  SDL_Texture* message_texture_;
  SDL_Color message_panel_colour_;
  
  int **grids_;
  int message_width_;
  int message_height_;
};

#endif
