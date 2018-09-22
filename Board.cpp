/*
Ben Wasserman
2280906
wasse114@mail.chapman.edu
CPSC 350 Section 3
Assignment #2: Game of Life

Purpose of source file:

This source file contains all the relevant functions to the program.
At a high level, this source file initializes two boards, and then calls
functions for every cell in the board that count the neighbors, determine whether
the cell should live or die, and then assigns the proper character for that
position in the next generation. When all the proper positions have been assigned,
the two boards are swapped and the process is repeated.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include "Board.h"

using namespace std;

/*
These are some of the global variables that need to be tracked
throughout the program. Generation is the generation for the population, starting
with 0 upon initialization. The string modeCheck checks to see what mode
the user wants to play in. The string outFile takes in the name or the path
to the file the user wants to print to. The string pauseType takes in the way
that the user wants to see the generations ouput. Lastly, the integer exitNumber
is necessary to ignore the delimiter in the userExit function.
*/
int generation = 0;
string modeCheck;
string outFile = "1";
string pauseType;
int exitNumber = 0;

//Default constructor
Board::Board()
{

}

//Constructor
Board::Board(int rows, int columns){
  char** board = new char*[rows];
  for (int s = 0; s < rows; ++s){
    board[s] = new char[columns];
  }
}

//Destructor
Board::~Board()
{

  delete[] board;

  cout << "Object deleted" << endl;
  for (int n = 0; n < rows; ++n){
    delete[] board[columns];
   delete[] board;
  }
  for (int s = 0; s < rows; ++s){
    delete[] copiedBoard[columns];
  delete[] copiedBoard;
  }

}

/*
This function is the start of the program, and gets it all running. The name
of the function is run and it doesn't return any values, nor does it take
any values as parameters. It takes user input on the type of board they want
to initialize and calls the proper initialization function. If they choose
random configuration it also takes in the frequency of filled cells the
user wants to specify. The function doesn't throw any exceptions but frequently
checks for valid input using the validInput functions and uses while loops to
ensure that the user has multiple chances to enter the proper commands.
*/
void Board::run()
{
  bool inputError = true;
  bool columnError = true;
  bool rowError = true;
  bool fileError = true;
  string startType;
  int boardColumns = 0;
  int boardRows = 0;
  string exitProgram;
  double initialDensity;
  while (inputError){
    cout << "Would you like to start with a random configuration or specify a flat file configuration? " << endl;
    cout << "Enter '1' for the random configuration and '2' for the flat file configuration" << endl;
    cin >> startType;
    if (startType == "2")
    {
      string fileName;
      while (fileError){
        cout << "Enter the file path you would like to import " << endl;
        cin >> fileName;
        ifstream inFile(fileName);
        if (inFile.fail()){
          cout << "There was an error opening the file. Please try again to check a different directory or click 'e' to exit " << endl;
          cin >> exitProgram;
          if(exitProgram == "e"){
            exit(0);
          }
          else{
            initializeBoard(exitProgram);
            fileError = false;
          }
        }
        else{
          initializeBoard(fileName);
          fileError = false;
        }
      }
      inputError = false;
    }
    else if (startType == "1"){
      while (columnError){
        cout << "Enter the number of columns for the board" << endl;
        cin >> boardColumns;
        if (validInput(boardColumns)){
          columnError = false;
        }
        else{
          cout << "This isn't a valid value for the number of columns. Try again" << endl;
        }
      }
      while (rowError){
        cout << "Enter the number of rows for the board" << endl;
        cin >> boardRows;
        if (validInput(boardRows)){
          rowError = false;
        }
        else{
          cout << "This isn't a valid value for the number of rows. Try again" << endl;
        }
      }
      while (bool densityError = true){
        cout << "Enter a value between 0 and 1 for the initial population density" << endl;
        cin >> initialDensity;
        if ((validInput(initialDensity)) && (initialDensity>= 0) && (initialDensity<=1)){
          initializeBoard(initialDensity, boardRows, boardColumns);
          densityError = false;
        }
        else{
          cout << "This isn't a valid density. Enter a value greater than 0 and less than or equal to 1" << endl;
        }
      }
      }
    else{
      cout << "We don't recognize the command. Please try again" << endl;
    }
  }
}

/*
This is the function initializeBoard. It is the initialization function
for the board that is configured randomly. It doesn't return any value, but
it takes as parameters the frequency (a double) of cells that the user wants to have
filled to start, the number of rows (an integer), and the number of columns (an integer).
The function initializes a 1 dimensional array with the proper freqencies,
shuffles those elements, and then converts it into the 2D array. Then the function
calls the function copyBoard.
*/
void Board::initializeBoard(double frequency, int rows, int columns)
{

  char *newArray;
  newArray = new char [rows*columns];
  double cellOccurrences = (rows*columns) * frequency;
  int castedCellOccurrences = static_cast<int>(cellOccurrences + .5);

  //Initialize the 1D array
  for (int q = 0; q < rows*columns; ++q){
    if (castedCellOccurrences > 0){
      newArray[q] = 'X';
      castedCellOccurrences--;
    }
    else{
      newArray[q] = '-';
    }
  }

  //Random shuffle
  for (int m = 0; m < rows*columns; ++m){
    int randomLocation = rand() % (rows*columns);
    char temp = newArray[randomLocation];
    newArray[randomLocation] = newArray[m];
    newArray[m] = temp;
  }

  //Making 2D
  char** board = new char*[rows];
  for (int s = 0; s < rows; ++s){
    board[s] = new char[columns];
  }

  delete[] newArray;

  //Initializing 2D
  int countTemp = 0;
  for (int j = 0; j < rows; ++j){
    for (int s = 0; s < columns; ++s){
      board[j][s] = newArray[countTemp];
      countTemp++;
    }
  }
  //Allocate copied board
  char** copiedBoard = new char*[rows];
  for (int s = 0; s < rows; ++s){
    copiedBoard[s] = new char[columns];
  }
  copyBoard(board, copiedBoard, rows, columns);
}

/*
This is the initializeBoard function for the board that is input by file. It also
has no return type, but it takes in the parameter fileName (a string). This is the
name of the file or the path to the file that the user wants to read in. The function
creates the 2D array after reading in the file, and then calls the function copyBoard.
*/
void Board::initializeBoard(string fileName){
  int rows;
  int columns;
  ifstream inFile(fileName);

  //Get the rows and columns
  inFile >> rows >> columns;

  //Initialize 1D array
  char *newArray;
  newArray = new char [rows*columns];

  for (int i = 0; i < rows*columns; ++i){
    inFile >> newArray[i];
  }

  //Making 2D
  char** board = new char*[rows];
  for (int s = 0; s < rows; ++s){
    board[s] = new char[columns];
  }

  delete[] newArray;

  //Initializing 2D array
  int countTemp = 0;
  for (int j = 0; j < rows; ++j){
    for (int k = 0; k < columns; ++k){
      board[j][k] = newArray[countTemp];
      countTemp++;
    }
  }

  //Allocate copied board
  char** copiedBoard = new char*[rows];
  for (int s = 0; s < rows; ++s){
    copiedBoard[s] = new char[columns];
  }
  copyBoard(board, copiedBoard, rows, columns);
}
/*
The function copyBoard has no return type, but it takes in as parameters pointer
arrays of type char for both the board and copiedBoard. It also takes in as parameters
the integers, rows and columns. The function establishes some pointers to the
respective boards, swaps the pointers, and calls the function neighborsCountHelper.
*/
void Board::copyBoard(char **board, char** copiedBoard, int rows, int columns){

  char** maingrid = board;
  char** nextGrid = copiedBoard;

  char** temp = nextGrid;
  nextGrid = maingrid;
  maingrid = temp;

  neighborsCountHelper(nextGrid, maingrid, rows, columns);
}
/*
The function gameType returns a value of the type string to represent
the mode that the user would like to play in. The function takes in no parameters.
It is called in the function neighborsCountHelper on the first generation only.
It uses the boolean variable modeError and a while loop to check for improper input.
*/
string Board::gameType(){
  bool modeError = true;
  while (modeError){
    cout << "What type of mode would you like to play in? " << endl;
    cout << "Enter '1' for classic mode, '2' for doughnut mode and '3' for mirror mode" << endl;
    cin >> modeCheck;
    if (modeCheck == "1"){
      modeError = false;
      return modeCheck;
    }
    else if (modeCheck == "2"){
      modeError = false;
      return modeCheck;
    }
    else if (modeCheck == "3"){
      modeError = false;
      return modeCheck;
    }
    else{
      cout << "This is an invalid input. Try again." << endl;
    }
  }
  return modeCheck;
}
/*
The function gapType returns a value of the type string. The return value is either
pauseType (how the user wants to pause between generations), or ouFile (the name of the file
they would like to the write the results to). The function takes no parameters, and doesn't
throw any exceptions because any key strokes other than those specified just means
the program will run without any pause specifications.
*/
string Board::gapType(){
  cout << "Enter '1' for a brief pause between generations, '2' if you would like to be forced to hit the enter key for a new generation, and '3' if you would like to output to a file. Enter anything else to run without any of the specifications" << endl;
  cin >> pauseType;
  if (pauseType == "1"){
    cout << "There will be a brief pause between generations" << endl;
  }
  else if (pauseType == "2"){
    cout << "You will be forced to hit the enter key to  move to the next generation" << endl;
  }
  else if (pauseType == "3"){
    cout << "Enter the name of the file you would like to write to" << endl;
    cin >> outFile;
    return outFile;
  }
  return pauseType;
}
/*
The function neighborsCountHelper doesn't return a value but it takes in 4 parameters.
Two are pointer arrays, copiedBoard and board, and the other two are inegers, rows and
columns. On the 0th generation the function calls gameType to see what mode it should run.
Then, iterating through the board, it calls a neighborsCount function depending on
what mode the user decided to play in.
*/
void Board::neighborsCountHelper(char** copiedBoard, char** board, int rows, int columns){

  if (generation == 0){
    modeCheck = gameType();
  }
  if (modeCheck == "1"){
    for (int i = 0; i < rows; ++i){
      for (int j = 0; j < columns; ++j){
        neighborsCount(copiedBoard, board, i, j, rows, columns);
      }
    }
  }
  else if (modeCheck == "2"){
    for (int i = 0; i < rows; ++i){
      for (int j = 0; j < columns; ++j){
        neighborsCountDoughnut(copiedBoard, board, i, j, rows, columns);
        }
      }
    }
  else{
    for (int i = 0; i < rows; ++i){
      for (int j = 0; j < columns; ++j){
        neighborsCountMirror(copiedBoard, board, i, j, rows, columns);
        }
      }
    }
  }


/*
The function neighborsCount doesn't return anything. It takes in 6 parameters as
input. It takes in the pointers to the char arrays, copiedBoard and board, and the integers,
i, j, rows, and columns. This function is called for every cell in the board. The
details of each cell being checked depending on its location on the board are outlined
below. The iterations through s and k are how we search through the 8 neighbors that each cell
has. The integer, count, increments each time a neighbor is found, and then the function nextGeneration
is called for each cell. This is the function that grabs the counts for classic mode.
*/
void Board::neighborsCount(char** copiedBoard, char** board, int i, int j, int rows, int columns){

  int count = 0;

  //Checks upper left corner
  if (i == 0 && j==0){
    for (int s = 0; s < 2; ++s){
      for (int k = 0; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
            }
            break;
          }
        }
      }
  }
  //Checks uppper right corner
  else if (j == columns-1 && i == 0){
    for (int s = 0; s < 2; ++s){
      for (int k = -1; k < 1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
              count++;
            }
            break;
          }
        }
    }
  }
  //Checks lower left corner
  else if (i == rows-1 && j == 0){
    for (int s = -1; s < 1; ++s){
      for (int k = 0; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
              }
          break;
            }
          }
      }
  }
  //Checks lower right corner
  else if (i == rows-1 && j == columns-1){
    for (int s = -1; s < 1; ++s){
      for (int k = -1; k < 1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
              }
          break;
            }
          }
      }
  }
  //Top Cells
  else if (i == 0){
    for (int s = 0; s < 2; ++s){
      for (int k = -1; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;

            }
          }
      }
  }
  //Left cells
  else if (j == 0){
    for (int s = -1; s < 2; ++s){
      for (int k = 0; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;
            }
          }
      }
  }
  //Right cells
  else if (j==columns-1){
    for (int s = -1; s < 2; ++s){
      for (int k = -1; k < 1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;
            }
          }
      }
  }
  //Bottom cells
  else if (i == rows-1){
    for (int s = -1; s < 1; ++s){
      for (int k = -1; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
              count++;
          }
          break;
        }
      }
    }
  }
  //Middle cell
  else if (!((i == 0) || (j == 0) || (j == columns) || (i == rows))){
    for (int s = -1; s<=1; ++s){
      for (int k = -1; k<=1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if(copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;
          }
        }
      }
    }
  nextGeneration(count, board, copiedBoard, i, j, rows, columns);
}



/*
The function neighborsCountDoughnut doesn't return anything. It takes in 6 parameters as
input. It takes in the pointers to the char arrays, copiedBoard and board, and the integers,
i, j, rows, and columns. This function is called for every cell in the board. The
details of each cell being checked depending on its location on the board are outlined
below. The iterations through s and k are how we search through the 8 neighbors that each cell
has. The integer, count, increments each time a neighbor is found, and then the function nextGeneration
is called for each cell. This is the function that grabs the counts for doughnut mode. At each potential
cell, the function breaks out of the nested for loops to check cases specific to doughnut mode.
*/
void Board::neighborsCountDoughnut(char** copiedBoard, char** board, int i, int j, int rows, int columns)
{
  int count = 0;

  //Checks upper left corner
  if (i == 0 && j==0){
    for (int s = 0; s < 2; ++s){
      for (int k = 0; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
            }
            break;
          }
        }
      }
    if (copiedBoard[rows-1][0] == 'X'){
      count++;
    }
    if (copiedBoard[rows-1][1] == 'X'){
      count++;
    }
    if(copiedBoard[0][columns-1] == 'X'){
      count++;
    }
    if(copiedBoard[1][columns-1] == 'X'){
      count++;
    }
    if (copiedBoard[rows-1][columns-1] == 'X'){
      count++;
    }
  }
  //Checks uppper right corner
  else if (j == columns-1 && i == 0){
    for (int s = 0; s < 2; s++){
      for (int k = -1; k < 1; k++){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
            }
          break;
          }
        }
    }
    if (copiedBoard[0][0] == 'X'){
      count++;
    }
    if (copiedBoard[1][0] == 'X'){
      count++;
    }
    if(copiedBoard[rows-1][columns-1] == 'X'){
      count++;
    }
    if(copiedBoard[rows-1][columns-2] == 'X'){
      count++;
    }
    if (copiedBoard[rows-1][0] == 'X'){
      count++;
    }
  }
  //Checks lower left corner -
  else if (i == rows-1 && j == 0){
    for (int s = -1; s < 1; ++s){
      for (int k = 0; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
              }
          break;
            }
          }
      }
    if (copiedBoard[0][1] == 'X'){
      count++;
    }
    if (copiedBoard[0][0] == 'X'){
      count++;
    }
    if(copiedBoard[rows-1][columns-1] == 'X'){
      count++;
    }
    if(copiedBoard[rows-2][columns-1] == 'X'){
      count++;
    }
    if (copiedBoard[0][columns-1] == 'X'){
      count++;
    }
  }
  //Checks lower right corner
  else if (i == rows-1 && j == columns-1){
    for (int s = -1; s < 1; ++s){
      for (int k = -1; k < 1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
              }
          break;
            }
          }
      }
    if (copiedBoard[0][columns-1] == 'X'){
      count++;
    }
    if (copiedBoard[0][columns-2] == 'X'){
      count++;
    }
    if(copiedBoard[rows-1][0] == 'X'){
      count++;
    }
    if(copiedBoard[rows-2][0] == 'X'){
      count++;
    }
    if (copiedBoard[0][0] == 'X'){
      count++;
    }
  }
  //Top Cells
  else if (i == 0){
    for (int s = 0; s < 2; ++s){
      for (int k = -1; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;

            }
          }
      }
    for (int k = -1; k < 2; ++k){
      if (copiedBoard[rows-1][j+k] == 'X'){
        count++;
        }
      }
  }
  //Left cells
  else if (j == 0){
    for (int s = -1; s < 2; ++s){
      for (int k = 0; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;
            }
          }
      }
    for (int s = -1; s < 2; ++s){
      if (copiedBoard[i+s][columns-1] == 'X'){
        count++;
      }
      }
  }
  //Right cells
  else if (j==columns-1){
    for (int s = -1; s < 2; ++s){
      for (int k = -1; k < 1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;
            }
          }
      }
    for (int s = -1; s < 2; ++s){
      if (copiedBoard[i+s][0] == 'X'){
        count++;
      }
      }
  }
  //Bottom cells
  else if (i == rows-1){
    for (int s = -1; s < 1; ++s){
      for (int k = -1; k < 2; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if (copiedBoard[i+s][j+k] == 'X'){
              count++;
          }
          break;
        }
      }
    }
  for (int k = -1; k < 2; ++k){
    if (copiedBoard[0][j+k] == 'X'){
      count++;
    }
    }
  }
  //Middle cell
  else if (!((i == 0) || (j == 0) || (j == columns) || (i == rows))){
    for (int s = -1; s<=1; ++s){
      for (int k = -1; k<=1; ++k){
        while (!(s==0 && k==0)){ //Doesn't check itself as a neighbor
          if(copiedBoard[i+s][j+k] == 'X'){
            count++;
          }
          break;
          }
        }
      }
    }
  nextGeneration(count, board, copiedBoard, i, j, rows, columns);
}

/*
The function neighborsCountMirror doesn't return anything. It takes in 6 parameters as
input. It takes in the pointers to the char arrays, copiedBoard and board, and the integers,
i, j, rows, and columns. This function is called for every cell in the board. The
details of each cell being checked depending on its location on the board are outlined
below. The integer, count, increments each time a neighbor is found, and then the function nextGeneration
is called for each cell. This is the function that grabs the counts for mirror mode.
*/
void Board::neighborsCountMirror(char** copiedBoard, char** board, int i, int j, int rows, int columns){

    int count = 0;

    //Checks upper left corner
    if (i == 0 && j==0){
      if (copiedBoard[i][j] == 'X'){
        count+=3; //corner cell gives 3 neighbors
      }
      if (copiedBoard[i+1][j] == 'X'){
        count+=2; //side cell gives 2 neighbors
      }
      if (copiedBoard[i][j+1] == 'X'){
        count+=2; //side cell gives 2 neighbors
      }
      if (copiedBoard[i+1][j+1] == 'X'){
        count++;
      }
  }
    //Checks uppper right corner
    else if (j == columns-1 && i == 0){
      if (copiedBoard[i][j] == 'X'){
        count+=3;
      }
      if (copiedBoard[i][j-1] == 'X'){
        count+=2;
      }
      if (copiedBoard[i+1][j] == 'X'){
        count+=2;
      }
      if (copiedBoard[i+1][j-1] == 'X'){
        count++;
      }
    }
    //Checks lower left corner
    else if (i == rows-1 && j == 0){
      if (copiedBoard[i][j] == 'X'){
        count+=3;
      }
      if (copiedBoard[i][j+1] == 'X'){
        count+=2;
      }
      if (copiedBoard[i-1][j] == 'X'){
        count+=2;
      }
      if (copiedBoard[i-1][j+1] == 'X'){
        count++;
      }
    }
    //Checks lower right corner
    else if (i == rows-1 && j == columns-1){
      if (copiedBoard[i][j] == 'X'){
        count+=3;
      }
      if (copiedBoard[i-1][j] == 'X'){
        count+=2;
      }
      if (copiedBoard[i][j-1] == 'X'){
        count+=2;
      }
      if (copiedBoard[i-1][j-1] == 'X'){
        count++;
      }
    }
    //Top Cells
    else if (i == 0){
      if (copiedBoard[i][j] == 'X'){
        count++;
      }
      for (int s = -1; s < 2; ++s){
        while (s!=0){
          if (copiedBoard[i][j+s] == 'X'){
            count+=2;
          }
          break;
        }
        if (copiedBoard[i+1][j+s] == 'X'){
          count++;
        }
    }
  }
    //Left cells
    else if (j == 0){
      if (copiedBoard[i][j] == 'X'){
        count++;
      }
      for (int s = -1; s < 2; ++s){
        while (s!=0){
          if (copiedBoard[i+s][j] == 'X'){
            count+=2;
          }
          break;
        }
        if (copiedBoard[i+s][j+1] == 'X'){
          count++;
        }
        }
    }
    //Right cells
    else if (j==columns-1){
      if (copiedBoard[i][j] == 'X'){
        count++;
      }
      for (int s = -1; s < 2; ++s){
        while (s!=0){
          if (copiedBoard[i+s][j] == 'X'){
            count+=2;
          }
          break;
        }
        if (copiedBoard[i+s][j-1] == 'X'){
          count++;
        }
        }
    }
    //Bottom cells
    else if (i == rows-1){
      if (copiedBoard[i][j] == 'X'){
        count++;
      }
      for (int s = -1; s < 2; ++s){
        while (s!=0){
          if (copiedBoard[i][j+s] == 'X'){
            count+=2;
          }
          break;
        }
        if (copiedBoard[i-1][j+s] == 'X'){
          count++;
        }
      }
    }
    //Middle cell
    else if (!((i == 0) || (j == 0) || (j == columns) || (i == rows))){
      for (int s = -1; s<=1; ++s){
        for (int k = -1; k<=1; ++k){
          while (!(s==0 && k==0)){
            if(copiedBoard[i+s][j+k] == 'X'){
              count++;
            }
            break;
            }
          }
        }
      }
    nextGeneration(count, board, copiedBoard, i, j, rows, columns);
}

/*
The function nextGeneration is called by all the neighborsCount functions after
the neighbors are counted for each individual cell. This function takes in as parameters
the integer neighbors, the char pointer arrays board and copiedBoard, and the integers
i, j, rows, and columns. The paramters i and j specify the location on the board. The
function updates the next generation's board for each individual cell as called. It also
checks the pause specifications by calling the function gapType, and then calls the appropriate
function to either print to the board, pause between generations, etc.
*/
void Board::nextGeneration(int neighbors, char** board, char** copiedBoard, int i, int j, int rows, int columns){

  //Dead
  if (neighbors < 2){
    board[i][j] = '-';
  }
  //Same
  else if (neighbors == 2){
    board[i][j] = copiedBoard[i][j];
  }
  //New birth
  else if (neighbors == 3){
    board[i][j] = 'X';
  }
  //Dead
  else{
    board[i][j] = '-';

  }
  if (generation == 0 && i == 0 && j == 0){
    pauseType = gapType();
    if (outFile != "1"){
      cout << "The results have been printed to " << outFile << endl;
    }
  }
  if (outFile == "1"){
    printBoard(board, copiedBoard, rows, columns, i, j, pauseType);
  }
  else{
    filePrintBoard(board, copiedBoard, rows, columns, i, j);
  }
}

/*
The function stagnatedBoard takes in 4 parameters. Two pointers to char arrays, board
and copiedBoard, and two integers, rows and columns. The function returns a value of
the type boolean. It checks each cell and determines if board is equal to copiedBoard
at every cell. If so, the function returns true, but if any cells don't match, the function
returns false.
*/
bool Board::stagnatedBoard(char** board, char** copiedBoard,  int rows, int columns){
  for(int i = 0; i <rows; ++i){
    for (int j = 0; j<columns; ++j){
      if (board[i][j]!=copiedBoard[i][j]){
        return false;
      }
    }
  }
  return true;
}

/*
The function emptyBoard takes in three inputs as parameters. Those parameters
are the pointer to the char array board and the integers, rows and columns. The
function returns a value of the type boolean. It iterates through every cell
in board and checks if the cell is occupied with an 'X'. If one of the cells
is occupied the function returns false. Otherwise, if all the cells are unoccupied
the function returns true.
*/
bool Board::emptyBoard(char** board, int rows, int columns){
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < columns; ++j){
      if (board[i][j] == 'X'){
        return false;
      }
    }
  }
  return true;
}

/*
The function printBoard doesn't return a value but it takes in 7 parameters.
It takes in the pointers to the char arrays, board and copiedBoard. It also
takes in 4 integers - rows, columns, x, and y. The integers x and y refer to
the location of the cell being printed. Lastly, it takes in the string pauseType.
PauseType is merely the output of the function gapType. The function starts by
calling the functions stagnatedBoard and emptyBoard to establish boolean values
for whether or not the board is stagnated or empty. Then, if it's the first generation
(or generation = 0), the function prints copiedBoard and then calls the function copyBoard,
after checking if the board is empty. Then for any generation that is greater than 0,
the function prints board and then calls the function copyBoard, provided that the
board isn't empty or hasn't stagnated.
*/
void Board::printBoard(char** board, char** copiedBoard, int rows, int columns, int x, int y, string pauseType){

  bool stagnation = stagnatedBoard(board, copiedBoard, rows, columns);
  bool emptied = emptyBoard(board, rows, columns);

  if (generation == 0){
    bool emptied = emptyBoard(copiedBoard, rows, columns);
    if (x == 0 && y == 0){
      cout << "\n";
      cout << generation << endl;
      cout << copiedBoard[x][y];
    }
    else if (y == columns-1 && x == rows -1){
      cout << copiedBoard[x][y];
      if (pauseType == "2"){
        userReturn();
      }
      if (stagnation == 1 || emptied == 1){
        userExit(stagnation, board, copiedBoard, rows, columns);
      }
      generation++;
      copyBoard(copiedBoard, board, rows, columns);
    }
    else if (y == columns-1){
      cout << copiedBoard[x][y];
      cout << "\n";
    }
    else{
      cout << copiedBoard[x][y];
    }
  }

  else if (generation <= 1000){
    if (x == 0 && y == 0){
      cout << "\n";
      if (pauseType == "1"){
        usleep(1000000);
      }
      else if (pauseType == "2"){
        userReturn();
      }
      cout << generation << endl;
      cout << board[x][y];
    }
    else if (y == columns-1 && x == rows -1){
      cout << board[x][y];
      if (stagnation == 1 || emptied == 1){
        userExit(stagnation, board, copiedBoard, rows, columns);
      }
      if (generation == 1000){
        cout << "\n";
        cout << "1000 generations have been printed and cells are still occupied" << endl;
      }
      generation++;
      copyBoard(board, copiedBoard, rows, columns);
    }
    else if (y == columns-1){
      cout << board[x][y];
      cout << "\n";
    }
    else{
      cout << board[x][y];
    }
  }
}

/*
The function userReturn doesn't return a value and it doesn't take in
any parameters. The function merely forces the user to hit enter each time
to see the next generation by using the ignore and get functions for the
delimiter.
*/
void Board::userReturn(){
  cin.ignore(0, '\r').get();
}

/*
The function userExit doesn't return a value. It takes in one parameter, a boolean
called stagnated. If the board has stagnated, the function prompts you to quit by
hitting enter - letting you know that the board has stagnated. Otherwise, based on when
it's called, the board must be empty, so it prompts you to quit by hitting enter - letting
you know that the board has become empty.
*/
void Board::userExit(bool stagnated, char** board, char** copiedBoard, int rows, int columns){
  if (stagnated){
    string exitProgram;
    cout << "\n";
    cout << "Hit enter to quit. The board has stagnated on generation " << generation << " and will not change in future generations.";
    exitProgram = cin.get();
    if (exitNumber == 0){
      cin.ignore(1, '\n');
      if (exitProgram == "\n"){
        cout << "exited" << endl;
        exit(0);
      }
    }
    else if (exitProgram == "\n"){
      cout << "exited" << endl;
      exit(0);
    }
  }
  else{
    string exitProgram;
    cout << "\n";
    cout << "Hit enter to quit. The board has become empty on generation " << generation << " and will not change in future generations.";
    exitProgram = cin.get();
    if (exitNumber == 0){
      cin.ignore(1, '\n');
      if (exitProgram == "\n"){
        cout << "exited" << endl;
        exit(0);
      }
    }
    else if (exitProgram == "\n"){
      cout << "exited" << endl;
      exit(0);
    }
  }
  exitNumber++;
}

/*
The function validInput takes in the parameter of type double, input, and returns
a value of type bool. If the input doesn't match type double the function returns
false. Otherwise it returns true. It is called in the run function when the user
is inputting the density filled cells for the grid.
*/
bool Board::validInput(double input){
  if (!input){
    cin.clear(); //clears the error flag on cin so future operations work
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignore anything up to the new line character
    return false;
  }
  return true;
}

/*
The function validInput takes in the parameter of type int, input, and returns
a value of type bool. If the input doesn't match type int the function returns
false. Otherwise it returns true. It is called in the run function when the user
is inputting the rows and columns.
*/
bool Board::validInput(int input){
  if (!input){
    cin.clear(); //clears the error flag on cin so future operations work
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignore anything up to the new line character
    return false;
  }
  return true;
}

/*
The function filePrintBoard doesn't return anything but it takes in 6 parameters.
The parameters are two pointers to char arrays - board and copiedBoard - and 4 integers -
rows, columns, x, and y. The function acts identically to printBoard, except it is
printing to the file specified in gapType. It also checks for stagnation and an empty
board. If either of these are the case, the user is prompted to exit the program.
*/
void Board::filePrintBoard(char** board, char** copiedBoard, int rows, int columns, int x, int y){

  bool stagnation = stagnatedBoard(board, copiedBoard, rows, columns);
  bool emptied = emptyBoard(board, rows, columns);

  ofstream printFile;
  printFile.open(outFile, ios_base::app);
  if (printFile.is_open()){
    if (generation == 0){
      bool emptied = emptyBoard(copiedBoard, rows, columns);
      if (x == 0 && y == 0){
        printFile << endl;
        printFile << generation << endl;
        printFile << copiedBoard[x][y];
      }
      else if (y == columns-1 && x == rows-1){
        printFile << copiedBoard[x][y] << "\n";
        if (stagnation == 1 || emptied == 1){
          userExit(stagnation, board, copiedBoard, rows, columns);
        }
        generation++;
        printFile.close();
        copyBoard(copiedBoard, board, rows, columns);
      }
      else if (y == columns-1){
        printFile << copiedBoard[x][y];
        printFile << "\n";
      }
      else{
        printFile << copiedBoard[x][y];
      }
    }
    else if (generation <= 1000){
      if (x == 0 && y == 0){
        printFile << "\n";
        printFile << generation << endl;
        printFile << board[x][y];
      }
      else if (y == columns-1 && x == rows -1){
        printFile << board[x][y] << "\n";
        if (stagnation == 1 || emptied == 1){
          userExit(stagnation, board, copiedBoard, rows, columns);
        }
        if (generation == 1000){
          printFile << "\n";
          printFile << "1000 generations have been printed and cells are still occupied" << endl;
        }
        generation++;
        printFile.close();
        copyBoard(board, copiedBoard, rows, columns);
      }
      else if (y == columns-1){
        printFile << board[x][y];
        printFile << "\n";
      }
      else{
        printFile << board[x][y];
      }
    }
  }
  else{
    cout << "There was an error opening the file " << endl;
  }
}
