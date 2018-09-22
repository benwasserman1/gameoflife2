#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

class Board{
public:
  char** board;
  char** copiedBoard;
  int rows;
  int columns;
  Board();
  ~Board();
  Board(int rows, int columns);
  void run();
  void initializeBoard(double frequency, int rows, int columns);
  void initializeBoard(string fileName);
  bool stagnatedBoard(char** board, char** copiedBoard, int rows, int columns);
  bool emptyBoard(char** board, int rows, int columns);
  void copyBoard(char** board, char** copiedBoard, int rows, int columns);
  void printBoard(char** board, char** copiedBoard, int rows, int columns, int x, int y, string pauseType);
  void filePrintBoard(char** board, char** copiedBoard, int rows, int columns, int x, int y);
  string gameType();
  string gapType();
  bool validInput(double input);
  bool validInput(int input);
  void userReturn();
  void userExit(bool stagnated, char** board, char** copiedBoard, int rows, int columns);
  void neighborsCountHelper(char** copiedBoard, char** board, int rows, int columns);
  void neighborsCount(char** copiedBoard, char** board, int x, int y, int rows, int columns);
  void nextGeneration(int neighbors, char** board, char** copiedBoard, int i, int j, int rows, int columns);
  void neighborsCountDoughnut(char** copiedBoard, char** board, int x, int y, int rows, int columns);
  void neighborsCountMirror(char** copiedBoard, char** board, int i, int j, int rows, int columns);
};
