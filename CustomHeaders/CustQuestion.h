#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "CustImage.h"

class Question {
public:
	Image* questionVisuals;			//has to be public in order to use main renderer
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, Image* setImage);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList, Image* setImage);
	void printQuestion();
	void askForUserAnswer();
	void askFollowUpQuestions();
private:
	void printRewardMessage();
	bool hasFollowUpQuestions();
	std::string privQuestion, privAnswer, privRewardMessage;
	int tries;
	std::vector<Question*> followUpQuestionList;
};
