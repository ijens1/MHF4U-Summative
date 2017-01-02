/* 
MHF4U-Summative.
Written By: John Phillips, Isaac Jensen-Large, Benjamin Wilkins
Start Date: 2016-12-26

Header Structure:
All main files for the project and included in the file Core.h which is located in CustomHeaders directory
This main header file is preincluded in CustSDLInit.h
If you need to add headers, first make sure you are not creating an include loop, then add it to Core.h
If you need to add initializing functions that are general purpose, please add those to CustSDLInit.h

Path returning ressource finding function included in CustPath.h
This header is currently working for the following OSs: Windows 10, MAC.

Global Variables:
(ALL GLOBAL VARIABLES START WITH CAPITAL LETTER IN FRONT)
Both MainWindow and MainRenderer come set as nullptr from CustSDLInit.h
The MainWindow variable is the main UI window that will be used in the project
The MainRenderer variable is the main Renderer used in the project. We have chosen to use hardware acceleration and V-Sync Enabled
However, if the computer that the program is being run on cannot handle this, call the generalInit function with the respective boolean values (see definition)

Local Variables:
All other variables should be locally made inside their respective functions. This includes textures, surfaces, and other SDL structures.
Free the Memory A.S.A.P.

Initialization and De-Initialization:
See the CustSDLInit.h file.
generalInit:
	Initializes the main window and renderer
generalDeInit:
	Destroys MainRenderer variable
	Destroys MainWindow variable
	Quits SDL

DON'T FORGET TO FREE MEMORY WHEN DONE USING IT.

ERROR CODES FOR MAIN:
0: Successful run of program
1: Failed to complete generalInit function
2: failed to load texture

check out this page to get started, it seems like it has good stuff.
http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world
*/
 
#include "CustSDLInit.h"

/*
int main() {
	fillAndRandomizeQList();
	for (int i = 0; i < (int)MainQuestionList.size(); i++) {
		MainQuestionList[i]->printQuestion();
		MainQuestionList[i]->askForUserAnswer();
	}
	system("PAUSE");
	return 0;
}
*/

int main() {
	return 0;
}
