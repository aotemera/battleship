#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

const int NUM_ROWS = 10;
const int NUM_COLS = 10;

// lets store ship info in 3 parallel arrays
const string shipNames[] = {"carrier", "battleship", "cruiser", "submarine", "destroyer"};
const char shipSymbols[] = {'c', 'b', 'r', 's', 'd'};
// task: one more parallel array for ship sizes
const int shipSizes[] = {5, 4, 3, 3, 2};

void welcomeScreen();
void printArray(const char array[NUM_ROWS][NUM_COLS], int, int, int boardNum);
void initializeBoard(char array[NUM_ROWS][NUM_COLS], int, int);
int selectWhoStartsFirst();
void manuallyPlaceShips(char board[NUM_ROWS][NUM_COLS], const string shipName, const char shipSymbol, const int shipSize, int, int);
void randomlyPlaceShips(char board[NUM_ROWS][NUM_COLS], int, int);
void randomlyPlaceSingleShip(char board[NUM_ROWS][NUM_COLS], const char shipSymbol, const int shipSize, int, int);
int randomComputerGuessRow();
int randomComputerGuessCol();
bool checkUserInput(char opponentBoard[NUM_ROWS][NUM_COLS], int userGuessRow, int userGuessCol, int rows, int cols);
bool checkShot(char opponentBoard[NUM_ROWS][NUM_COLS], int guessRow, int guessCol, int, int);
bool checkIfSunkShip(char opponentboard[NUM_ROWS][NUM_COLS], const char shipSymbol, int, int);
bool isWinner(char opponentBoard[NUM_ROWS][NUM_COLS], int rows, int cols);
void updateBoard(char array[NUM_ROWS][NUM_COLS], int guessRow, int guessCol, char marker, int, int);
void outputCurrentMove(ofstream& outFile, int userGuessRow, int userGuessCol, bool goodShot, bool sunkShip, int playerNum);
void outputStats(ofstream& outFile, string whoWon, int p1Guesses, int p1Hits, int p2Guesses, int p2Hits);

