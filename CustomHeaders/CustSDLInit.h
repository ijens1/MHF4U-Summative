#pragma once
#include "Core.h"
#ifdef _WIN32
	#undef main				//get rid of double defined main from SDL2 libs
#endif

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

SDL_Window *MainWindow = nullptr;	
SDL_Renderer *MainRenderer = nullptr;
std::vector<Question*> MainQuestionList;		// a randomized list of all of the math questions


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
template<> inline void cleanup<Mix_Chunk>(Mix_Chunk *chunk) {
    if (!chunk)
        return;
    Mix_FreeChunk(chunk);
}

//This function will set a given window and renderer
// if you would like an accelerated renderer, or a v-sync enabled renderer, pass true for the third and fourth parameters

int generalInit(const char *MainWindowName, bool hardAcc, bool vSync) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
		return 0;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 0;
	}
	MainWindow = SDL_CreateWindow(MainWindowName , 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);		// make the main window at 100,100 with size 640x480, and show it
	if (MainWindow == nullptr) {
        logSDLError(std::cout, "SDL_CreateWindow");
		IMG_Quit();
		SDL_Quit();
		return 0;
	}
    
    //Initialize fonts
    if (TTF_Init() != 0){
        logSDLError(std::cout, "TTF_Init");
        SDL_Quit();
        return 1;
    }
    
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
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
		IMG_Quit();
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
	SDL_Texture *texture = IMG_LoadTexture(MainRenderer, file.c_str());
	if (texture == nullptr) {
		logSDLError(std::cout, "LoadTexture");
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

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @param renderer The renderer to load the texture in
 * @param width Width of text before wrapping
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
                        SDL_Color color, int fontSize, SDL_Renderer *renderer, int width)
{
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        logSDLError(std::cout, "TTF_OpenFont");
        return nullptr;
    }
    
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, message.c_str(), color, width);
    if (surf == nullptr){
        TTF_CloseFont(font);
        logSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        logSDLError(std::cout, "CreateTexture");
    }
    
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

/**
 * Loads a chunk
 * @param file The BMP image file to load
 * @return chunk or nullptr if something went wrong.
 */
Mix_Chunk* loadChunk(const std::string &file){
    Mix_Chunk *chunk = Mix_LoadWAV(file.c_str());
    if (chunk == nullptr) {
        logSDLError(std::cout, "LoadChunk");
    }
    return chunk;
}

void fillAndRandomizeQList() {
	std::srand(std::time(0));
	std::vector<Question*> followUpQuestions;		// this is for the case where we may have needed for some questions to follow one another, but that isn't the case
    
    //POLYNOMIAL AND RATIONAL FUNCTIONS
	MainQuestionList.push_back(new Question("Lock: 4 letter Lock. \nQuestion: \nMr. Harriz\'s presence in the media can be modelled by the function p(x) (see visual window for function definition). Over the given interval (0,20), there have been many explosions in media coverage for Mr. Harriz. However, when he notices that too much attention is being drawn to him, he suddenly disappears, and there is no available data for a year. Determine the years where Mr. Harriz disappeared.", "3, 7, 10, 14, 18", "The first and last letters of the combination are M and H", new Image(loadPath("Images/garethQ1.png"), MainRenderer, 0, 0, SCREEN_WIDTH, 89)));
	MainQuestionList.push_back(new Question("Lock: 4 letter Lock. \nQuestion: \nMr. Harriz uses a special ray gun to break through walls when he raids art galleries. The ray's trajectory can be modelled by a curve: it pierces the wall at one point and then comes back around to pierce it at several other points that lie on the same horizontal plane to weaken it. Mr. Harriz can then just kick the wall down. Its trajectory is modelled by r(x)  (see visuals window for function definition). Assuming the wall that he wants to break is the x-axis and you are looking down at the wall from a birdís eye view, how many times does the ray gun pierce the wall?", "3", "The second letter in the combination is O", new Image(loadPath("Images/garethQ2.png"), MainRenderer, 0, 0, SCREEN_WIDTH, 80)));
	MainQuestionList.push_back(new Question("Lock: 4 letter Lock. \nQuestion: \nMr. Harriz has a security camera in front of his house that malfunctions several times in one day. The function m(x) (see visuals window for function definition) models those malfunctions. Whenever m(x) > 0, the security camera is on. How many intervals are there where the camera suddenly shuts off?", "2", "The third letter in the lock combination is T", new Image(loadPath("Images/garethQ3.png"), MainRenderer, 0, 0, SCREEN_WIDTH, 85)));
	std::random_shuffle(MainQuestionList.begin(), MainQuestionList.end());
    
    //EXPONENTIAL AND LOGARITHMIC FUNCTION
    MainQuestionList.push_back(new Question("Lock: Spinny Lock. \nQuestion: \nMr Harriz’s llama population can be modelled by the function f(t) (see visuals window for function definition) where t is the time in years since he started the farm. However once the population reached 100,000 llamas Mr. Harriz ran out of space and had to start selling off llamas. This caused a large increase in profits and thus allowed for a large increase in his criminal activities. In how many years after he started the farm did this occur? \nThis is the first number in the combination \n Press enter to proceed to the next Question", "", "", new Image(loadPath("Images/benQ1.png"), MainRenderer, 0, 0, SCREEN_WIDTH, 89)));
    MainQuestionList.push_back(new Question("Lock: Spinny Lock. \nQuestion: \nMr Harriz find earthquakes very annoying as they always seem to kill off his llamas. Because of this he keeps detailed records of earthquakes. Hidden in the room is a graph of all the earthquakes he has tracked. How many times more intense is the largest earthquake than the second largest?  \nThis is the second number in the combination \n Press enter to proceed to the next Question", "", "", new Image(loadPath("Images/benQ2.png"), MainRenderer, 0, 0, SCREEN_WIDTH, 242)));
    MainQuestionList.push_back(new Question("Lock: Spinny Lock. \nQuestion: \nIn order to break into Mr. Harriz’s safe you need to corrode the lock with an acid with a pH of 4.37. However all of your vials of acid are labelled with concentrations of hydrogen ions. What concentration should you use \nThis is the final number in the combination \n Press enter to proceed to the next Question", "", "", new Image(loadPath("Images/benQ3.png"), MainRenderer, 0, 0, SCREEN_WIDTH, 151)));
    
    //vertically centre images
    for (int i = 0; i < MainQuestionList.size(); i++){
        MainQuestionList[i]->questionVisuals->centre(SCREEN_WIDTH, SCREEN_HEIGHT, false, true);
    }
}


