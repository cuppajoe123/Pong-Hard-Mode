#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphics.h"


SDL_Window *win;
SDL_Renderer *rend;
TTF_Font *font;

SDL_Texture *make_texture(char *img)
{
    /* Creates an SDL_Surface from img, creates texture from surface,
       checks for errors, and returns the finished texture */
    SDL_Surface *surface = IMG_Load(img);
    if (!surface) {
        printf("error creating paddle_surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return NULL; // not sure if this is the correct return value
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        printf("error creating paddle_texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return NULL;
    }

    return tex;
}

int init_graphics(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    SDL_Quit();
    return 1;
    }

    font = TTF_OpenFont("resources/FiraCode-Regular.ttf", 15);

    // create a window
    win = SDL_CreateWindow("Pong: Hard Mode!",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!win) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    return 0;
}
