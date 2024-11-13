#pragma once

#include <string>
#include <SDL2/SDL.h>

#define SDL_COL_BLACK {0, 0, 0, 255}
#define SDL_COL_WHITE {255, 255, 255, 255}
#define SDL_COL_RED {255, 0, 0, 255}
#define SDL_COL_GREEN {0, 255, 0, 255}
#define SDL_COL_BLUE {0, 0, 255, 255}


class SDLWindow {
    private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        int mWidth;
        int mHeight;
    public:
        SDLWindow();
        ~SDLWindow();
        bool init(int width, int height);
        void close();
        SDL_Renderer* getRenderer();
        int width();
        int height();
};

class Polygon {
    protected:
        int mX;
        int mY;
        SDL_Color mColor;
        bool mFillFlag = false;
    public:
        Polygon(int x, int y, SDL_Color color, bool fill) :
            mX(x), mY(y), mColor(color), mFillFlag(fill) {};
        Polygon(int x, int y, SDL_Color color) :
            mX(x), mY(y), mColor(color) {};
        int x() { return mX; }
        int y() { return mY; }
        void move(int x, int y) { mX = x; mY = y; };
        void setColor(SDL_Color color) { mColor = color; };
        virtual void draw(SDL_Renderer* renderer) = 0;
};

class Circle: public Polygon {
    private:
        int mRadius;
    public:
        Circle(int x, int y, int radius, SDL_Color color, bool fill) :
            Polygon(x, y, color, fill), mRadius(radius) {};
        Circle(int x, int y, int radius, SDL_Color color) :
            Polygon(x, y, color), mRadius(radius) {};
        int radius() { return mRadius; }
        void draw(SDL_Renderer* renderer);
};

class Rectangle: public Polygon {
    private:
        int mWidth;
        int mHeight;
    public:
        Rectangle(int x, int y, int width, int height, SDL_Color color, bool fill) :
            Polygon(x, y, color, fill), mWidth(width), mHeight(height) {};
        Rectangle(int x, int y, int width, int height, SDL_Color color) :
            Polygon(x, y, color), mWidth(width), mHeight(height) {};
        int width() { return mWidth; }
        int height() { return mHeight; }
        void draw(SDL_Renderer* renderer);
};

class Point: public Polygon {
    public:
        Point(int x, int y, SDL_Color color) :
            Polygon(x, y, color) {};
        void draw(SDL_Renderer* renderer);
};
