#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

// speed in pixels/second
#define SPEED (300)

/* the window and renderer variables are global so other functions can use them without having to pass them as arguments */
SDL_Window *win;
SDL_Renderer *rend;

SDL_Texture *make_texture(char *img);

int main(void)
{
    // attempt to initialize graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

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

    SDL_Texture *paddle_tex = make_texture("resources/paddle.png");

    // struct to hold the position and size of both paddle sprites
    SDL_Rect left_pad;
    SDL_Rect right_pad;

    // get and scale the dimensions of paddle_texture
    SDL_QueryTexture(paddle_tex, NULL, NULL, &left_pad.w, &left_pad.h);
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

    // Code for the ball
    SDL_Texture *ball_tex = make_texture("resources/ball.png");
    SDL_Rect ball;
    SDL_QueryTexture(ball_tex, NULL, NULL, &ball.w, &ball.h);
    ball.w /= 10;
    ball.h /= 10;
    float ball_x_pos = (float)(WINDOW_WIDTH) / 2;
    float ball_y_pos = (float)(WINDOW_HEIGHT) / 2;
    ball.y = (int) ball_y_pos;
    ball.x = (int) ball_x_pos;

    float ball_x_vel = SPEED;
    float ball_y_vel = SPEED;


    // initial velocity set to zero
    float pad_x_vel = 0;
    float pad_y_vel = 0;

    // keep track of which inputs are given
    bool pad_up = false;
    bool pad_down = false;

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
                            pad_up = true;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            pad_down = true;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            pad_up = false;
                            break;
                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            pad_down = false;
                            break;
                    }
                    break;
            }
        }

        // collision detection for ball
        if (ball_x_pos <= 0) {
            ball_x_pos = 0;
            ball_x_vel = -ball_x_vel;
        }
        if (ball_y_pos <= 0) {
            ball_y_pos = 0;
            ball_y_vel = -ball_y_vel;
        }
        if (ball_x_pos >= WINDOW_WIDTH - ball.w) {
            ball_x_pos = WINDOW_WIDTH - ball.w;
            ball_x_vel = -ball_x_vel;
        }
        if (ball_y_pos >= WINDOW_HEIGHT - ball.h) {
            ball_y_pos = WINDOW_HEIGHT - ball.h;
            ball_y_vel = -ball_y_vel;
        }

        ball_x_pos += ball_x_vel / 60;
        ball_y_pos += ball_y_vel / 60;

        ball.x = (int) ball_x_pos;
        ball.y = (int) ball_y_pos;


        // determine velocity of paddles
        pad_x_vel = pad_y_vel = 0;
        if (pad_up && !pad_down) pad_y_vel = -SPEED;
        if (pad_down && !pad_up) pad_y_vel = SPEED;

        // update positions of paddles
        left_pad_x_pos += pad_x_vel / 60;
        right_pad_x_pos += pad_x_vel / 60;
        left_pad_y_pos += pad_y_vel / 60;
        right_pad_y_pos += pad_y_vel / 60;

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

        // set the positions of paddles in the struct
        left_pad.y = (int) left_pad_y_pos;
        left_pad.x = (int) left_pad_x_pos;
        right_pad.y = (int) right_pad_y_pos;
        right_pad.x = (int) right_pad_x_pos;

        // clear the window
        SDL_RenderClear(rend);

        // draw the left and right paddle to the window
        SDL_RenderCopy(rend, paddle_tex, NULL, &left_pad);
        SDL_RenderCopy(rend, paddle_tex, NULL, &right_pad);
        SDL_RenderCopy(rend, ball_tex, NULL, &ball);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(paddle_tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}


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
