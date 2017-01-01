#pragma once
#include "Core.h"
#ifdef _WIN32
	#undef main				//get rid of double defined main from SDL2 libs
#endif

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window *MainWindow = nullptr;		// set them to null ptr
SDL_Renderer *MainRenderer = nullptr;


/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message to
 * @param msg The function which caused the error
 */
void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " Error: " << SDL_GetError() << std::endl;
	system("PAUSE");
}

//Recursively destroys all passed arguments
template<typename T, typename... Args> void cleanup(T *t, Args&&... args) {
    cleanup(t);
    cleanup(std::forward<Args>(args)...);
}

template<> inline void cleanup<SDL_Window>(SDL_Window *win) {
    if (!win)
        return;
    SDL_DestroyWindow(win);
}
template<> inline void cleanup<SDL_Renderer>(SDL_Renderer *ren) {
    if (!ren)
        return;
    SDL_DestroyRenderer(ren);
}
template<> inline void cleanup<SDL_Texture>(SDL_Texture *tex) {
    if (!tex)
        return;
    SDL_DestroyTexture(tex);
}
template<> inline void cleanup<SDL_Surface>(SDL_Surface *surf) {
    if (!surf)
        return;
    SDL_FreeSurface(surf);
}

//This function will set a given window and renderer
// if you would like an accelerated renderer, or a v-sync enabled renderer, pass true for the third and fourth parameters

int generalInit(const char *MainWindowName, bool hardAcc, bool vSync) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError(std::cout, "SDL_Init");
		return 0;
	}
    
	MainWindow = SDL_CreateWindow(MainWindowName , 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);		// make the main window at 100,100 with size 640x480, and show it
	if (MainWindow == nullptr) {
        logSDLError(std::cout, "SDL_CreateWindow");
		SDL_Quit();
		return 0;
	}
    
    if (hardAcc && !vSync){
        MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);		// various conditions for what the user wants
    }else if (!hardAcc && vSync){
        MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    }else if (hardAcc && vSync){
        MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }else{
        MainRenderer = SDL_CreateRenderer(MainWindow, -1, 0);
    }
    
	if (MainRenderer == nullptr) {
        cleanup(MainWindow);
        logSDLError(std::cout, "SDL_CreateRenderer");
		SDL_Quit();
		return 0;
	}
	return 1;
}

/**
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
    
    //Initialize to nullptr to avoid dangling pointer issues
    SDL_Texture *texture = nullptr;
    //Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    
    //If the loading went ok, convert to texture and return the texture
    if (loadedImage != nullptr){
        
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        cleanup(loadedImage);
        
        //Make sure converting went ok too
        if (texture == nullptr){
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(std::cout, "LoadBMP");
    }
    
    return texture;
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    
    //Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}
