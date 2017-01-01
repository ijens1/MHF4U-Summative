#pragma once
#include <CustQuestion.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

std::vector<Question*> MainQuestionList;
void fillAndRandomizeQList() {
	std::srand(std::time(0));
	MainQuestionList.push_back(new Question("What's 3 x 15?", "45", "Good multiplication"));
	MainQuestionList.push_back(new Question("What's 2 + 2?", "4", "Great math!"));
	std::random_shuffle(MainQuestionList.begin(), MainQuestionList.end());
}