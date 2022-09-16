/*
 * This program was written using the SDL2 graphics library. All functions and data types beginning in SDL* are from the graphics library:
 * https://github.com/libsdl-org/SDL
 *
 * The font used for the user interface is Fira Code: https://github.com/tonsky/FiraCode
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphics.h"
#include "leader_board.h"
#include "wasm.h"

#ifdef __EMSCRIPTEN__
static EM_BOOL mainloop(double time, void *args);

struct main_state {
    SDL_Texture *paddle_tex;
    SDL_Texture *ball_tex;

    SDL_Rect left_pad;
    SDL_Rect right_pad;
    SDL_Rect ball1;
    SDL_Rect ball2;

    int ball1_x_vel;
    int ball1_y_vel;
    int ball2_x_vel;
    int ball2_y_vel;

    int pad_x_vel;
    int pad_y_vel;
    bool pad_up;
    bool pad_down;

    char *score_str;
};
#endif

int main(void)
{
    init_graphics();

#ifdef __EMSCRIPTEN__
    struct main_state args;
#endif

    SDL_Texture *paddle_tex = make_texture_img("resources/paddle.png");
    // struct to hold the position and size of both paddle sprites
    SDL_Rect left_pad, right_pad;

    // get and scale the dimensions of paddle_texture
    SDL_QueryTexture(paddle_tex, NULL, NULL, &left_pad.w, &left_pad.h);
    left_pad.w /= 2;
    left_pad.h /= 2;
    right_pad.w = left_pad.w;
    right_pad.h = left_pad.h;

    // start left paddle sprite in the left side of the screen with some space from the window border
    left_pad.x = (WINDOW_WIDTH - (WINDOW_WIDTH - 10));
    left_pad.y = (WINDOW_HEIGHT - left_pad.h) / 2;

    // start right paddle sprite on right side of the screen with some space from the window border
    right_pad.x = (WINDOW_WIDTH - 10);
    right_pad.y = (WINDOW_HEIGHT - left_pad.h) / 2;

    SDL_Texture *ball_tex = make_texture_img("resources/ball.png");
    
    // Code for the ball1
    SDL_Rect ball1;
    SDL_QueryTexture(ball_tex, NULL, NULL, &ball1.w, &ball1.h);
    ball1.w /= 10;
    ball1.h /= 10;
    ball1.x = (WINDOW_WIDTH) / 2;
    ball1.y = (WINDOW_HEIGHT) / 2;

    int ball1_x_vel = SPEED;
    int ball1_y_vel = 75;

    // Code for ball2
    SDL_Rect ball2;
    SDL_QueryTexture(ball_tex, NULL, NULL, &ball2.w, &ball2.h);
    ball2.w /= 10;
    ball2.h /= 10;
    ball2.x = (WINDOW_WIDTH) / 2;
    ball2.y = (WINDOW_HEIGHT) / 2;

    int ball2_x_vel = SPEED;
    int ball2_y_vel = -100;


    // initial velocity set to zero
    int pad_x_vel = 0;
    int pad_y_vel = 0;

    // keep track of which inputs are given
    bool pad_up = false;
    bool pad_down = false;

    bool close_requested = false;


    char score_str[80] = "";

#if __EMSCRIPTEN__
    args.paddle_tex = paddle_tex;
    args.ball_tex = ball_tex;
    args.left_pad = left_pad;
    args.right_pad = right_pad;
    args.ball1 = ball1;
    args.ball2 = ball2;
    args.ball1_x_vel = ball1_x_vel;
    args.ball1_y_vel = ball1_y_vel;
    args.ball2_x_vel = ball2_x_vel;
    args.ball2_y_vel = ball2_y_vel;
    args.pad_x_vel = pad_x_vel;
    args.pad_y_vel = pad_y_vel;
    args.pad_up = pad_up;
    args.pad_down = pad_down;
    args.score_str = score_str;
#endif


    // start screen loop
    if (start_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(paddle_tex);
        SDL_DestroyTexture(ball_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        TTF_Quit();
    }
    // start username_screen
    char username[15] = "";
    if (username_screen(username) == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(paddle_tex);
        SDL_DestroyTexture(ball_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        TTF_Quit();
    }

#ifdef __EMSCRIPTEN__
    emscripten_request_animation_frame_loop(mainloop, &args);
    SDL_DestroyTexture(args.paddle_tex);
    SDL_DestroyTexture(args.ball_tex);
    return 0;
#endif
        
    // main game loop
    while (!close_requested) {
        // counter is the amount of frames the second ball waits to start moving
        static int counter = 0;
        static long score_int = 0;
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    TTF_CloseFont(font);
                    SDL_DestroyTexture(paddle_tex);
                    SDL_DestroyTexture(ball_tex);
                    SDL_DestroyRenderer(rend);
                    SDL_DestroyWindow(win);
                    TTF_Quit();
                    SDL_Quit();
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


        // checks if ball should bounce off top or bottom of screen
        if (ball1.y <= 0) {
            ball1.y = 0;
            ball1_y_vel = -ball1_y_vel;
        }
        if (ball1.y >= WINDOW_HEIGHT - ball1.h) {
            ball1.y = WINDOW_HEIGHT - ball1.h;
            ball1_y_vel = -ball1_y_vel;
        }

        // checks if either ball got past the paddles
        if (ball1.x <= 0) break;
        if (ball1.x >= WINDOW_WIDTH) break;
        if (ball2.x <= 0) break;
        if (ball2.x >= WINDOW_WIDTH) break;


        // collision detection for paddles
        if (ball1.x >= left_pad.x && ball1.x <= left_pad.x + left_pad.w && ball1.y >= left_pad.y && ball1.y <= left_pad.y + left_pad.h) {
            ball1_x_vel = -ball1_x_vel;
            score_int += 10;
        }
        if (ball1.x + ball1.w >= right_pad.x && ball1.x + ball1.w <= right_pad.x + right_pad.w && ball1.y >= right_pad.y && ball1.y <= right_pad.y + right_pad.h) {
            ball1_x_vel = -ball1_x_vel;
            score_int += 10;
        }

        ball1.x += ball1_x_vel / 60;
        ball1.y += ball1_y_vel / 60;


        if (counter++ >= 120) {
            // same code as for ball1 
            if (ball2.y <= 0) {
                ball2.y = 0;
                ball2_y_vel = -ball2_y_vel;
            }
            if (ball2.y >= WINDOW_HEIGHT - ball2.h) {
                ball2.y = WINDOW_HEIGHT - ball2.h;
                ball2_y_vel = -ball2_y_vel;
            }

            if (ball2.x >= left_pad.x && ball2.x <= left_pad.x + left_pad.w && ball2.y >= left_pad.y && ball2.y <= left_pad.y + left_pad.h) {
                ball2_x_vel = -ball2_x_vel;
                score_int += 10;
            }
            if (ball2.x + ball2.w >= right_pad.x && ball2.x + ball2.w <= right_pad.x + right_pad.w && ball2.y >= right_pad.y && ball2.y <= right_pad.y + right_pad.h) {
                ball2_x_vel = -ball2_x_vel;
                score_int += 10;
            }

            ball2.x += ball2_x_vel / 60;
            ball2.y += ball2_y_vel / 60;
        }


        // determine velocity of paddles
        pad_x_vel = pad_y_vel = 0;
        if (pad_up && !pad_down) pad_y_vel = -SPEED - 100;
        if (pad_down && !pad_up) pad_y_vel = SPEED + 100;

        // update positions of paddles
        left_pad.x += pad_x_vel / 60;
        right_pad.x += pad_x_vel / 60;
        left_pad.y += pad_y_vel / 60;
        right_pad.y += pad_y_vel / 60;

        // collision detection with bounds
        // only checks left paddle because both paddles will have the same y position
        if (left_pad.y <= 0) {
            left_pad.y = 0;
            right_pad.y = 0;
        }
        if (left_pad.y >= WINDOW_HEIGHT - left_pad.h) {
            left_pad.y = WINDOW_HEIGHT - left_pad.h;
            right_pad.y = WINDOW_HEIGHT - right_pad.h;
        }


        // Creates surface and texture for scoreboard
        sprintf(score_str, "%ld", score_int);
        SDL_Texture *scoreboard_texture = make_texture_str(score_str);
        SDL_Rect score_rect = make_textbox(scoreboard_texture, 0, 0, 1, 0x0);

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

    save_user_data(score_str, username);

    if (leader_board_screen() == 1) {
        TTF_CloseFont(font);
        SDL_DestroyTexture(paddle_tex);
        SDL_DestroyTexture(ball_tex);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        TTF_Quit();
        SDL_Quit();
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

#ifdef __EMSCRIPTEN__
static EM_BOOL mainloop(double time, void *args) {
    struct main_state *state = (struct main_state *)args;
    static int counter = 0;
    static long score_int = 0;
    // process events
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            return EM_FALSE;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    state->pad_up = true;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    state->pad_down = true;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    state->pad_up = false;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    state->pad_down = false;
                    break;
            }
            break;
    }


    // checks if state->ball should bounce off top or bottom of screen
    if (state->ball1.y <= 0) {
        state->ball1.y = 0;
        state->ball1_y_vel = -state->ball1_y_vel;
    }
    if (state->ball1.y >= WINDOW_HEIGHT - state->ball1.h) {
        state->ball1.y = WINDOW_HEIGHT - state->ball1.h;
        state->ball1_y_vel = -state->ball1_y_vel;
    }

    // checks if either state->ball got past the paddles
    if (state->ball1.x <= 0) return EM_FALSE;
    if (state->ball1.x >= WINDOW_WIDTH) return EM_FALSE;
    if (state->ball2.x <= 0) return EM_FALSE;
    if (state->ball2.x >= WINDOW_WIDTH) return EM_FALSE;


    // collision detection for paddles
    if (state->ball1.x >= state->left_pad.x && state->ball1.x <= state->left_pad.x + state->left_pad.w && state->ball1.y >= state->left_pad.y && state->ball1.y <= state->left_pad.y + state->left_pad.h) {
        state->ball1_x_vel = -state->ball1_x_vel;
        score_int += 10;
    }
    if (state->ball1.x + state->ball1.w >= state->right_pad.x && state->ball1.x + state->ball1.w <= state->right_pad.x + state->right_pad.w && state->ball1.y >= state->right_pad.y && state->ball1.y <= state->right_pad.y + state->right_pad.h) {
        state->ball1_x_vel = -state->ball1_x_vel;
        score_int += 10;
    }

    state->ball1.x += state->ball1_x_vel / 60;
    state->ball1.y += state->ball1_y_vel / 60;


    if (counter++ >= 120) {
        // same code as for state->ball1 
        if (state->ball2.y <= 0) {
            state->ball2.y = 0;
            state->ball2_y_vel = -state->ball2_y_vel;
        }
        if (state->ball2.y >= WINDOW_HEIGHT - state->ball2.h) {
            state->ball2.y = WINDOW_HEIGHT - state->ball2.h;
            state->ball2_y_vel = -state->ball2_y_vel;
        }

        if (state->ball2.x >= state->left_pad.x && state->ball2.x <= state->left_pad.x + state->left_pad.w && state->ball2.y >= state->left_pad.y && state->ball2.y <= state->left_pad.y + state->left_pad.h) {
            state->ball2_x_vel = -state->ball2_x_vel;
            score_int += 10;
        }
        if (state->ball2.x + state->ball2.w >= state->right_pad.x && state->ball2.x + state->ball2.w <= state->right_pad.x + state->right_pad.w && state->ball2.y >= state->right_pad.y && state->ball2.y <= state->right_pad.y + state->right_pad.h) {
            state->ball2_x_vel = -state->ball2_x_vel;
            score_int += 10;
        }

        state->ball2.x += state->ball2_x_vel / 60;
        state->ball2.y += state->ball2_y_vel / 60;
    }


    // determine velocity of paddles
    state->pad_x_vel = state->pad_y_vel = 0;
    if (state->pad_up && !state->pad_down) state->pad_y_vel = -SPEED - 100;
    if (state->pad_down && !state->pad_up) state->pad_y_vel = SPEED + 100;

    // update positions of paddles
    state->left_pad.x += state->pad_x_vel / 60;
    state->right_pad.x += state->pad_x_vel / 60;
    state->left_pad.y += state->pad_y_vel / 60;
    state->right_pad.y += state->pad_y_vel / 60;

    // collision detection with bounds
    // only checks left paddle because both paddles will have the same y position
    if (state->left_pad.y <= 0) {
        state->left_pad.y = 0;
        state->right_pad.y = 0;
    }
    if (state->left_pad.y >= WINDOW_HEIGHT - state->left_pad.h) {
        state->left_pad.y = WINDOW_HEIGHT - state->left_pad.h;
        state->right_pad.y = WINDOW_HEIGHT - state->right_pad.h;
    }


    // Creates surface and texture for scoreboard
    sprintf(state->score_str, "%ld", score_int);
    SDL_Texture *scoreboard_texture = make_texture_str(state->score_str);
    SDL_Rect score_rect = make_textbox(scoreboard_texture, 0, 0, 1, 0x0);

    // clear the window
    SDL_RenderClear(rend);

    // draw all of the textures to the window
    SDL_RenderCopy(rend, state->paddle_tex, NULL, &state->left_pad);
    SDL_RenderCopy(rend, state->paddle_tex, NULL, &state->right_pad);
    SDL_RenderCopy(rend, state->ball_tex, NULL, &state->ball1);
    SDL_RenderCopy(rend, state->ball_tex, NULL, &state->ball2);
    SDL_RenderCopy(rend, scoreboard_texture, NULL, &score_rect);
    SDL_RenderPresent(rend);

    SDL_DestroyTexture(scoreboard_texture);
    return EM_TRUE;
}
#endif
