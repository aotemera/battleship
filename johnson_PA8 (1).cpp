/*Name: Alexia Johnson
Class: CPSC 121, Spring 2020
Date: April 17, 2020
Programming Assignment: PA8                                                        
Description: This program runs the game Battleship, where P1 is the user, and P2 is the computer. The user can manually or randomly place ships. The game records to an output file stats on who shot where, if it hit, who won, and statistics about guesses and hits.
Notes: N/A
*/

#include "battleship.h"

int main() {
	char p1SolutionGrid[NUM_ROWS][NUM_COLS]; // 10 x 10 chars
	char p2SolutionGrid[NUM_ROWS][NUM_COLS]; 
	char p2OutputGrid[NUM_ROWS][NUM_COLS]; // can't output computer's ships to the console, so need an array for display
	string outFileName = "battleship.log";
	ofstream battleshipLog;
	int firstPlayer;
	char placeShips;
	int i = 0;
	string trashLine;
	bool someoneWon = false;
	istringstream userInput;
	string userInputString;
	int userGuessRow;
	int userGuessCol;
	bool goodGuess = false;
	bool goodShot;
	char whichShip;
	int p1Guesses = 0;
	int p1Hits = 0;
	int computerGuessRow;
	int computerGuessCol;
	int p2Guesses = 0;
	int p2Hits = 0;
	bool sunkShip;
	string whoWon;

	battleshipLog.open(outFileName);
	if (!battleshipLog.is_open()) {
		cout << "Couldn't open the file." << endl;
		exit(-1);
	}

	welcomeScreen();

	initializeBoard(p1SolutionGrid, NUM_ROWS, NUM_COLS);
	printArray(p1SolutionGrid, NUM_ROWS, NUM_COLS, 1);
	cout << endl;
	
	initializeBoard(p2SolutionGrid, NUM_ROWS, NUM_COLS);
	initializeBoard(p2OutputGrid, NUM_ROWS, NUM_COLS);
	printArray(p2OutputGrid, NUM_ROWS, NUM_COLS, 2);

	srand(time(0));
	firstPlayer = selectWhoStartsFirst();

	cout << "Place ships manually or randomly? Enter m or r: ";
	cin  >> placeShips;
	cin.clear();
	cin.ignore(265, '\n');

	if (placeShips == 'm') {
		for (i = 0; i < 5; ++i) {
			manuallyPlaceShips(p1SolutionGrid, shipNames[i], shipSymbols[i], shipSizes[i], NUM_ROWS, NUM_COLS);
		}		
	}
	else {
		randomlyPlaceShips(p1SolutionGrid, NUM_ROWS, NUM_COLS);
	}
	
	printArray(p1SolutionGrid, NUM_ROWS, NUM_COLS, 1);

	randomlyPlaceShips(p2SolutionGrid, NUM_ROWS, NUM_COLS); //for computer ships, p2SolutionGrid
	printArray(p2OutputGrid, NUM_ROWS, NUM_COLS, 2);

	while (firstPlayer == 0) {
		cout << "P1 goes first." << endl;
		cout << "Please press enter..." << endl;
		getline(cin, trashLine);
		while (someoneWon == false) {
			userGuessRow = -1;
			userGuessCol = -1;
			while (goodGuess == false) {		
				cout << "Please enter your guess. Enter a row number 0-9, a space, then a column number 0-9: ";
				getline(cin, userInputString); //able to get multiple numbers at a time from the user
				userInput.clear(); //clears the input stream buffer - from Zybooks 4.3
				userInput.str(userInputString);
				userInput >> userGuessRow;
				userInput >> userGuessCol;
				goodGuess = checkUserInput(p2SolutionGrid, userGuessRow, userGuessCol, NUM_ROWS, NUM_COLS);
				if (goodGuess == false) {
					continue;
				}
			}
			p1Guesses += 1;
			goodShot = checkShot(p2SolutionGrid, userGuessRow, userGuessCol, NUM_ROWS, NUM_COLS);

			if (goodShot == true) {
				cout << "Good shot!" << endl;
				p1Hits += 1;
				whichShip = p2SolutionGrid[userGuessRow][userGuessCol];
				updateBoard(p2OutputGrid, userGuessRow, userGuessCol, '*', NUM_ROWS, NUM_COLS);
				updateBoard(p2SolutionGrid, userGuessRow, userGuessCol, '*', NUM_ROWS, NUM_COLS);
				sunkShip = checkIfSunkShip(p2SolutionGrid, whichShip, NUM_ROWS, NUM_COLS);
				outputCurrentMove(battleshipLog, userGuessRow, userGuessCol, goodShot, sunkShip, 1);			
				if (sunkShip == true) {
					cout << "Congrats! You sunk one of P2's ships." << endl;
					someoneWon = isWinner(p2SolutionGrid, NUM_ROWS, NUM_COLS);
					if (someoneWon == true) {
						cout << "You won!" << endl;
						whoWon = "Player 1";
						firstPlayer = 2;
						break;
						break;
					}
				}
			}
			else {
				cout << "You missed!" << endl;
				updateBoard(p2OutputGrid, userGuessRow, userGuessCol, 'm', NUM_ROWS, NUM_COLS);
				updateBoard(p2SolutionGrid, userGuessRow, userGuessCol, '*', NUM_ROWS, NUM_COLS);
			}
			goodGuess = false;
			printArray(p1SolutionGrid, NUM_ROWS, NUM_COLS, 1);
			printArray(p2OutputGrid, NUM_ROWS, NUM_COLS, 2);
	
			while (goodGuess == false) {		
				cout << "The computer is guessing: ";
				computerGuessRow = randomComputerGuessRow();
				computerGuessCol = randomComputerGuessCol();
				cout << computerGuessRow << " " << computerGuessCol << endl;
				goodGuess = checkUserInput(p1SolutionGrid, computerGuessRow, computerGuessCol, NUM_ROWS, NUM_COLS);
				if (goodGuess == false) {
					continue;
				}	
			}
			p2Guesses += 1;
			goodShot = checkShot(p1SolutionGrid, computerGuessRow, computerGuessCol, NUM_ROWS, NUM_COLS);

			if (goodShot == true) {
				cout << "The computer landed a hit!" << endl;
				p2Hits += 1;
				whichShip = p1SolutionGrid[computerGuessRow][computerGuessCol];
				updateBoard(p1SolutionGrid, computerGuessRow, computerGuessCol, '*', NUM_ROWS, NUM_COLS);
				sunkShip = checkIfSunkShip(p1SolutionGrid, whichShip, NUM_ROWS, NUM_COLS);
				outputCurrentMove(battleshipLog, computerGuessRow, computerGuessCol, goodShot, sunkShip, 2);			
				if (sunkShip == true) {
					cout << "Unfortunately, P2 sunk one of your ships!" << endl;
					someoneWon = isWinner(p1SolutionGrid, NUM_ROWS, NUM_COLS);
					if (someoneWon == true) {
						cout << "The computer won!" << endl;
						whoWon = "Player 2";
						firstPlayer = 2;
						break;
						break;
					}
				}
			}
			else {
				cout << "The computer missed!" << endl;
				updateBoard(p1SolutionGrid, computerGuessRow, computerGuessCol, 'm', NUM_ROWS, NUM_COLS);
			}
			goodGuess = false;
			printArray(p1SolutionGrid, NUM_ROWS, NUM_COLS, 1);
			printArray(p2OutputGrid, NUM_ROWS, NUM_COLS, 2);
			cout << "Please press enter..." << endl;
			getline(cin, trashLine);
			system("clear");
		}
	}

	while (firstPlayer == 1) {
		cout << "P2 goes first." << endl;
		cout << "Please press enter..." << endl;
		getline(cin, trashLine);
		while (someoneWon == false) {
			userGuessRow = -1;
			userGuessCol = -1;
			while (goodGuess == false) {		
				cout << "The computer is guessing: ";
				computerGuessRow = randomComputerGuessRow();
				computerGuessCol = randomComputerGuessCol();
				cout << computerGuessRow << " " << computerGuessCol << endl;
				goodGuess = checkUserInput(p1SolutionGrid, computerGuessRow, computerGuessCol, NUM_ROWS, NUM_COLS);
				if (goodGuess == false) {
					continue;
				}	
			}
			p2Guesses += 1;
			goodShot = checkShot(p1SolutionGrid, computerGuessRow, computerGuessCol, NUM_ROWS, NUM_COLS);

			if (goodShot == true) {
				cout << "The computer landed a hit!" << endl;
				p2Hits += 1;
				whichShip = p1SolutionGrid[computerGuessRow][computerGuessCol];
				updateBoard(p1SolutionGrid, computerGuessRow, computerGuessCol, '*', NUM_ROWS, NUM_COLS);
				sunkShip = checkIfSunkShip(p1SolutionGrid, whichShip, NUM_ROWS, NUM_COLS);
				outputCurrentMove(battleshipLog, computerGuessRow, computerGuessCol, goodShot, sunkShip, 2);			
				if (sunkShip == true) {
					cout << "Unfortunately, P2 sunk one of your ships!" << endl;
					someoneWon = isWinner(p1SolutionGrid, NUM_ROWS, NUM_COLS);
					if (someoneWon == true) {
						cout << "The computer won!" << endl;
						whoWon = "Player 2";
						firstPlayer = 2;
						break;
						break;
					}
				}
			}
			else {
				cout << "The computer missed!" << endl;
				updateBoard(p1SolutionGrid, computerGuessRow, computerGuessCol, 'm', NUM_ROWS, NUM_COLS);
			}
			goodGuess = false;
			printArray(p1SolutionGrid, NUM_ROWS, NUM_COLS, 1);
			printArray(p2OutputGrid, NUM_ROWS, NUM_COLS, 2);

			while (goodGuess == false) {		
				cout << "Please enter your guess. Enter a row number 0-9, a space, then a column number 0-9: ";
				getline(cin, userInputString); //able to get multiple numbers at a time from the user
				userInput.clear(); //clears the input stream buffer - from Zybooks 4.3
				userInput.str(userInputString);
				userInput >> userGuessRow;
				userInput >> userGuessCol;
				goodGuess = checkUserInput(p2SolutionGrid, userGuessRow, userGuessCol, NUM_ROWS, NUM_COLS);
				if (goodGuess == false) {
					continue;
				}
			}
			p1Guesses += 1;
			goodShot = checkShot(p2SolutionGrid, userGuessRow, userGuessCol, NUM_ROWS, NUM_COLS);

			if (goodShot == true) {
				cout << "Good shot!" << endl;
				p1Hits += 1;
				whichShip = p2SolutionGrid[userGuessRow][userGuessCol];
				updateBoard(p2OutputGrid, userGuessRow, userGuessCol, '*', NUM_ROWS, NUM_COLS);
				updateBoard(p2SolutionGrid, userGuessRow, userGuessCol, '*', NUM_ROWS, NUM_COLS);
				sunkShip = checkIfSunkShip(p2SolutionGrid, whichShip, NUM_ROWS, NUM_COLS);
				outputCurrentMove(battleshipLog, userGuessRow, userGuessCol, goodShot, sunkShip, 1);			
				if (sunkShip == true) {
					cout << "Congrats! You sunk one of P2's ships." << endl;
					someoneWon = isWinner(p2SolutionGrid, NUM_ROWS, NUM_COLS);
					if (someoneWon == true) {
						cout << "You won!" << endl;
						whoWon = "Player 1";
						firstPlayer = 2;
						break;
						break;
					}
				}

			}
			else {
				cout << "You missed!" << endl;
				updateBoard(p2OutputGrid, userGuessRow, userGuessCol, 'm', NUM_ROWS, NUM_COLS);
				updateBoard(p2SolutionGrid, userGuessRow, userGuessCol, '*', NUM_ROWS, NUM_COLS);
			}
			goodGuess = false;
			printArray(p1SolutionGrid, NUM_ROWS, NUM_COLS, 1);
			printArray(p2OutputGrid, NUM_ROWS, NUM_COLS, 2);
			cout << "Please press enter..." << endl;
			getline(cin, trashLine);
			system("clear");	
		}
	}
	outputStats(battleshipLog, whoWon, p1Guesses, p1Hits, p2Guesses, p2Hits);
	battleshipLog.close();
	return 0;
}
