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
	std::random_shuffle(MainQuestionList.begin(), MainQuestionList.end());
}
