#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

class Question {
public:
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage);
	Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList);
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