#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "sudoku.h"

using namespace std;

// START: The main function
int main(int argc, char const *argv[])
{
  // Initialising seed for random number generation
  srand(time(NULL));

  // Creating an instance of Sudoku
  cout << "======================================" <<endl;
  cout << "random generating puzzle by SAT...." <<endl;
  cout << "======================================" <<endl;
  Sudoku *puzzle = new Sudoku();

  // Creating a seed for puzzle generation
  puzzle->createSeed();

  // Generating the puzzle cage
  puzzle->genPuzzle();

  // testing by printing the grid
  // puzzle->printGrid();

  // Printing the grid into SVG file
  string rem = "sudokuGen";
  string path = argv[0];
  path = path.substr(0,path.size() - rem.size());
  puzzle->printSVG(path);
  puzzle->printSVG(path, "images/puzzles_sol.svg", true);

  cout << "======================================" <<endl;
  cout << "trying to solve puzzle by SAT...." <<endl;
  cout << "======================================" <<endl;

  puzzle->solveBySAT();
  puzzle->printSVG(path, "images/puzzles_solbySAT.svg", true);
  // cout<<"The above sudoku puzzle has been stored in puzzles.svg in current folder\n";
  // freeing the memory
  // puzzle->printGrid();
  delete puzzle;

  return 0;
}
// END: The main function
