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

int playBriefing();

int main() {
	int successfulGeneralInit = generalInit("Tutorial 1", true, true);
	if (!successfulGeneralInit) return 1;
	fillAndRandomizeQList();			
    
    if (playBriefing() == 1){
        cleanup(MainWindow, MainRenderer);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    //set background to white
    SDL_SetRenderDrawColor( MainRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    
	for (int i = 0; i < (int)MainQuestionList.size(); i++) {
		SDL_RenderClear(MainRenderer);
		if (MainQuestionList[i]->questionVisuals != nullptr) {
			MainQuestionList[i]->questionVisuals->render(MainRenderer);
			SDL_RenderPresent(MainRenderer);
		}
		MainQuestionList[i]->printQuestion();
		MainQuestionList[i]->askForUserAnswer();
		MainQuestionList[i]->askFollowUpQuestions();			//this will only do anything if the question has any follow up questions
	}
    
	cleanup(MainWindow, MainRenderer);
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int playBriefing(){
    SDL_Event e;
    
    bool cont = false;
    //load music
    Mix_Chunk *brief = loadChunk(loadPath("Images/Briefing.wav"));
    if (brief == nullptr){
        return 1;
    }
    
    Image mugshot(loadPath("Images/Mug Shot.jpg"), MainRenderer, 0, 0);
    
    if (!mugshot.successfullLoad()){
        cleanup(brief);
        return 1;
    }
    
    
    //load fonts
    SDL_Color color = { 0, 255, 0, 255 };
    SDL_Texture *title = renderText("Briefing", loadPath("Images/Arial Black.ttf"), color, 30, MainRenderer, 1000);
    SDL_Texture *instructions = renderText("Press Enter to Continue", loadPath("Images/Arial Black.ttf"), color, 30, MainRenderer, 1000);
    if (title == nullptr || instructions == nullptr){
        cleanup(brief);
        mugshot.cleanup();
        return 1;
    }
    
    //Calculate text positions
    int titleW, instructionW;
    SDL_QueryTexture(title, NULL, NULL, &titleW, NULL);
    SDL_QueryTexture(instructions, NULL, NULL, &instructionW, NULL);
    int titlex = SCREEN_WIDTH / 2 - titleW / 2;
    int instructionx = SCREEN_WIDTH / 2 - instructionW / 2;
    
    //calculate image position
    mugshot.centre(SCREEN_WIDTH, SCREEN_HEIGHT, true, true);
    
    //play brief
    if( Mix_PlayChannel( -1, brief, 0 ) == -1 ){
        return 2;
    }
    
    while (!cont){
        //check if press enter
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN){
                cont = true;
            }
        }
        
        //clear screen to black
        SDL_SetRenderDrawColor( MainRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear(MainRenderer);
        
        //render outline to image
        SDL_Rect fillRect = { mugshot.getx()-5, mugshot.gety()-5, mugshot.getw()+10, mugshot.geth()+10 };
        SDL_SetRenderDrawColor( MainRenderer, 0x00, 0xFF, 0x00, 0xFF );
        SDL_RenderFillRect( MainRenderer, &fillRect );
        
        //render image
        mugshot.render(MainRenderer);
        
        //render text
        renderTexture(title, MainRenderer, titlex, 10);
        renderTexture(instructions, MainRenderer, instructionx, SCREEN_HEIGHT - mugshot.gety() + 10);
        
        SDL_RenderPresent(MainRenderer);
    }
    
    cleanup(brief, title, instructions);
    mugshot.cleanup();
    return 0;
}
