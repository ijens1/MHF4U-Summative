#include "CustQuestion.h"

Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	prompt = "Enter your solution \n(or simply press enter if instructed to do so by the question):\t";
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	followUpQuestionList = qList;
	prompt = "Enter your solution \n(or simply press enter if instructed to do so by the question):\t";
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, Image* setImage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	questionVisuals = setImage;
	prompt = "Enter your solution \n(or simply press enter if instructed to do so by the question):\t";
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList, Image* setImage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	followUpQuestionList = qList;
	questionVisuals = setImage;
	prompt = "Enter your solution \n(or simply press enter if instructed to do so by the question):\t";
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, char* userPrompt, Image* setImage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	prompt = userPrompt;
	questionVisuals = setImage;
}

void Question::printQuestion() {
	std::cout << privQuestion << std::endl;
}

void Question::renderQuestion(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width){
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        std::cout << "TTF_OpenFont" << " Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, privQuestion.c_str(), color, width);
    if (surf == nullptr){
        TTF_CloseFont(font);
        std::cout << "TTF_RenderText" << " Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        std::cout << "CreatTexture" << " Error: " << SDL_GetError() << std::endl;
    }
    
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    
    //Query the texture to get its width and height to use
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    
    SDL_DestroyTexture(texture);
}

void Question::renderRewardMessage(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width){
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        std::cout << "TTF_OpenFont" << " Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, privRewardMessage.c_str(), color, width);
    if (surf == nullptr){
        TTF_CloseFont(font);
        std::cout << "TTF_RenderText" << " Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        std::cout << "CreatTexture" << " Error: " << SDL_GetError() << std::endl;
    }
    
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    
    //Query the texture to get its width and height to use
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    
    SDL_DestroyTexture(texture);
}

std::string inputSDL(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width){
    std::string inputText;
    SDL_Event e;
    
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        std::cout << "TTF_OpenFont" << " Error: " << SDL_GetError() << std::endl;
        return "";
    }
    
    while (true){
        //The rerender text flag
        bool renderText = false;

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_RETURN){
                    return inputText;
                }else if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ){
                    //lop off character
                    inputText.pop_back();
                    renderText = true;
                }
            }else if( e.type == SDL_TEXTINPUT ){
                //Not copy or pasting
                if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
                {
                    //Append character
                    inputText += e.text.text;
                    renderText = true;
                }
            }
        }
        if (renderText){
            //We need to first render to a surface as that's what TTF_RenderText
            //returns, then load that surface into a texture
            SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font, inputText.c_str(), color, width);
            if (surf == nullptr){
                TTF_CloseFont(font);
                std::cout << "TTF_RenderText" << " Error: " << SDL_GetError() << std::endl;
                return "";
            }
            
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
            if (texture == nullptr){
                std::cout << "CreatTexture" << " Error: " << SDL_GetError() << std::endl;
            }
            
            //Clean up the surface and font
            SDL_FreeSurface(surf);
            TTF_CloseFont(font);
            
            //Setup the destination rectangle to be at the position we want
            SDL_Rect dst;
            dst.x = x;
            dst.y = y;
            
            //Query the texture to get its width and height to use
            SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, texture, NULL, &dst);
            
            SDL_DestroyTexture(texture);
        }
    }
}

void Question::askForUserAnswer(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width) {
    std::string tempUserAnswer = inputSDL(x, y, fontFile, color, fontSize, renderer, width);
	if (tempUserAnswer == privAnswer)
		printRewardMessage();
	else {
		askForUserAnswer(x, y, fontFile, color, fontSize, renderer, width);
	}
}
void Question::printRewardMessage() {
	std::cout << privRewardMessage << std::endl << std::endl << std::endl;
}
bool Question::hasFollowUpQuestions() {
	return (followUpQuestionList.size() > 0);
}
void Question::askFollowUpQuestions() {
	if (hasFollowUpQuestions()) {
        for (int i = 0; i < (int)followUpQuestionList.size(); i++){
			followUpQuestionList[i]->printQuestion();
			//followUpQuestionList[i]->askForUserAnswer();
		}
	}
}
