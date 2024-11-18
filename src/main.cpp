#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "utils.hpp"
#include <cmath>


float map(float value, float in_min, float in_max, float out_min, float out_max) {
    return (value - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

float lerp(float alpha, float a, float b) {
    // (1 - alpha)*a + alpha*b
    return a + alpha * (b - a);
}


class RandomWalker: public Circle {
    private:
        const int mWindowHeight;
        const int mWindowWidth;
        const int perlin_size = 4095;
        float perlin[4096];
        float tx = 0.01;
        float ty = 1000;
        float scaled_cosine(float x) {
            return 0.5 * (1.0 - cos(x * M_PI));
        }
        float perlin_noise(float x, float y=0, float z=0) {
            // https://github.com/processing/p5.js/blob/33883e5a326fcc3c1ba73a50d74b910af077a688/src/math/noise.js#L254
            const int perlin_ywrapb = 4;
            const int perlin_ywrap = 1 << perlin_ywrapb;
            const int perlin_zwrapb = 8;
            const int perlin_zwrap = 1 << perlin_zwrapb;
            const int perlin_octaves = 4;
            const float perlin_amp_falloff = 0.5;

            int xi = (int)floor(x);
            float xf = x - xi;
            int yi = (int)floor(y);
            float yf = y - yi;
            int zi = (int)floor(z);
            float zf = z - zi;

            float r = 0, ampl = 0.5;
            float rxf, ryf;
            int of;
            float n1, n2, n3;

            for (int i = 0; i < perlin_octaves; i++) {
                of = xi + (yi << perlin_ywrapb) + (zi << perlin_zwrapb);

                rxf = scaled_cosine(xf);
                ryf = scaled_cosine(yf);

                n1 = perlin[of & perlin_size];
                n1 += rxf * (perlin[(of + 1) & perlin_size] - n1);
                n2 = perlin[(of + perlin_ywrap) & perlin_size];
                n2 += rxf * (perlin[(of + perlin_ywrap + 1) & perlin_size] - n2);
                n1 += ryf * (n2 - n1);

                of += perlin_zwrap;
                n2 = perlin[of & perlin_size];
                n2 += rxf * (perlin[(of + 1) & perlin_size] - n2);
                n3 = perlin[(of + perlin_ywrap) & perlin_size];
                n3 += rxf * (perlin[(of + perlin_ywrap + 1) & perlin_size] - n3);
                n2 += ryf * (n3 - n2);

                n1 += scaled_cosine(zf) * (n2 - n1);

                r += n1 * ampl;
                ampl *= perlin_amp_falloff;
                xi <<= 1;
                xf *= 2;
                yi <<= 1;
                yf *= 2;
                zi <<= 1;
                zf *= 2;

                if (xf >= 1.0) {
                    xi++;
                    xf--;
                }
                if (yf >= 1.0) {
                    yi++;
                    yf--;
                }
                if (zf >= 1.0) {
                    zi++;
                    zf--;
                }
            }
            return r;
        }
    public:
        RandomWalker(int x, int y, SDL_Color color, int window_height, int window_width) :
            Circle(x, y, 2, color),
            mWindowHeight(window_height),
            mWindowWidth(window_width)
        {
            // init perlin array
            for (int i = 0; i < perlin_size; i++) {
                perlin[i] = (float)rand() / RAND_MAX;
            }
        };
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
        void perlinStep(float step_size = 0.01) {
            float noiseX = perlin_noise(tx);
            float noiseY = perlin_noise(ty);
            mX = map(perlin_noise(tx), 0, 1, 0, mWindowWidth);
            mY = map(perlin_noise(ty), 0, 1, 0, mWindowHeight);
            tx += step_size;
            ty += step_size;
            if (tx > 1e6) {
                tx = 0.01;
            }
            if (ty > 1e6) {
                ty = 1000;
            }
        }
};

int main( int argc, char* args[] )
{
    srand(2178);
    bool quit = false;
    SDL_Event e;
    SDLWindow window;
    SDL_Renderer* renderer;
    // initialize SDL
    if (!window.init(600, 520)) {
        printf("Failed to initialize\n");
        return 1;
    }
    renderer = window.getRenderer();

    RandomWalker walker_red = RandomWalker(
        window.width()/3, window.height()/2, SDL_COL_RED,
        window.height(), window.width()
    );
    RandomWalker walker_grn = RandomWalker(
        2*window.width()/3, window.height()/2, SDL_COL_GREEN,
        window.height(), window.width()
    );
    RandomWalker walker_blu = RandomWalker(
        window.width()/2, window.height()/3, SDL_COL_BLUE,
        window.height(), window.width()
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
        walker_blu.perlinStep(0.005);
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


