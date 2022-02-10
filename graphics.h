#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
// speed in pixels/second
#define SPEED (150)
#define CENTERED 0x01


extern SDL_Window *win;
extern SDL_Renderer *rend;
extern TTF_Font *font;

SDL_Texture *make_texture_img(char *img);
SDL_Texture *make_texture_str(char *str);
int init_graphics(void);
SDL_Rect make_textbox(SDL_Texture *tex, int x, int y, int scale, int args);
void start_screen(void);

#endif
