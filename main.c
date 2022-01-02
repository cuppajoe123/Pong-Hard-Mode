#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

// speed in pixels/second
#define SPEED (300)

int main(void)
{
    // attempt to initialize graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Pong: Hard Mode!",
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
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load the paddle image into memory using SDL_image library function
    SDL_Surface *surface = IMG_Load("resources/paddle.png");
    if (!surface) {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load the paddle image into the graphics hardware's memory
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // struct to hold the position and size of both paddle sprites
    SDL_Rect left_pad;
    SDL_Rect right_pad;

    // get and scale the dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &left_pad.w, &left_pad.h);
    left_pad.w /= 2;
    left_pad.h /= 2;
    right_pad.w = left_pad.w;
    right_pad.h = left_pad.h;

    // start left paddle sprite in the left side of the screen with some space from the window border
    float left_pad_x_pos = (float)(WINDOW_WIDTH - (WINDOW_WIDTH - 10));
    float left_pad_y_pos = (float)(WINDOW_HEIGHT - left_pad.h) / 2;
    // set the positions in the struct
    left_pad.y = (int) left_pad_y_pos;
    left_pad.x = (int) left_pad_x_pos;

    // start right paddle sprite on right side of the screen with some space from the window border
    float right_pad_x_pos = (float)(WINDOW_WIDTH - 10);
    float right_pad_y_pos = (float)(WINDOW_HEIGHT - left_pad.h) / 2;
    // set the positions in the struct
    right_pad.y = (int) right_pad_y_pos;
    right_pad.x = (int) right_pad_x_pos;

    // initial velocity set to zero
    float x_vel = 0;
    float y_vel = 0;

    // keep track of which inputs are given
    bool up = false;
    bool down = false;

    bool close_requested = false;

    // animation loop
    while (!close_requested) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    close_requested = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = true;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = true;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            up = false;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            down = false;
                            break;
                    }
                    break;
            }
        }

        // determine velocity
        x_vel = y_vel = 0;
        if (up && !down) y_vel = -SPEED;
        if (down && !up) y_vel = SPEED;

        // update positions
        left_pad_x_pos += x_vel / 60;
        right_pad_x_pos += x_vel / 60;
        left_pad_y_pos += y_vel / 60;
        right_pad_y_pos += y_vel / 60;

        // collision detection with bounds
        // only checks left paddle because both paddles will have the same y position
        if (left_pad_y_pos <= 0) {
            left_pad_y_pos = 0;
            right_pad_y_pos = 0;
        }
        if (left_pad_y_pos >= WINDOW_HEIGHT - left_pad.h) {
            left_pad_y_pos = WINDOW_HEIGHT - left_pad.h;
            right_pad_y_pos = WINDOW_HEIGHT - right_pad.h;
        }

        // set the positions in the struct
        left_pad.y = (int) left_pad_y_pos;
        left_pad.x = (int) left_pad_x_pos;
        right_pad.y = (int) right_pad_y_pos;
        right_pad.x = (int) right_pad_x_pos;

        // clear the window
        SDL_RenderClear(rend);

        // draw the left paddle to the window
        SDL_RenderCopy(rend, tex, NULL, &left_pad);
        SDL_RenderPresent(rend);

        SDL_RenderCopy(rend, tex, NULL, &right_pad);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
