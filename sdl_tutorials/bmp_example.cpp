
/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT = 0,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};


// Globals
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL; // surface contained by window
SDL_Surface* gHelloWorld = NULL;    // image we will load onto screen (additional surface)
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL]; // array of surfaces for key presses
SDL_Surface* gCurrentSurface = NULL; // current surface to render


// initialize SDL and initialize the window and its surface
bool init_sdl() {
    bool success = true;
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        printf("Error: SDL could not initialize, SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        // create window
        gWindow = SDL_CreateWindow("SDL Tutorial",
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH, SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Error: Window could not be created, SDL_Error: %s\n", SDL_GetError()); 
            success = false;
        } else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

void close_sdl() {
    SDL_FreeSurface(gHelloWorld);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
}


/*
 * EXAMPLES
 */


// just open a window and color it white
void helloSDL() {
    //Fill the surface with white
	SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0xFF, 0xFF, 0xFF ) );
    //Update the surface
	SDL_UpdateWindowSurface( gWindow );
}


SDL_Surface* loadSurfaceBMP(std::string path) {
    SDL_Surface* optimized = NULL;

    // load image
    SDL_Surface* loaded = SDL_LoadBMP(path.c_str());
    if (loaded == NULL) {
        printf("Error: Unable to load image %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        /* Convert to match screen format
         * This is an optimization since otherwise SDL will convert the image
         *  every time it is blitted, so might as well do it once up front
        */
        optimized = SDL_ConvertSurface(loaded, gScreenSurface->format, 0);
        if (optimized == NULL) {
            printf("Error: Unable to optimize image %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
        }
        // free original surface
        SDL_FreeSurface(loaded);
    }
    return optimized;
}


// load BMP files and assign to surfaces
bool loadAllBMP() {
    bool success = true;

    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurfaceBMP("assets/default.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurfaceBMP("assets/up.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurfaceBMP("assets/down.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurfaceBMP("assets/left.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurfaceBMP("assets/right.bmp");

    for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
        if (gKeyPressSurfaces[i] == NULL) {
            success = false;
            break; // if any failures, end
        }
    }
    return success;
}

SDL_Surface* surfaceFromKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            return gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
        case SDLK_DOWN:
            return gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
        case SDLK_LEFT:
            return gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
        case SDLK_RIGHT:
            return gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
        default:
            return gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
    }
}


int main( int argc, char* args[] )
{
    bool quit = false;
    SDL_Event e;
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;

    // initialize SDL
    quit = !init_sdl();
    if (quit) {
        printf("Failed to initialize\n");
        goto cleanup;
    }

    gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

    // examples
    helloSDL();
    loadAllBMP();

    while (!quit) {
        // handle events on queue - returns 0 if empty queue
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                printf("Key pressed: %s\n", SDL_GetKeyName(e.key.keysym.sym));
                gCurrentSurface = surfaceFromKeyPress(e.key.keysym.sym);
                // apply current image - stretch to window
                // SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
                SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
                // update window surface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

 cleanup:
    close_sdl();

	return quit ? 1 : 0;
}

