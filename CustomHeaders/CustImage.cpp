#include "CustImage.h"

Image::Image(const std::string &file, SDL_Renderer* ren, int x, int y){
    texture = IMG_LoadTexture(ren, file.c_str());
    
    if (texture == nullptr) {
         std::cout << "LoadTexture" << " Error: " << SDL_GetError() << std::endl;
    }
    
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
}

Image::Image(const std::string &file, SDL_Renderer* ren, int x, int y, int w, int h){
    texture = IMG_LoadTexture(ren, file.c_str());
    
    if (texture == nullptr) {
        std::cout << "LoadTexture" << " Error: " << SDL_GetError() << std::endl;
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

//Render with border
void Image::renderBorder(SDL_Renderer* ren, int w, Uint8 r, Uint8 g, Uint8 b){
    SDL_SetRenderDrawColor( ren, r, g, b, 0xFF );
    SDL_Rect fillRect = { dst.x-w, dst.y-w,dst.w+2*w, dst.h+2*w };
    SDL_RenderFillRect(ren, &fillRect);
    render(ren);
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
