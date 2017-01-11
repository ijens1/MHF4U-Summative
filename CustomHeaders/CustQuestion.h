#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "CustImage.h"
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

class Question {
public:
	Image* questionVisuals;			//has to be public in order to use main renderer
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, Image* setImage);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList, Image* setImage);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, char* userPrompt, Image* setImage);
	void printQuestion();
    void renderQuestion(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width);
    void renderRewardMessage(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width);
	void askForUserAnswer(int x, int y, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer, int width);
	void askFollowUpQuestions();
private:
	void printRewardMessage();
	bool hasFollowUpQuestions();
	std::string privQuestion, privAnswer, privRewardMessage, prompt;
	std::vector<Question*> followUpQuestionList;
};
