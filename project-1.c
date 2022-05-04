#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <stddef.h>
//#define  _CRT_SECURE_NO_WARNINGS
#define TEMPLATE_WIDTH 62
#define TEMPLATE_HEIGHT 18
#define _GNU_SOURCE


void menu(char* c, char* flag);
void scoreUpdate(int x, int mistake);
void drawScreen(char city[6][64], char bomb[3][3], char plane[3][16], int keep);
void saveGame();
void loadGame();
void newGame(char* flag);
void getWord(char* word);

struct data {
	int score; // Game score
	int planePos; // Current position of the plane
	int bombPos; // Current position of the bomb
	char word[11]; // Deactivation code of the bomb
	int len; // Length of word
	int remainder; // how many N length words we have to use ex. len = 3 & remainder = 2  => (2 more 3 letter word)
	int keep;  // Graphical solution for the bomb drop just underneath the plane 
};
struct data data = { 0,0,1,"",3, 5,3 }; // Setting initial values of data

void getWord(char* word) {
	int j = 0; // counter
	char all_words[300] = { 0 }; // word storage
	char* tokenPtr = NULL; // for tokens
	int randnum = 0; // random word decision

	FILE* cWord; // for file
	if ((cWord = fopen("codewords.txt", "r")) != NULL) { // if file exists
		fread(all_words, sizeof(all_words), 1, cWord);
		fclose(cWord);
	}
	else { // if file doesnt exist then prints 
		printf("Please check your codewords file's existence"); 
	}

	srand(time(NULL)); // randomizing 

	if (data.remainder == 0) // Example: if we done with 3 letter words then
	{
		data.len++; // we increase the length of the word 
		data.remainder = 5; // and remainder will again be 5 
	}
	if (data.len == 10)
	{
		data.remainder = 1000; // 10 letter words are the limit, thus we will continue having 10 letter words
	}
	randnum = rand() % 5; // random decision of word


	tokenPtr = strtok(all_words, ":");

	if (data.len == 3) { // if 3 then just take the first token
		tokenPtr = strtok(tokenPtr, ","); // takes the first token(:) of the first token(,) (different demiliters)


	}
	else {
		for (j = 0; j < data.len - 3; j++) { // if length is not 3, then enters into loop untill reach the data.len
			tokenPtr = strtok(NULL, ":");
		}

		tokenPtr = strtok(tokenPtr, ","); // if randnum is 0, first one is chosen
	}


	for (j = 0; j < randnum; j++) { // if not randnum is 0, then enters into loop

		tokenPtr = strtok(NULL, ","); // untill reaches the randum numbers token
	}
	if (tokenPtr != NULL) { // if exists
		strcpy(word, tokenPtr); // put the tokenPtr into word(data.word)
		data.remainder--;
	}


}

void menu(char* c, char* flag) { // When ESC button pressed menu will appear and ask following quesiton
	int menunum;

	printf("1. New Game\n2. Load a Saved Game\n3. Save Current Game\n4. Return to Game\n5. Exit\n");
	scanf("%1d", &menunum);

	if (menunum == 1) // if 1 selected then new game will start
	{
		newGame(flag);
		*flag = 1;
		*c = 0;
	}
	else if (menunum == 2) // if 2 selected then calls the loadGame func. and if a save exists then loads it
	{
		loadGame(flag);
		*c = 0;
	}
	else if (menunum == 3) // if 3 selected then calls the saveGame func. and game will be saved
	{
		saveGame();
		*flag = 1;
		*c = 27;
	}
	else if (menunum == 4) // if 4 selected then just returns the game
	{
		system("cls");
		*c = 0;
		*flag = 1;
	}
	else if (menunum == 5) // if 5 selected then it exits
	{
		system("cls");
		*flag = 0;
	}

}

void saveGame() { //Game status can be saved by using saveGame() func.

	int i = 0;
	char save[50] = ""; // Name of the save file 

	printf("Please give a name for your game record: \n");
	scanf("%49s", &save);

	if (save != NULL) { // if save[] exists

		FILE* crPtr = fopen(save, "w");

		fwrite(&data, sizeof(struct data), 1, crPtr);
		// writes the data into file 
		fclose(crPtr);
	}

}

void loadGame(char* flag) { // To load a saved game status

	int i = 0;
	char save[50] = ""; // Name of the file that wanted to be load

	printf("Please enter which saving you play:  \n");
	scanf("%49s", save);

	if (save != NULL) { // Checks user gived a name or not

		FILE* crPtr;

		if ((crPtr = fopen(save, "r")) != NULL) // Checks if there is a file with given name
		{
			fread(&data, sizeof(struct data), 1, crPtr);
			*flag = 1;
			fclose(crPtr);
		}
		else { // if it doenst exists then prints 
			printf("No such file\n");
			*flag = 0;
		}

	}

}

void newGame(char* flag) {

	// Just setting everything to the default

	data.score = 0;
	data.planePos = 0;
	data.bombPos = 1;
	data.len = 3;
	data.remainder = 5;
	data.keep = 3;
	getWord(&data.word);
	*flag = 1;
}

void scoreUpdate(int x, int mistake) {
	//The score will be updated according to the following formula:
	//(New Score) = (Old Score) + (Number of letters typed correctly) - (Number of letters mistyped)
	data.score = (data.score) + (x - mistake);

}

void getTemplate(char plane[3][16], char city[6][64], char bomb[3][4]) { // Gets the bomb, city and plane templates from files. (Just works at the beginnig)

	int line = 0; // for multiple lines line variable is used


	//----------------------------------------------------------------PLANE
	FILE* cPlane = fopen("plane.txt", "r");

	while (fscanf(cPlane, "%[^\n] ", &plane[line]) != EOF) {
		line++;
	}
	fclose(cPlane);

	//----------------------------------------------------------------CITY
	FILE* cCity = fopen("city.txt", "r");
	line = 0;
	while (fscanf(cCity, "%[^\n] ", &city[line]) != EOF) {
		//printf("%s\n", plane[line]);
		line++;
	}
	fclose(cCity);

	//----------------------------------------------------------------BOMB
	FILE* cBomb = fopen("bomb.txt", "r");
	line = 0;
	while (fscanf(cBomb, "%[^\n] ", &bomb[line]) != EOF) {
		//printf("%s\n", plane[line]);
		line++;
	}
	fclose(cBomb);
	// following part is for compiler error
	bomb[0][3] = 0;
	bomb[1][3] = 0;
	bomb[2][3] = 0;

	line = 0;

}

void cityDraw(char city[6][64]) { // prints city
	int i = 0;

	for (i = 0; i < 6; i++) {
		printf("%s\n", city[i]);
	}

}


void planeMove(char plane[3][16]) { // prints planes
	int i, z = 0;
	if (data.planePos == TEMPLATE_WIDTH - 15) data.planePos = 0; // if plane reaches the end of the game template then it retruns the start

	for (i = 0; i < 3; i++) {
		if (i == 1) {
			for (z = 0; z < data.planePos; z++) { printf(" "); } // gives the move effect but plane doesnt move, moving part is spaces
			printf("%s", plane[i]);
			for (z = 0; z < TEMPLATE_WIDTH - data.planePos - 15; z++) { printf(" "); }  // after plane spaces

			printf("\n");
		}
		else if (i == 2) {
			for (z = 0; z < data.planePos; z++) { printf(" "); } // for plane's 3rd line we have expection for spaces
			printf("        %s", plane[i]);
			for (z = 0; z < TEMPLATE_WIDTH - data.planePos - 15; z++) { printf(" "); }
			printf("\n");
		}
		else {
			for (z = 0; z < data.planePos; z++) { printf(" "); }
			printf("%s", plane[i]);
			for (z = 0; z < TEMPLATE_WIDTH - data.planePos - 15; z++) { printf(" "); }
			printf("Score: %d", data.score);
			printf("\n");
		}
	}

}

void bombMove(char bomb[3][4], const int keep) {
	int i = 0;
	int z = 0;

	
;	for (i = 0; i < data.bombPos; i++) { printf("\n"); } // this is the part that creates above spaces (y axi)

	for (i = 0; i < 3; i++) { //part for bomb printing
		for (z = 0; z < keep; z++) { printf(" "); }  // spaces for x axis
		if (i == 1) {
			printf("%2s%s\n", bomb[i], data.word);
		}
		else if (i == 2) printf("%2s", bomb[i]);
		else printf("%2s\n", bomb[i]);


	}


	for (i = 0; i < TEMPLATE_HEIGHT - data.bombPos; i++) { printf("\n"); }// this is the part that creates belove spaces

}

void drawScreen(char city[6][64], char bomb[3][4], char plane[3][16], int keep) {
	// combines all the printing functions	
	planeMove(plane);
	bombMove(bomb, keep);
	cityDraw(city);
	
}


int main() {

	int alb = 0; // just for incrementing and deciding process of speed that you will see at lines 327 and 331

	char flag = 1; // controller of run or break
	char* flagPtr = &flag; // ptr flag
	
	char stat_game = 0; // after game over, decision of user will be saved here(y or n)

	int y = 0; // for score update, contains correct letters
	char c = 0; // input of user getch()
	int i, k, z = 0; // loop counters 
	
	int mistake = 0; // for score update, contains mistake letters
	char city[6][64];// city
	char plane[3][16];//plane 
	char bomb[3][4];// bomb
	

	getWord(data.word); // gets the first word
	getTemplate(&plane, &city, &bomb); // gets the template

	while (flag != 0) { // when flag is not zero game continues
		
		
		
		while (_kbhit()) { // get the input characters from user
			c = _getch();

		}
		drawScreen(city, bomb, plane, data.keep);
		

		if (alb % 5 == 1) { // every 1 of 5 loop plane will move 1 space
			data.planePos++;
		}

		if (alb % (int)(100/ data.len)== 0) { // this is speed solution for bomb which will increase when words get long
			data.bombPos++;

		}

		

		alb++;// increasing the counter
		
		if (data.bombPos == TEMPLATE_HEIGHT) // bomb reaches the city
		{
			stat_game = 0;
			system("cls"); // makes ready for game over print

			printf("\n%30s\n\n", "GAME OVER"); // GAME OVER

			printf("Do you want to start a new game?(y or n)\n"); // selection of continue or not
			scanf("%1c", &stat_game); // gets the choice form user

			if (stat_game == 'y') // if y, then newgame will start  
			{
				newGame(flagPtr);// same within menu functipn
				c = 0; // c is set as 0
				stat_game = 0; // choice reset
				continue; // continue 
			}
			else if (stat_game=='n') { // if n, then just breaks
				stat_game = 0;
				break;
			}

		}

		
		
		system("cls"); // Clears the screen for redrawing the screen
		if (c == data.word[0]) // if we input the right letter
		{
			memmove(data.word, &data.word[1], strlen(data.word));
			y++; // increase for correct letter for update score
			c = 0; // reset of c
			if (data.word[0] == 0) { // when we compeletely input the letters

				getWord(data.word); // new word will be printed 
				data.keep = data.planePos + 3; // just for visual effect, bomb will drop belove the plane 
				scoreUpdate(y, mistake); // score update
				data.bombPos = 0; // bomb will start form beginning

				mistake = 0; // reset mistake
				y = 0; // reset y
				c = 0; // rest c
				continue;

			}
		}
		else if (c == 27) { // ESC key pressed then menu will be printed	

			menu(&c, flagPtr); // menu func.
		}
		else if (isalpha(c)) { // mistake count if c doesnt match with word's first letter

			mistake++; // mistale will increase one 
			c = 0; // c reset
		}
		
	}
	return 0;
}


