#pragma once
#include <iostream>
#include <string>
#include <cstring>

class Question {
public:
	Question(char* setQuestion, char* setAnswer, char* rewardMessage);
	void printQuestion();
	void askForUserAnswer();
private:
	void printRewardMessage();
	std::string privQuestion, privAnswer, privRewardMessage;
	int tries;
};