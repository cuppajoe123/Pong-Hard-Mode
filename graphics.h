#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
// speed in pixels/second
#define SPEED (150)
#define CENTERED_X 0x01
#define CENTERED_Y 0x02


extern SDL_Window *win;
extern SDL_Renderer *rend;
extern TTF_Font *font;

SDL_Texture *make_texture_img(char *img);
SDL_Texture *make_texture_str(char *str);
int init_graphics(void);
SDL_Rect make_textbox(SDL_Texture *tex, int x, int y, float scale, int args);
int start_screen(void);
int username_screen(char *username);
int leader_board_screen(void);

#endif
