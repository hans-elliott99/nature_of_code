#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "utils.hpp"


int main( int argc, char* args[] )
{
    bool quit = false;
    SDL_Event e;
    SDLWindow window;
    SDL_Renderer* renderer;
    // initialize SDL
    if (!window.init(640, 480)) {
        printf("Failed to initialize\n");
        return 1;
    }
    renderer = window.getRenderer();

    Circle circle(0, 240, 100, SDL_COL_RED, true);
    Circle circle2(0, 240, 50, SDL_COL_GREEN, true);
    Rectangle rect(0, 240, 80, 80, SDL_COL_BLUE, false);
    int anim_frame = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                // handle events
            }
        }
        // clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        // move shapes
        circle.move(circle.x() + 3, circle.y());
        circle2.move(circle2.x() + 4, circle2.y());
        rect.move(rect.x() + 2, rect.y());
        // wrap around screen
        if (circle.x() - circle.radius() >= window.width()) {
            circle.move(0 - circle.radius(), 240);
        }
        if (circle2.x() - circle2.radius() >= window.width()) {
            circle2.move(0 - circle.radius(), 240);
        }
        if (rect.x() >= window.width()) {
            rect.move(0 - rect.width(), 240);
        }
        // draw circle
        circle.draw(renderer);
        circle2.draw(renderer);
        rect.draw(renderer);
        // update screen
        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }

    window.close();
}


