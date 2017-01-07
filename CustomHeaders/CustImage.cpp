#include "CustImage.h"

Image::Image(const std::string &file, SDL_Renderer* ren, int x, int y){
    texture = IMG_LoadTexture(ren, file.c_str());
    
    if (texture == nullptr) {
        logSDLError(std::cout, "LoadTexture");
    }
    
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
}

Image::Image(const std::string &file, SDL_Renderer* ren, int x, int y, int w, int h){
    texture = IMG_LoadTexture(ren, file.c_str());
    
    if (texture == nullptr) {
        logSDLError(std::cout, "LoadTexture");
    }
    
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
}

bool Image::successfullLoad(){
    if (texture == nullptr){
        return false;
    }else{
        return true;
    }
}

void Image::render(SDL_Renderer* ren){
    SDL_RenderCopy(ren, texture, NULL, &dst);
}

void Image::centre(int screenW, int screenH, bool horizontal, bool vertical){
    if (horizontal){
        dst.x = screenW / 2 - dst.w / 2;
    }
    
    if (vertical){
        dst.y = screenH/2 - dst.h/2;
    }
}

void Image::cleanup(){
    SDL_DestroyTexture(texture);
}

int Image::getx(){
    return dst.x;
}
int Image::gety(){
    return dst.y;
}
int Image::getw(){
    return dst.w;
}
int Image::geth(){
    return dst.h;
}
