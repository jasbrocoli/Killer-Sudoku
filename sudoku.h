#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numeric>
#include <bits/stdc++.h>

#include "color.h"
#include "gate.h"

#define UNASSIGNED 0

using namespace std;

struct Position {
    size_t x, y;
    int val;
};

bool sortGrid(const Position& a, const Position& b);
const size_t boxSize = 3;
const size_t gridSize = boxSize*boxSize;
const size_t maxCageSizeMinus1 = 5; //gridSize - 4;

class Cage {
public:
    Cage(vector<Position>& e){
        _cageId = -1;
        _sum = 0;
        _eles = e;
    }
    Cage(int i, int s, vector<Position>& e){
        _cageId = i;
        _sum = s;
        _eles = e;
    }
    ~Cage(){}

    int getSum() const { return _sum; }
    int getID() const { return _cageId; }
    size_t getCageSize() const {return _eles.size();}
    size_t getPox(size_t i) const {return _eles[i].x;}
    size_t getPoy(size_t i) const {return _eles[i].y;}
    Gate& getGate() {return _g;}
    void setGate(SatSolver& s) {
        Var v = s.newVar();
        _g.setVar(v);
    }
    bool addEle(Position e, int i) {
        if(getCageSize() > maxCageSizeMinus1){
            return false;
        }
        for(Position x : _eles){
            if(x.val == i){
                return false;
            }
        }
        _eles.push_back(e); 
        _sum += i;
        return true;
    }

private:
    int _cageId;
    int _sum;
    vector<Position> _eles;
    Gate _g;
};

class Sudoku {
private:
  int _grid[gridSize][gridSize];
  int _solnGrid[gridSize][gridSize];
  int _cageId[gridSize][gridSize];
  vector<Cage> _cages;
  Gate*** _gates;

  static Color _color;

  void gateInitial();
  void genProofModel();
  void subsetSum(vector<int>numbers, const int& s, const int& target, vector<int> partial, vector<vector<int> >& answer);
  SatSolver _solver;

public:
  Sudoku ();
  void createSeed();
  void printGrid();
  string getGrid();
  void genPuzzle();
  void printSVG(string path="",string svgName="images/puzzle.svg", bool printSol=false);
  bool solveBySAT();
};

#endif