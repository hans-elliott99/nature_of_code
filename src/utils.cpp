#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "utils.hpp"

SDLWindow::SDLWindow() {
    SDL_Window* mWindow = NULL;
    SDL_Renderer* mRenderer = NULL;
    int mWidth = 0;
    int mHeight = 0;
}

SDLWindow::~SDLWindow() {
    close();
}

bool SDLWindow::init(int width, int height) {
    bool success = true;
    mWidth = width;
    mHeight = height;
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("Error: SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // create window
        mWindow = SDL_CreateWindow("SDL Tutorial",
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   mWidth, mHeight,
                                   SDL_WINDOW_SHOWN);
        if (mWindow == NULL) {
            printf("Error: Window could not be created, SDL_Error: %s\n", SDL_GetError()); 
            success = false;
        } else {
            // create renderer (vsynced)
            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (mRenderer == NULL) {
                printf("Error: Renderer could not be created, SDL_Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // initialize renderer color
                SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
            }
        }
    }
    return success;
}

void SDLWindow::close() {
    // destroy renderer & window
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    mRenderer = NULL;
    mWindow = NULL;
    // quit SDL subsystems
    SDL_Quit();
}

SDL_Renderer* SDLWindow::getRenderer() {
    return mRenderer;
}

int SDLWindow::width() {
    return mWidth;
}

int SDLWindow::height() {
    return mHeight;
}


// CIRCLE
void Circle::draw(SDL_Renderer* renderer) {
    const int diameter = (mRadius * 2);
    int x = (mRadius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    while (x >= y) {

        if (mFillFlag) {
            SDL_RenderDrawLine(renderer, mX + x, mY + y, mX - x, mY + y);
            SDL_RenderDrawLine(renderer, mX + x, mY - y, mX - x, mY - y);
            SDL_RenderDrawLine(renderer, mX + y, mY + x, mX - y, mY + x);
            SDL_RenderDrawLine(renderer, mX + y, mY - x, mX - y, mY - x);
        } else {
            SDL_RenderDrawPoint(renderer, mX + x, mY - y);
            SDL_RenderDrawPoint(renderer, mX + x, mY + y);
            SDL_RenderDrawPoint(renderer, mX - x, mY - y);
            SDL_RenderDrawPoint(renderer, mX - x, mY + y);
            SDL_RenderDrawPoint(renderer, mX + y, mY - x);
            SDL_RenderDrawPoint(renderer, mX + y, mY + x);
            SDL_RenderDrawPoint(renderer, mX - y, mY - x);
            SDL_RenderDrawPoint(renderer, mX - y, mY + x);
        }

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }
        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}


// RECTANGLE
void Rectangle::draw(SDL_Renderer *renderer) {
    SDL_Rect fillRect = { mX, mY, mWidth, mHeight };
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    if (mFillFlag) {
        SDL_RenderFillRect(renderer, &fillRect);
    } else {
        SDL_RenderDrawRect(renderer, &fillRect);
    }
}


// POINT
void Point::draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_RenderDrawPoint(renderer, mX, mY);
}
