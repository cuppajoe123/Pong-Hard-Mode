#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

// speed in pixels/second
#define SPEED (150)

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

    if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    SDL_Quit();
    return 1;
    }

    TTF_Font *font = TTF_OpenFont("resources/FiraCode-Regular.ttf", 15);
    // color of the text
    SDL_Color color = {255, 255, 255};

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

    SDL_Texture *ball_tex = make_texture("resources/ball.png");
    
    // Code for the ball1
    SDL_Rect ball1;
    SDL_QueryTexture(ball_tex, NULL, NULL, &ball1.w, &ball1.h);
    ball1.w /= 10;
    ball1.h /= 10;
    float ball1_x_pos = (float)(WINDOW_WIDTH) / 2;
    float ball1_y_pos = (float)(WINDOW_HEIGHT) / 2;
    ball1.y = (int) ball1_y_pos;
    ball1.x = (int) ball1_x_pos;

    float ball1_x_vel = SPEED;
    float ball1_y_vel = 75;

    // Code for ball2
    SDL_Rect ball2;
    SDL_QueryTexture(ball_tex, NULL, NULL, &ball2.w, &ball2.h);
    ball2.w /= 10;
    ball2.h /= 10;
    float ball2_x_pos = (float)(WINDOW_WIDTH) / 2;
    float ball2_y_pos = (float)(WINDOW_HEIGHT) / 2;
    ball2.y = (int) ball2_y_pos;
    ball2.x = (int) ball2_x_pos;

    float ball2_x_vel = SPEED;
    float ball2_y_vel = -100;


    // initial velocity set to zero
    float pad_x_vel = 0;
    float pad_y_vel = 0;

    // keep track of which inputs are given
    bool pad_up = false;
    bool pad_down = false;

    bool close_requested = false;

    // counter is the amount of frames the second ball waits to start moving
    int counter = 0;

    long score_int = 0;
    char score_str[80] = "";

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


        if (ball1_y_pos <= 0) {
            ball1_y_pos = 0;
            ball1_y_vel = -ball1_y_vel;
        }
        if (ball1_y_pos >= WINDOW_HEIGHT - ball1.h) {
            ball1_y_pos = WINDOW_HEIGHT - ball1.h;
            ball1_y_vel = -ball1_y_vel;
        }

        // checks if either ball got past the paddles
        if (ball1_x_pos <= 0) break;
        if (ball1_x_pos >= WINDOW_WIDTH) break;
        if (ball2_x_pos <= 0) break;
        if (ball2_x_pos >= WINDOW_WIDTH) break;


        if (ball1_x_pos >= left_pad.x && ball1_x_pos <= left_pad.x + left_pad.w && ball1_y_pos >= left_pad.y && ball1_y_pos <= left_pad.y + left_pad.h) {
            ball1_x_vel = -ball1_x_vel;
            score_int += 10;
        }
        if (ball1_x_pos >= right_pad.x && ball1_x_pos <= right_pad.x + right_pad.w && ball1_y_pos >= right_pad.y && ball1_y_pos <= right_pad.y + right_pad.h) {
            ball1_x_vel = -ball1_x_vel;
            score_int += 10;
        }

        ball1_x_pos += ball1_x_vel / 60;
        ball1_y_pos += ball1_y_vel / 60;

        ball1.x = (int) ball1_x_pos;
        ball1.y = (int) ball1_y_pos;

        if (counter++ >= 120) {
            if (ball2_y_pos <= 0) {
                ball2_y_pos = 0;
                ball2_y_vel = -ball2_y_vel;
            }
            if (ball2_y_pos >= WINDOW_HEIGHT - ball2.h) {
                ball2_y_pos = WINDOW_HEIGHT - ball2.h;
                ball2_y_vel = -ball2_y_vel;
            }

            if (ball2_x_pos >= left_pad.x && ball2_x_pos <= left_pad.x + left_pad.w && ball2_y_pos >= left_pad.y && ball2_y_pos <= left_pad.y + left_pad.h) {
                ball2_x_vel = -ball2_x_vel;
                score_int += 10;
            }
            if (ball2_x_pos >= right_pad.x && ball2_x_pos <= right_pad.x + right_pad.w && ball2_y_pos >= right_pad.y && ball2_y_pos <= right_pad.y + right_pad.h) {
                ball2_x_vel = -ball2_x_vel;
                score_int += 10;
            }

            ball2_x_pos += ball2_x_vel / 60;
            ball2_y_pos += ball2_y_vel / 60;

            ball2.x = (int) ball2_x_pos;
            ball2.y = (int) ball2_y_pos;
        }


        // determine velocity of paddles
        pad_x_vel = pad_y_vel = 0;
        if (pad_up && !pad_down) pad_y_vel = -SPEED - 100;
        if (pad_down && !pad_up) pad_y_vel = SPEED + 100;

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

        // Creates surface and texture for scoreboard
        sprintf(score_str, "%ld", score_int);
        SDL_Surface *scoreboard_surface = TTF_RenderText_Solid(font, score_str, color);
        SDL_Texture *scoreboard_texture = SDL_CreateTextureFromSurface(rend, scoreboard_surface);
        SDL_FreeSurface(scoreboard_surface);
        int score_h, score_w;
        SDL_QueryTexture(scoreboard_texture, NULL, NULL, &score_w, &score_h);
        SDL_Rect score_rect = {0, 0, score_w, score_h};

        // clear the window
        SDL_RenderClear(rend);


        // draw all of the textures to the window
        SDL_RenderCopy(rend, paddle_tex, NULL, &left_pad);
        SDL_RenderCopy(rend, paddle_tex, NULL, &right_pad);
        SDL_RenderCopy(rend, ball_tex, NULL, &ball1);
        SDL_RenderCopy(rend, ball_tex, NULL, &ball2);
        SDL_RenderCopy(rend, scoreboard_texture, NULL, &score_rect);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
        SDL_DestroyTexture(scoreboard_texture);
    }

    // clean up resources before exiting
    TTF_CloseFont(font);
    SDL_DestroyTexture(paddle_tex);
    SDL_DestroyTexture(ball_tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    TTF_Quit();
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
