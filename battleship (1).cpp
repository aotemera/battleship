#include "battleship.h"

void welcomeScreen() {
	string trashInput;	
	
	cout << "*****WELCOME******" << endl;
	cout << "*******TO*********" << endl;
	cout << "****BATTLESHIP****" << endl << endl;

	cout << "Here are the rules:" << endl;

	cout << "Battleship is a 2-player strategy game. Each player has a 10 x 10 board, which the other player can't see." << endl;
	cout << "Each player gets to place 5 ships: " << endl;
	cout << "	The carrier is 5 spaces long." << endl;
	cout << "	The battleship is 4 spaces long." << endl;
	cout << "	The cruiser is 3 spaces long." << endl;
	cout << "	The submarine is 3 spaces long." << endl;
	cout << "	The destroyer is 2 spaces long." << endl;
	cout << "You each place your ships on your board. Then, you take turns trying to shoot the other person's ships down." << endl;
	cout << "To do this, you call out coordinates. If it's a miss, an 'm' will appear on the board." << endl;
	cout << "If it's a hit, a '*' will appear." << endl;
	cout << "Hit all of the spaces of a ship to sink it. Sink all 5 of the other person's ships, and you win!" << endl;
	cout << "You are Player 1, and the computer is Player 2." << endl;
	
	cout << "Press enter to begin the game...";
	getline (cin, trashInput);
	cout << endl;
}

void printArray(const char array[NUM_ROWS][NUM_COLS], int rows, int cols, int boardNum) {
	int i, j, k;
	k = 0;
	cout << "Player " << boardNum << "'s Board:" << endl;

	cout << "  0 1 2 3 4 5 6 7 8 9" << endl;
	for (i = 0; i < rows; i++) {
		cout << k << " ";
		++k;
		for (j = 0; j < cols; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}

void initializeBoard(char array[NUM_ROWS][NUM_COLS], int rows, int cols) {
	int i, j;
	
	// outer loop iterates over each row
	for (i = 0; i < rows; i++) {
		// inner loop iterates over int in arr[i] (row)
		for (j = 0; j < cols; j++) {
			array[i][j] = '-';
		}
	}
}

int selectWhoStartsFirst() {
	int firstPlayer;

	firstPlayer = (rand() % 2);
	return firstPlayer;
}

void manuallyPlaceShips(char board[NUM_ROWS][NUM_COLS], const string shipName, const char shipSymbol, const int shipSize, int row, int cols) {
	bool gettingInput = true;
	bool badInput = false;
	istringstream userInput;
	string userInputString;
	string userCoord, userRowStr, userColStr;
	int userCoordInt, userRow, userCol;
	int i = 0;
	int j = 0;

	while (gettingInput == true) {
		gettingInput = true;
		badInput = false;
		cout << "Please enter the " << shipSize << " coordinates for the " << shipName << "." << endl;
		getline(cin, userInputString);
		userInput.clear();
		userInput.str(userInputString);
		for (i = 0; i < shipSize; ++i) {
			userInput >> userCoord;
			userCoordInt = stoi(userCoord);
			if ((userCoordInt < 0) || (userCoordInt > 99)) {
				cout << "That isn't valid input. Please redo your input." << endl;
				badInput = true;
				break;
			}
			userRowStr = userCoord[0];
			userRow = stoi(userRowStr);
			userColStr = (userCoord[1]);
			userCol = stoi(userColStr);
			if (board[userRow][userCol] != '-') {
				cout << "You've already placed a ship at " << userRow << userCol << ". Please redo your input." << endl;
				badInput = true;
				break;	
			}
		}
		if (badInput == false) {
			userInput.clear();
			userInput.str(userInputString);
			for (j = 0; j < shipSize; ++j) {
				userInput >> userCoord;
				userRowStr = userCoord[0];
				userRow = stoi(userRowStr);
				userColStr = (userCoord[1]);
				userCol = stoi(userColStr);
				board[userRow][userCol] = shipSymbol;
			}
			gettingInput = false;
		}
	printArray(board, NUM_ROWS, NUM_COLS, 1);
	}
}

void randomlyPlaceShips(char board[NUM_ROWS][NUM_COLS], int, int) {
	int i;

	for (i = 0; i < 5; ++i) {
		randomlyPlaceSingleShip(board, shipSymbols[i], shipSizes[i], NUM_ROWS, NUM_COLS);
	}
}

void randomlyPlaceSingleShip(char board[NUM_ROWS][NUM_COLS], const char shipSymbol, const int shipSize, int, int) {
	int row, col, orientation;
	int i, j, end;
	int orientationsTried[4] = {};
	bool placingShip = true;
	bool checkingFit = true;
	bool ranIntoShip = false;
	int checkRow, checkCol;

	// 0 = up = - row
	// 1 = right = + col
	// 2 = down = + row
	// 3 = left = - col

	while (placingShip == true) {
		ranIntoShip = false;
		row = (rand() % 10);
		col = (rand() % 10);
		checkRow = row;
		checkCol = col;

		// if collision, choose new starting point
		if (board[row][col] != '-') {
			continue;
		}

		// check if ship fits
		orientationsTried[0] = 0;
		orientationsTried[1] = 0;
		orientationsTried[2] = 0;
		orientationsTried[3] = 0;
		while (checkingFit == true) {
			ranIntoShip = false;
			checkRow = row;
			checkCol = col;
			orientation = rand() % 4;
			if (orientationsTried[orientation] == 1) {
				//check if all orientations have failed based on checks below
				if ((orientationsTried[0] == 1) && (orientationsTried[1] == 1) && (orientationsTried[2] == 1) && (orientationsTried[3] == 1)) {
					break;
				}
				continue;
			}
			orientationsTried[orientation] = 1;

			if (orientation == 0) {
				end = row - shipSize - 1;
			}
			else if (orientation == 1) {
				end = col + shipSize - 1;
			}
			else if (orientation == 2) {
				end = row + shipSize - 1;
			}
			else {
				end = col - shipSize - 1;
			}

			//check if ship doesn't fit within the board
			if (end < 0 || end > 9) {
				continue;
			}
	
			//check if ship in random orientation
			if (orientation == 0) {
				for (i = 1; i < shipSize; ++i) {
					checkRow -= 1;
					if (board[checkRow][checkCol] != '-') {
						ranIntoShip = true;
						break;
					}
				}
				if (ranIntoShip == true) {
					continue;
				}
			}
			else if (orientation == 1) {
				for (i = 1; i < shipSize; ++i) {
					checkCol += 1;
					if (board[checkRow][checkCol] != '-') {
						ranIntoShip = true;
						break;
					}
				}
				if (ranIntoShip == true) {
					continue;
				}
			}
			else if (orientation == 2) {
				for (i = 1; i < shipSize; ++i) {
					checkRow += 1;
					if (board[checkRow][checkCol] != '-') {
						ranIntoShip = true;
						break;
					}
				}
				if (ranIntoShip == true) {
					continue;
				}
			}
			else {
				for (i = 1; i < shipSize; ++i) {
					checkCol -= 1;
					if (board[checkRow][checkCol] != '-') {
						ranIntoShip = true;
						break;
					}
				}
				if (ranIntoShip == true) {
					continue;
				}
			}

			//ship fits and no collisions, so now place it
			if (orientation == 0) {
				for (i = 0; i < shipSize; ++i) {
					board[row][col] = shipSymbol;
					row -= 1;
				}
			}
			else if (orientation == 1) {
				for (i = 0; i < shipSize; ++i) {
					board[row][col] = shipSymbol;
					col += 1;
				}
			}
			else if (orientation == 2) {
				for (i = 0; i < shipSize; ++i) {
					board[row][col] = shipSymbol;
					row += 1;
				}
			}
			else {
				for (i = 0; i < shipSize; ++i) {
					board[row][col] = shipSymbol;
					col -= 1;
				}
			}
			placingShip = false;
			break;
		}
	}
}

int randomComputerGuessRow() {
	int computerGuessRow;

	computerGuessRow = (rand() % 10);
	return computerGuessRow;
}

int randomComputerGuessCol() {
	int computerGuessCol;

	computerGuessCol = (rand() % 10);
	return computerGuessCol;
}

bool checkUserInput(char opponentBoard[NUM_ROWS][NUM_COLS], int userGuessRow, int userGuessCol, int rows, int cols) {
	bool goodGuess = true;
	
	if ((userGuessRow < 0) || (userGuessRow > 9)) {
		goodGuess = false;
		cout << "Sorry, that row isn't valid. Try again." << endl;
	}
	if ((userGuessCol < 0) || (userGuessCol > 9)) {
		cout << "Sorry, that column isn't valid. Try again." << endl;
		goodGuess = false;
	}
	if ((opponentBoard[userGuessRow][userGuessCol] == '*') || (opponentBoard[userGuessRow][userGuessCol] == 'm')) {
		cout << "You've already guessed that. Try again." << endl;
		goodGuess = false;
	}
	
	return goodGuess;
}
	
bool checkShot(char opponentBoard[NUM_ROWS][NUM_COLS], int guessRow, int guessCol, int rows, int cols) {
	bool hitShip = false;

	if (opponentBoard[guessRow][guessCol] != '-') {
		hitShip = true;
	}

	return hitShip;
}

bool checkIfSunkShip(char opponentBoard[NUM_ROWS][NUM_COLS], const char shipSymbol, int rows, int cols) {
	int i, j;
	bool sunkShip = true;
	
	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if (opponentBoard[i][j] == shipSymbol) {
				sunkShip = false;
			}
		}
	}
	
	return sunkShip;
}

bool isWinner(char opponentBoard[NUM_ROWS][NUM_COLS], int rows, int cols) {
	bool someoneWon = true;
	int i, j;

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			if ((opponentBoard[i][j] != '-') && (opponentBoard[i][j] != '*') && (opponentBoard[i][j] != 'm')) {
				someoneWon = false;
				break;
				break;
			}
		}
	}

	return someoneWon;
}

void updateBoard(char array[NUM_ROWS][NUM_COLS], int guessRow, int guessCol, char marker, int rows, int cols) {
	array[guessRow][guessCol] = marker;
}

void outputCurrentMove(ofstream& outFile, int userGuessRow, int userGuessCol, bool goodShot, bool sunkShip, int playerNum) {
	
	outFile << "Player" << playerNum << ": Shot at " << userGuessRow << " " << userGuessCol;
	if (goodShot == true) {
		outFile << ", hit." << endl;
	}
	else {
		outFile << ", miss." << endl;
	}
	if (sunkShip == true) {
		outFile << "Player" << playerNum << " sunk a ship." << endl;
	}
}

void outputStats(ofstream& outFile, string whoWon, int p1Guesses, int p1Hits, int p2Guesses, int p2Hits) {
	int p1Misses, p2Misses;
	double p1Ratio, p2Ratio;	

	outFile << whoWon << " won." << endl << endl;
	
	outFile << "*****Player 1 Statistics*****" << endl;
	outFile << "Total hits: " << p1Hits << endl;
	p1Misses = p1Guesses - p1Hits;
	outFile << "Total misses: " << p1Misses << endl;
	outFile << "Total guesses: " << p1Guesses << endl;
	p1Ratio = (static_cast<double>(p1Hits) / p1Guesses) * 100;
	outFile << "Hit to miss ratio: " << p1Ratio << "%" << endl << endl;

	outFile << "*****Player 2 Statistics*****" << endl;
	outFile << "Total hits: " << p2Hits << endl;
	p2Misses = p2Guesses - p2Hits;
	outFile << "Total misses: " << p2Misses << endl;
	outFile << "Total guesses: " << p2Guesses << endl;
	p2Ratio = (static_cast<double>(p2Hits) / p2Guesses) * 100;
	outFile << "Hit to miss ratio: " << p2Ratio << "%" << endl;
}
