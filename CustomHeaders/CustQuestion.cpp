#include "CustQuestion.h"

Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	tries = 0;
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	followUpQuestionList = qList;
	tries = 0;
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, Image* setImage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	questionVisuals = setImage;
	tries = 0;
}
Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage, std::vector<Question*>& qList, Image* setImage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
	followUpQuestionList = qList;
	questionVisuals = setImage;
	tries = 0;
}
void Question::printQuestion() {
	std::cout << privQuestion << std::endl;
}
void Question::askForUserAnswer() {
	std::string tempUserAnswer;
	std::cout << "Enter your solution:\t" << std::endl;
	std::cin >> tempUserAnswer;
	if (tempUserAnswer == privAnswer)
		printRewardMessage();
	else {
		tries++;
		askForUserAnswer();
	}
}
void Question::printRewardMessage() {
	std::cout << privRewardMessage << std::endl;
}
bool Question::hasFollowUpQuestions() {
	return (followUpQuestionList.size() > 0);
}
void Question::askFollowUpQuestions() {
	if (hasFollowUpQuestions()) {
		for (int i = 0; i < (int)followUpQuestionList.size(); i++) {
			followUpQuestionList[i]->printQuestion();
			followUpQuestionList[i]->askForUserAnswer();
		}
	}
}

