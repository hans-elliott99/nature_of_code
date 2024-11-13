#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "utils.hpp"


class RandomWalker: public Circle {
    public:
        RandomWalker(int x, int y, SDL_Color color) : Circle(x, y, 2, color) {};
        // step up, down, left, or right
        void step(int magnitude = 1) {
            int choice = rand() % 4;
            if (choice == 0) {
                mX = mX + magnitude;
            } else if (choice == 1) {
                mX = mX - magnitude;
            } else if (choice == 2) {
                mY = mY + magnitude;
            } else {
                mY = mY - magnitude;
            }
        }
        // step in any direction - up, down, left, right, and diagonals
        void step8(int magnitude = 1) {
            int choice = rand() % 8;
            if (choice == 0) {
                mX = mX + magnitude;
            } else if (choice == 1) {
                mX = mX - magnitude;
            } else if (choice == 2) {
                mY = mY + magnitude;
            } else if (choice == 3) {
                mY = mY - magnitude;
            } else if (choice == 4) {
                mX = mX + magnitude;
                mY = mY + magnitude;
            } else if (choice == 5) {
                mX = mX - magnitude;
                mY = mY + magnitude;
            } else if (choice == 6) {
                mX = mX + magnitude;
                mY = mY - magnitude;
            } else {
                mX = mX - magnitude;
                mY = mY - magnitude;
            }
        }
        void perlinStep() {
            // TODO
            // https://rtouti.github.io/graphics/perlin-noise-algorithm
        }
};

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

    RandomWalker walker_red = RandomWalker(
        window.width()/3, window.height()/2, {255, 0, 0, 100}
    );
    RandomWalker walker_grn = RandomWalker(
        2*window.width()/3, window.height()/2, {0, 0, 255, 100}
    );
    RandomWalker walker_blu = RandomWalker(
        window.width()/2, window.height()/3, {0, 255, 0, 100}
    );

    // clear screen once
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                // handle events
            }
        }
        // update shapes
        walker_red.step(2);
        walker_grn.step8(2);
        walker_blu.step8(2);
        // draw shapes
        walker_red.draw(renderer);
        walker_grn.draw(renderer);
        walker_blu.draw(renderer);
        // update screen
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    window.close();
}


