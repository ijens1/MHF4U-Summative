#pragma once
#include "CustQuestion.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

std::vector<Question*> MainQuestionList;
void fillAndRandomizeQList() {
	std::srand(std::time(0));
	std::vector<Question*> followUpQuestions;
	followUpQuestions.push_back(new Question("What's 3x15?", "45", "Great math!"));
	MainQuestionList.push_back(new Question("What's 2 + 2?", "4", "Good addition", followUpQuestions));
	std::cout << followUpQuestions.size() << std::endl;
	std::random_shuffle(MainQuestionList.begin(), MainQuestionList.end());
}
