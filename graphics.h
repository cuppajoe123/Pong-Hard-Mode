#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
// speed in pixels/second
#define SPEED (150)


extern SDL_Window *win;
extern SDL_Renderer *rend;
extern TTF_Font *font;

SDL_Texture *make_texture(char *img);
int init_graphics(void);

#endif
