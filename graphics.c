#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphics.h"


SDL_Window *win;
SDL_Renderer *rend;
TTF_Font *font;

SDL_Texture *make_texture_img(char *img)
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

SDL_Texture *make_texture_str(char *str)
{
    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, str, color);
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



SDL_Rect make_textbox(SDL_Texture *tex, int x, int y, int scale, int args)
{
    SDL_Rect rect;
    SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
    rect.w *= scale;
    rect.h *= scale;
    if ((args ^ 0x01) == 0) {
        rect.x = (WINDOW_WIDTH / 2) - (rect.w / 2);
        rect.y = y;
        return rect;
    }

    rect.x = x;
    rect.y = y;
    return rect;

    return rect;
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


void start_screen(void)
{
    SDL_Texture *title_texture = make_texture_str("Pong: Hard Mode");
    SDL_Rect title_rect = make_textbox(title_texture, 0, WINDOW_HEIGHT / 2, 2, CENTERED);

    SDL_Texture *prompt_texture = make_texture_str("Press any button to start");
    SDL_Rect prompt_rect = make_textbox(prompt_texture, 0, WINDOW_HEIGHT / 1.5, 1, CENTERED);

    bool close_requested = false;
    while (!close_requested) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close_requested = true;
                    break;
            }
        }

        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, title_texture, NULL, &title_rect);
        SDL_RenderCopy(rend, prompt_texture, NULL, &prompt_rect);
        SDL_RenderPresent(rend);
        SDL_Delay(1000/60);
    }
    SDL_DestroyTexture(title_texture);
    SDL_DestroyTexture(prompt_texture);
}
