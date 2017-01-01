#include <CustQuestion.h>


Question::Question(char* setQuestion, char* setAnswer, char* setRewardMessage) {
	privQuestion = setQuestion;
	privAnswer = setAnswer;
	privRewardMessage = setRewardMessage;
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
	else
		askForUserAnswer();
}
void Question::printRewardMessage() {
	std::cout << privRewardMessage << std::endl;
}
