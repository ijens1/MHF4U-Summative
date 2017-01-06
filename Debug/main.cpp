/* 
MHF4U-Summative.
Written By: John Phillips, Isaac Jensen-Large, Benjamin Wilkins
Start Date: 2016-12-26

Header Structure:
All main files for the project and included in the file Core.h which is located in CustomHeaders directory
This main header file is preincluded in CustSDLInit.h
If you need to add headers, first make sure you are not creating an include loop, then add it to Core.h
If you need to add initializing functions that are general purpose, please add those to CustSDLInit.h

Path returning ressource finding function included in CustPath.h
This header is currently working for the following OSs: Windows 10, MAC.

Global Variables:
(ALL GLOBAL VARIABLES START WITH CAPITAL LETTER IN FRONT)
Both MainWindow and MainRenderer come set as nullptr from CustSDLInit.h
The MainWindow variable is the main UI window that will be used in the project
The MainRenderer variable is the main Renderer used in the project. We have chosen to use hardware acceleration and V-Sync Enabled
However, if the computer that the program is being run on cannot handle this, call the generalInit function with the respective boolean values (see definition)

Local Variables:
All other variables should be locally made inside their respective functions. This includes textures, surfaces, and other SDL structures.
Free the Memory A.S.A.P.

Initialization and De-Initialization:
See the CustSDLInit.h file.
generalInit:
	Initializes the main window and renderer
generalDeInit:
	Destroys MainRenderer variable
	Destroys MainWindow variable
	Quits SDL

DON'T FORGET TO FREE MEMORY WHEN DONE USING IT.

ERROR CODES FOR MAIN:
0: Successful run of program
1: Failed to complete generalInit function
2: failed to load texture or surface

check out this page to get started, it seems like it has good stuff.
http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world

*/
#include "CustSDLInit.h"

int main() {
	int successfulGeneralInit = generalInit("Tutorial 1", true, true);
	if (!successfulGeneralInit) return 1;
    
    //load images
	SDL_Texture* background = loadTexture(loadPath("Images/background.png"), MainRenderer);
	SDL_Texture* mainImage = loadTexture(loadPath("Images/image.png"), MainRenderer);
	if (background == nullptr || mainImage == nullptr) {
		cleanup(MainWindow, MainRenderer, background, mainImage);
		SDL_Quit();
		IMG_Quit();
		return 2;
	}
    
    //load fonts
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Texture *text = renderText("lorem ipsum some more words", loadPath("Images/Arial Black.ttf"), color, 30, MainRenderer, 100);
    if (text == nullptr){
        cleanup(MainWindow, MainRenderer, background, mainImage);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    //load music
    Mix_Chunk *brief = loadChunk(loadPath("Images/Briefing.wav"));
    if (brief == nullptr){
        cleanup(MainWindow, MainRenderer, background, mainImage, text);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    //Get the texture w/h so we can center it in the screen
    int tW, tH;
    SDL_QueryTexture(text, NULL, NULL, &tW, &tH);
    int tx = SCREEN_WIDTH / 2 - tW / 2;
    int ty = SCREEN_HEIGHT / 2 - tH / 2;

    
	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;

    SDL_Event e;
    bool quit = false;
    
    //play brief
    if( Mix_PlayChannel( -1, brief, 0 ) == -1 ){
        return 1;
    }
    
    while (!quit) {
        //read in all events since last frame
        //read anount handling key presses here:
        //https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                quit = true;
            }
        }
        
		SDL_RenderClear(MainRenderer);

		for (int i = 0; i < xTiles * yTiles; i++) {
			int x = i % xTiles;
			int y = i / xTiles;
			renderTexture(background, MainRenderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}

		int iW, iH;
		SDL_QueryTexture(mainImage, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		renderTexture(mainImage, MainRenderer, x, y);
        
        //Render text
        renderTexture(text, MainRenderer, tx, ty);
		
		SDL_RenderPresent(MainRenderer);
	}
	cleanup(MainWindow, MainRenderer, background, mainImage, text, brief);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
