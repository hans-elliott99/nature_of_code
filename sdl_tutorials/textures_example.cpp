
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int WALKING_ANIMATION_FRAMES = 4;



class LTexture {
    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile(std::string path);
        void free(); // deallocate texture
        void render(int x, int y, SDL_Rect* clip);
        void setColor(Uint8 red, Uint8 green, Uint8 blue); // color modulation
        void setBlendMode(SDL_BlendMode blending); // set blending
        void setAlpha(Uint8 alpha); // set alpha modulation
        int getWidth();
        int getHeight();
};


// Globals
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
// background
LTexture gBackgroundTexture;
// circle sprites
LTexture gCircleSpriteTexture;
SDL_Rect gCircleSpriteClips[4]; // sprite clips for sprite sheet (4 total "sprites")
// foo sprite/walking animation
LTexture gStickSpriteTexture;
SDL_Rect gStickSpriteClips[WALKING_ANIMATION_FRAMES];



LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
LTexture::~LTexture() {
    free();
}
void LTexture::free() {
    if (this->mTexture != NULL) {
        SDL_DestroyTexture(this->mTexture);
        this->mTexture = NULL;
        this->mWidth = 0;
        this->mHeight = 0;
    }
}
bool LTexture::loadFromFile(std::string path) {
    // get rid of any preexisting texture
    free();

    // load new texture
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL) {
        printf("Error: Unable to load image %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        // color key the image
        SDL_SetColorKey(surface,   // surface to update
                        SDL_TRUE,  // flag: use color key
                        // color key: maps cyan - RGB(0, 0xFF, 0xFF) - to transparent
                        SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

        // now create the texture from the surface
        this->mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
        if (this->mTexture == NULL) {
            printf("Error: Unable to create texture from %s, SDL_Error: %s\n", path.c_str(), SDL_GetError());
        } else {
            // get image dimensions
            this->mWidth = surface->w;
            this->mHeight = surface->h;
        }
    }
    // free the loaded surface
    SDL_FreeSurface(surface);

    return this->mTexture != NULL;
}
void LTexture::render(int x, int y, SDL_Rect* clip = NULL) {
    // set rendering space and render to screen
    // - renderQuad defines where to place the texture on the screen
    SDL_Rect renderQuad = { x, y, this->mWidth, this->mHeight };
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(gRenderer, this->mTexture, clip, &renderQuad);
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    SDL_SetTextureColorMod(this->mTexture, red, green, blue);
}
void LTexture::setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(this->mTexture, blending);
}
void LTexture::setAlpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(this->mTexture, alpha);
}                
int LTexture::getWidth() {
    return this->mWidth;
}
int LTexture::getHeight() {
    return this->mHeight;
}




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
            // create renderer (vsynced)
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                printf("Error: Renderer could not be created, SDL_Error: %s\n", SDL_GetError());
                success = false;
            } else {
                // initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                // initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if ( !(IMG_Init(imgFlags) & imgFlags) ) {
                    printf("Error: SDL_Image could not be initialized, SDL_Error: %s\n", IMG_GetError());
                    success = false;
                }

            }
        }
    }
    return success;
}

void close_sdl() {
    // free loaded images
    gBackgroundTexture.free();
    gCircleSpriteTexture.free();
    gStickSpriteTexture.free();
    // destroy renderer & window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    // quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}



bool loadMedia() {
    bool success = true;

    if ( ! gBackgroundTexture.loadFromFile("assets/map.png") ) {
        printf("Failed to load Background texture image\n");
        success = false;
    }

    if ( ! gCircleSpriteTexture.loadFromFile("assets/circles.png") ) {
        printf("Failed to load circles texture image\n");
        success = false;
    } else {
        // set standard alpha blending
        gCircleSpriteTexture.setBlendMode(SDL_BLENDMODE_BLEND);

        // set top left sprite clip
        gCircleSpriteClips[0].x = 0;   gCircleSpriteClips[0].y = 0;
        gCircleSpriteClips[0].w = 100; gCircleSpriteClips[0].h = 100;
        // set top right sprite clip
        gCircleSpriteClips[1].x = 100; gCircleSpriteClips[1].y = 0;
        gCircleSpriteClips[1].w = 100; gCircleSpriteClips[1].h = 100;
        // set bottom left sprite clip
        gCircleSpriteClips[2].x = 0;   gCircleSpriteClips[2].y = 100;
        gCircleSpriteClips[2].w = 100; gCircleSpriteClips[2].h = 100;
        // set bottom right sprite clip
        gCircleSpriteClips[3].x = 100; gCircleSpriteClips[3].y = 100;
        gCircleSpriteClips[3].w = 100; gCircleSpriteClips[3].h = 100;
    }

    if ( ! gStickSpriteTexture.loadFromFile("assets/walkingstick.png")) {
        printf("Failed to load stick figure texture image\n");
    } else {
        // set sprite clips
        gStickSpriteClips[0].x = 0;   gStickSpriteClips[0].y = 0;
        gStickSpriteClips[0].w = 64;  gStickSpriteClips[0].h = 205;

        gStickSpriteClips[1].x = 64;  gStickSpriteClips[1].y = 0;
        gStickSpriteClips[1].w = 64;  gStickSpriteClips[1].h = 205;

        gStickSpriteClips[2].x = 128; gStickSpriteClips[2].y = 0;
        gStickSpriteClips[2].w = 64;  gStickSpriteClips[2].h = 205;

        gStickSpriteClips[3].x = 192; gStickSpriteClips[3].y = 0;
        gStickSpriteClips[3].w = 64;  gStickSpriteClips[3].h = 205;
    }

    return success;
}




int main( int argc, char* args[] )
{
    bool quit = false;
    SDL_Event e;
    // for color modulation of circle sptires
    Uint8 r = 255, g = 255, b = 255, a = 255;
    // for walking animation
    int frame = 0, anim_x = 0, anim_it = 0;
    const int frame_denom = 17;

    // initialize SDL
    quit = !init_sdl();
    if (quit) {
        printf("Failed to initialize\n");
        goto cleanup;
    }
    quit = !loadMedia();
    if (quit) {
        printf("Failed to load media\n");
        goto cleanup;
    }

    while (!quit) {
        // handle events on queue - returns 0 if empty queue
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        r += 32;
                        break;
                    case SDLK_w:
                        g += 32;
                        break;
                    case SDLK_e:
                        b += 32;
                        break;
                    case SDLK_a:
                        a += 32;
                        break;
                    default:
                        break;
                }

            }
        }
        // clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        // render BACKGROUND texture to screen
        gBackgroundTexture.render(0, 0);

        // modulate CIRCLE SPRITES colors and alpha
        gCircleSpriteTexture.setColor(r, g, b);
        gCircleSpriteTexture.setAlpha(a);
        // render top left sprite
        gCircleSpriteTexture.render(0, 0, &gCircleSpriteClips[0]);
        // render top right
        gCircleSpriteTexture.render(SCREEN_WIDTH - gCircleSpriteClips[1].w,
                                    0,
                                    &gCircleSpriteClips[1]);
        // render bottom left
        gCircleSpriteTexture.render(0,
                                    SCREEN_HEIGHT - gCircleSpriteClips[2].h,
                                    &gCircleSpriteClips[2]);
        // render bottom right
        gCircleSpriteTexture.render(SCREEN_WIDTH - gCircleSpriteClips[3].w,
                                    SCREEN_HEIGHT - gCircleSpriteClips[3].h,
                                    &gCircleSpriteClips[3]);
        
        // render STICK FIGURE walking animation
        gStickSpriteTexture.render(SCREEN_WIDTH - anim_x, // x
                                   SCREEN_HEIGHT - gStickSpriteClips[0].h, // y
                                   &gStickSpriteClips[frame/frame_denom] // clip
                                   );
        frame++;
        anim_it++;
        if (frame/frame_denom >= WALKING_ANIMATION_FRAMES) {
            frame = 0;
        }
        if (anim_it == frame_denom) {
            anim_x += gStickSpriteClips[0].w;
            if (anim_x >= SCREEN_WIDTH + 2 * gStickSpriteClips[0].w) {
                anim_x = 0; // wrap around
            }
            anim_it = 0;
        }

        // // render filled red squre
        // SDL_Rect fillRect = { SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 }; // x, y, w, h
        // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
        // SDL_RenderFillRect(gRenderer, &fillRect);

        // // render green outline
        // SDL_Rect outlineRect = { SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3 };
        // SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
        // SDL_RenderDrawRect(gRenderer, &outlineRect);

        // // draw horizontal line
        // SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
        // SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);

        // // draw vertical line of yellow dots
        // SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
        // for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
        //     SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH/2, i);
        // }

        // update screen
        SDL_RenderPresent(gRenderer);
    }

 cleanup:
    close_sdl();

	return quit ? 1 : 0;
}

