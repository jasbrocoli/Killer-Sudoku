# Killer-Sudoku Generator with SAT
  
**This project generates Killer-Sudoku puzzle in C++, and solved by SAT later.**

## Steps to Use:-  

### Requirements: 
-> git  
-> Latest version C++ compiler , (this program has been tested on g++ only)   

### Linux and MacOS

Type the follwing commands on your terminal (without the '$')  
```
$ git clone https://github.com/Yuling-Hsu/Killer-Sudoku.git
$ cd Killer-Sudoku
$ make
```  

**Now restart the terminal and you are good to go**  

To run => enter `$ sudokuGen` from anywhere in the terminal  
You can view the svg image generated in any Browser.  
   
### Output Images

After executing `$ sudokuGen`, you will get svg files under the folder `images/`:
* `puzzles_sol.svg` is the initial solution created when generating the puzzle
* `puzzles_solbySAT.svg` is the solution created by SAT solver

### Example Puzzle generated from the program
![image](https://github.com/Yuling-Hsu/Killer-Sudoku/blob/main/images/puzzle.svg)  

### Example Puzzle Solution by SAT generated from the program
![image](https://github.com/Yuling-Hsu/Killer-Sudoku/blob/main/images/puzzles_solbySAT.svg) 

### Variables that can be customized:  
In `suduku.h`: `maxCageSizeMinus1` and `gridSize` can be modified to get different hard level of the puzzle.
Initially I set it to 5 and 9 respectively.