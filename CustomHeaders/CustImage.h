#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

class Image{
private:
    SDL_Texture* texture;
    SDL_Rect dst;
public:
    Image(const std::string &file, SDL_Renderer* ren, int x, int y);
    Image(const std::string &file, SDL_Renderer* ren, int x, int y, int w, int h);
    bool successfullLoad();
    void render(SDL_Renderer* ren);
    void renderBorder(SDL_Renderer* ren, int w, Uint8 r, Uint8 g, Uint8 b);
    void centre(int screenW, int screenH, bool horizontal, bool vertical);
    void cleanup();
    
    int getx();
    int gety();
    int getw();
    int geth();
};
