/* 
ALL CREDIT FOR THE 'Mona Harriza' PHOTO GOES TO RIZVI RAB
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
	int successfulGeneralInit = generalInit("MHF4U-Summative", true, true);
	if (!successfulGeneralInit) return 1;
	fillAndRandomizeQList();			
    
    if (playBriefing() == 1){
        cleanup(MainWindow, MainRenderer);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    //set background to black
    SDL_SetRenderDrawColor( MainRenderer, 0x00, 0x00, 0x00, 0xFF );
    
    SDL_Color green = { 0, 255, 0, 255 };
    
    std::string inputText;
    SDL_Event e;
    bool renderTextFlag;
    
    SDL_Texture *inputTexture;
    
	for (int i = 0; i < (int)MainQuestionList.size(); i++) {
        while (true){
            //Handle Evvent/Read input
            while (SDL_PollEvent(&e)){
                if (e.type == SDL_KEYDOWN){
                    if(e.key.keysym.sym == SDLK_RETURN){
                        if (MainQuestionList[i]->checkAnswer(inputText)){
                            std::cout << "YAY" << std::endl; //FIX THIS
                        }else{
                            inputText = "";
                            renderTextFlag = true;
                        }
                    }else if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ){
                        //lop off character
                        inputText.pop_back();
                        renderTextFlag = true;
                    }
                }else if( e.type == SDL_TEXTINPUT ){
                    //Not copy or pasting
                    if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
                    {
                        //Append character
                        inputText += e.text.text;
                        renderTextFlag = true;
                    }
                }
            }
            
            //Render shit
            SDL_RenderClear(MainRenderer);
            if (MainQuestionList[i]->questionVisuals != nullptr) {
                MainQuestionList[i]->questionVisuals->render(MainRenderer);
                MainQuestionList[i]->renderQuestion(20, MainQuestionList[i]->questionVisuals->geth() + 20, loadPath("Images/Arial Black.ttf"), green, 16, MainRenderer, SCREEN_WIDTH - 40);
            }
            else {
                MainQuestionList[i]->renderQuestion(20, 20, loadPath("Images/Arial Black.ttf"), green, 16, MainRenderer, SCREEN_WIDTH - 40);
            }
            if (inputText != ""){
                inputTexture = renderText(inputText, loadPath("Images/Arial Black.ttf"), green, 16, MainRenderer, SCREEN_WIDTH -100);
            }else{
                inputTexture = renderText(" ", loadPath("Images/Arial Black.ttf"), green, 16, MainRenderer, SCREEN_WIDTH -100);

            }
            //FIX THIS: correct coordinates
            renderTexture(inputTexture, MainRenderer, 50, 50);
            
            SDL_RenderPresent(MainRenderer);
        }
	}
	SDL_RenderClear(MainRenderer);
	Image* monaHarriza = new Image(loadPath("Images/monaHarriza.jpg"), MainRenderer, 0, 0, 478, SCREEN_HEIGHT);
	std::cout << std::endl  << std::endl << "You discover, inside the final box, along with Mr.Harriz\'s secret stash of candy, many letters. You open one. It reads: \"Hello Mr. Harriz, I realize that it\'s been a while since we last talked. I received this piece of art a while back from a friend of mine. It reminded me of you, so I thought you might like it. I may end up sending more in the future. \nThank you,\nJesse Wang.\"" << std::endl;
	monaHarriza->centre(SCREEN_WIDTH, SCREEN_HEIGHT, true, false);
	monaHarriza->render(MainRenderer);
	SDL_RenderPresent(MainRenderer);
	system("PAUSE");
	monaHarriza->cleanup();
    
	cleanup(MainWindow, MainRenderer);
    
    //Cleanup images from questions
    for (int i = 0; i < (int)MainQuestionList.size(); i++){
		if (MainQuestionList[i]->questionVisuals != nullptr) {
			MainQuestionList[i]->questionVisuals->cleanup();
		}
    }
    
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
        
        mugshot.renderBorder(MainRenderer, 5, 0x00, 0xFF, 0x00);
        
        //render text
        renderTexture(title, MainRenderer, titlex, 10);
        renderTexture(instructions, MainRenderer, instructionx, SCREEN_HEIGHT - mugshot.gety() + 10);
        
        SDL_RenderPresent(MainRenderer);
    }
    
    cleanup(brief, title, instructions);
    mugshot.cleanup();
    return 0;
}
