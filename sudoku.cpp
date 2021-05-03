#include "sudoku.h"

// START: Get grid as string in row major order
string Sudoku::getGrid()
{
  string s = "";
  for(int row_num=0; row_num<gridSize; ++row_num)
  {
    for(int col_num=0; col_num<gridSize; ++col_num)
    {
      s = s + to_string(_grid[row_num][col_num]);
    }
  }

  return s;
}
// END: Get grid as string in row major order

// START: Create seed grid
void Sudoku::createSeed()
{ 
  this->solveBySAT();
  
  // Saving the solution grid
  for(int i=0;i<gridSize;i++)
  {
    for(int j=0;j<gridSize;j++)
    {
      _solnGrid[i][j] = _grid[i][j];
    }
  }
}
// END: Create seed grid


// START: Intialising
Sudoku::Sudoku()
{
  // Initialising the grid
  for(int i=0;i<gridSize;i++)
  {
    for(int j=0;j<gridSize;j++)
    {
      _grid[i][j]=0;
      _cageId[i][j] = -1;
    }
  }

}
// END: Initialising


// START: Printing the grid
void Sudoku::printGrid()
{
  for(int i=0;i<gridSize;i++)
  {
    for(int j=0;j<gridSize;j++)
    {
      if(_grid[i][j] == 0)
	cout<<".";
      else
	cout<<_grid[i][j];
      cout<<"|";
    }
    cout<<endl;
  }

  cout<<endl;
}
// END: Printing the grid

// START: Gneerate puzzle
void Sudoku::genPuzzle()
{
  size_t sum, sizeOfCage, dir, extPo, currentID, pox, poy;
  int deadLock, gridValue;
  vector<int> cageAppeared;
  for(size_t j=0; j<gridSize; ++j){
      for(size_t i=0; i<gridSize; ++i){
        if(_cageId[i][j] > -1){
            continue;
        }

        deadLock = 0;
        cageAppeared.clear();
        sizeOfCage = rand()%maxCageSizeMinus1 + 2;
        currentID = _cages.size();

        _cageId[i][j] = currentID;

        sum = _grid[i][j];
        
        vector<Position> temp;

        temp.push_back({i, j, sum});
        cageAppeared.push_back(sum);

        while (temp.size() < sizeOfCage && deadLock < 1)
        {      
            extPo = rand()%temp.size();  
            pox = temp[extPo].x;
            poy = temp[extPo].y;    

            dir = rand()%2;
            switch (dir)
            {
            case 0:
                if(pox<8 && _cageId[pox+1][poy] < 0){
                    gridValue = _grid[pox+1][poy];
                    if(find(cageAppeared.begin(), cageAppeared.end(), gridValue) == cageAppeared.end()){
                        temp.push_back({pox+1, poy, gridValue});
                        _cageId[pox+1][poy] = currentID;
                        cageAppeared.push_back(gridValue);
                        sum += gridValue;
                        break;
                    }
                } 
            case 1:
                if(poy<8 && _cageId[pox][poy+1] < 0){
                    gridValue = _grid[pox][poy+1];
                    if(find(cageAppeared.begin(), cageAppeared.end(), gridValue) == cageAppeared.end()){
                        temp.push_back({pox, poy+1, gridValue});
                        _cageId[pox][poy+1] = currentID;
                        cageAppeared.push_back(gridValue);
                        sum += gridValue;
                        break;
                    }
                }
            case 2:
                if(pox>1 && _cageId[pox-1][poy] < 0){
                    gridValue = _grid[pox-1][poy];
                    if(find(cageAppeared.begin(), cageAppeared.end(), gridValue) == cageAppeared.end()){
                        temp.push_back({pox-1, poy, gridValue});
                        _cageId[pox-1][poy] = currentID;
                        cageAppeared.push_back(gridValue);
                        sum += gridValue;
                        break;
                    }
                }
            case 3:
                if(poy>1 && _cageId[pox][poy-1] < 0 ){
                    gridValue = _grid[pox][poy-1];
                    if(find(cageAppeared.begin(), cageAppeared.end(), gridValue) == cageAppeared.end()){
                        temp.push_back({pox, poy-1, gridValue});
                        _cageId[pox][poy-1] = currentID;
                        cageAppeared.push_back(gridValue);
                        sum += gridValue;
                        break;
                    }
                }          
            default:
                ++deadLock;
                break;
            }
        }

        if(temp.size() == 1){
            switch (dir)
            {
            case 0:
                if(i>1 && _cages[_cageId[i-1][j]].addEle(temp[0], sum)){
                    _cageId[i][j] = _cageId[i-1][j];
                    continue;
                }            
            default:
                if(j>1 && _cages[_cageId[i][j-1]].addEle(temp[0], sum)){
                    _cageId[i][j] = _cageId[i][j-1];
                    continue;
                }
            }
        }

        sort(temp.begin(), temp.end(), sortGrid);
        _cages.push_back(Cage(currentID, sum, temp));
      } 
  }
}
// END: Generate puzzle


// START: Printing into SVG file
void Sudoku::printSVG(string path,string svgName, bool printSol)
{
  string fileName = path + "svgHead.txt";
  ifstream file1(fileName.c_str());
  stringstream svgHead;
  svgHead << file1.rdbuf();

  ofstream outFile(svgName);
  outFile << svgHead.rdbuf();

  file1.close();

  int width = 50 * (gridSize+2);

  stringstream head;
  head << width << "\" height=\"" << width << "\">\n" ;
  outFile << head.rdbuf();

  for(int j=0;j<gridSize;j++)
  {
    for(int i=0;i<gridSize;i++)
    {
      if(this->_grid[i][j]!=0)
      {
        int x = 50*j;
        int y = 50*i;

        stringstream text;
        text << "<rect x=\""<<x<<"\" y=\""<<y<<"\" width=\"50\" height=\"50\" style=\"fill:" << _color[_cageId[i][j]] <<";opacity:"<< _color.opacity(_cageId[i][j]) << "\"/>\n";
        if(printSol){
            text<<"<text x=\""<<x+16<<"\" y=\""<<y+35<<"\" style=\"font-weight:bold\" font-size=\"30px\">"<<_grid[i][j]<<"</text>\n";
        }
        outFile << text.rdbuf();
      }
    }    
  }

  for(auto it=_cages.begin(); it!=_cages.end(); ++it){
      int x = 50* it->getPoy(0) + 8;
      int y = 50* it->getPox(0) + 18;

      stringstream text;
      text<<"<text x=\""<<x<<"\" y=\""<<y<<"\" style=\"font-weight:bold\" fill=\"red\" font-size=\"15px\">"<<it->getSum()<<"</text>\n";
      outFile << text.rdbuf();
  }

  for(int i=0; i<=gridSize; ++i){
      stringstream text, text2;
      text<<  "<polyline points=\"" << 50*i << ",0 " << 50*i << "," << width-100 << "\" style=\"fill:none; stroke:black ; stroke-width:";
      text2<<  "<polyline points=\"0," << 50*i << " " << width-100 << "," << 50*i<< "\" style=\"fill:none; stroke:black ; stroke-width:";
      if(i%boxSize == 0){
          text << 5;
          text2 << 5;
      }
      else{
          text << 1;
          text2 << 1;
      }
      text <<"\" />\n";
      text2 <<"\" />\n";
      
      outFile << text.rdbuf();
      outFile << text2.rdbuf();
  }

  outFile << "</svg>";

  outFile.close();

}
// END: Printing into SVG file

// Initialization for color
Color Sudoku::_color = Color();

bool sortGrid(const Position& a, const Position& b){
    return (a.y != b.y)?(a.y < b.y):(a.x < b.x);
}

void Sudoku::gateInitial(){

    _solver.initialize();
    _gates = new Gate**[gridSize];

    for(int i=0; i<gridSize; ++i){
        _gates[i] = new Gate*[gridSize]; 
        for(int j=0; j<gridSize; ++j){
            _gates[i][j] = new Gate[gridSize];
            for(int k=0; k<gridSize; ++k){
                Var v = _solver.newVar();
                _gates[i][j][k].setVar(v);
            }
        }
    }
    
    
    for(size_t s = _cages.size(), i=0; i<s; ++i){
        _cages[i].setGate(_solver);
    }
}

void Sudoku::genProofModel(){
    vec<Lit> lits;
    // entry condition
    for(int j=0; j<gridSize; ++j){
        for(int i=0; i<gridSize; ++i){
            for(int k=0; k<gridSize; ++k){
                lits.push(Lit(_gates[i][j][k].getVar()));
            }
            _solver.addCNF(lits); lits.clear();
        }
    }
    for(int j=0; j<gridSize; ++j){
        for(int i=0; i<gridSize; ++i){
            for(int k=0; k<gridSize-1; ++k){
                for(int z=k+1; z<gridSize; ++z){
                    lits.push(~Lit(_gates[i][j][k].getVar()));
                    lits.push(~Lit(_gates[i][j][z].getVar()));
                    _solver.addCNF(lits); lits.clear();
                }                
            }
        }
    }

    // row 
    for(int j=0; j<gridSize; ++j){
        for(int k=0; k<gridSize; ++k){
            for(int i=0; i<gridSize-1; ++i){
                for(int q=i+1; q<gridSize; ++q){
                    lits.push(~Lit(_gates[i][j][k].getVar()));
                    lits.push(~Lit(_gates[q][j][k].getVar()));
                    _solver.addCNF(lits); lits.clear();
                }
            }
        }
    }

    // column
    for(int i=0; i<gridSize; ++i){
        for(int k=0; k<gridSize; ++k){
            for(int j=0; j<gridSize-1; ++j){
                for(int q=j+1; q<gridSize; ++q){
                    lits.push(~Lit(_gates[i][j][k].getVar()));
                    lits.push(~Lit(_gates[i][q][k].getVar()));
                    _solver.addCNF(lits); lits.clear();
                }
            }
        }
    }

    // box
    for(int k=0; k<gridSize; ++k){
        for(int q=0; q<boxSize; ++q){
            for(int r=0; r<boxSize; ++r){
                for(int i=0; i<boxSize; ++i){
                    for(int j=0; j<boxSize; ++j){
                        for(int s=j+1; s<boxSize; ++s){
                            lits.push(~Lit(_gates[boxSize*q+i][boxSize*r+j][k].getVar()));
                            lits.push(~Lit(_gates[boxSize*q+i][boxSize*r+s][k].getVar()));
                            _solver.addCNF(lits); lits.clear();
                        }
                    }
                }
            }
        }
    }

    for(int k=0; k<gridSize; ++k){
        for(int q=0; q<boxSize; ++q){
            for(int r=0; r<boxSize; ++r){
                for(int i=0; i<boxSize; ++i){
                    for(int j=0; j<boxSize; ++j){
                        for(int s=i+1; s<boxSize; ++s){
                            for(int t=0; t<boxSize; ++t){
                                lits.push(~Lit(_gates[boxSize*q+i][boxSize*r+j][k].getVar()));
                                lits.push(~Lit(_gates[boxSize*q+s][boxSize*r+t][k].getVar()));
                                _solver.addCNF(lits); lits.clear();
                            }
                        }
                    }
                }
            }
        }
    }

    vector<int> numbers;
    vector<int> partial;

    for(int i=1; i<=gridSize; ++i){
        numbers.push_back(i);
    }

    // sum
     for(auto it=_cages.begin(); it!=_cages.end(); ++it){
        /** this is concerned already bellow
         // each num appears at most once in the cage
         for(size_t s=it->getCageSize()-1, i=0; i<s; ++i){
            for(int k=0; k<9; ++k){
                lits.push(~Lit(gates[it->getPox(i)][it->getPoy(i)][k].getVar()));
                lits.push(~Lit(gates[it->getPox(i+1)][it->getPoy(i+1)][k].getVar()));
                solver.addCNF(lits); lits.clear();
            }
         }
        **/ 

         size_t cs = it->getCageSize();
         int sum = it->getSum();
         vector<vector<int> > answers;
         subsetSum(numbers, cs, sum, partial, answers);
         vec<Lit> validSols;
         for(auto ans=answers.begin(); ans!=answers.end(); ++ans){
             if(ans->size() == cs){
                 do{
                    for(size_t i=0; i<cs; ++i){
                        lits.push(Lit(_gates[it->getPox(i)][it->getPoy(i)][(*ans)[i]-1].getVar()));
                    }
                    Var v = _solver.newVar();
                    _solver.addAND(v, lits); lits.clear();

                    validSols.push(Lit(v));
                 }while(next_permutation(ans->begin(), ans->end()));
             }
         }
         _solver.addOR(it->getGate().getVar(), validSols);
     }

}

bool Sudoku::solveBySAT(){
    gateInitial();

    clock_t start, end;
    start = clock();
    genProofModel();
    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by genProofModel() is : " << fixed 
         << time_taken << setprecision(5);
    cout << " sec " << endl;

    bool result;
    // k = Solve(Gate(5) ^ !Gate(8))
    // Var newV = solver.newVar();
    // solver.addXorCNF(newV, gates[5]->getVar(), false, gates[8]->getVar(), true);
    _solver.assumeRelease();  // Clear assumptions

    // letting all sum condition be true
    for(size_t i=0, s=_cages.size(); i<s; ++i){
        _solver.assumeProperty(_cages[i].getGate().getVar(), true);
    }
    // solver.assumeProperty(newV, true);  // k = 1
    start = clock();
    result = _solver.assumpSolve();
    end = clock();
    time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by solver.assumpSolve() is : " << fixed 
         << time_taken << setprecision(5);
    cout << " sec " << endl;
    _solver.printStats();
    cout << (result? "SAT" : "UNSAT") << endl;
    if (result) {
        for (size_t i = 0, n=gridSize; i < n; ++i)
        {
            for(size_t j=0; j<gridSize; ++j){
                for(size_t k=0; k<gridSize; ++k){
                    if(_solver.getValue(_gates[i][j][k].getVar())){
                        _grid[i][j] = k+1;
                        break;
                    }
                }
            }
        }
    }
    else{
        for(size_t j=0; j<gridSize; ++j){
            for(size_t i=0; i<gridSize; ++i){
                    _grid[i][j] = 0;
            }
        }
    }

    for(int i=0; i<gridSize; ++i){
        for(int j=0; j<gridSize; ++j){
            delete [] _gates[i][j];
        }
        delete [] _gates[i];
    }
    delete [] _gates;
}

void Sudoku::subsetSum(vector<int>numbers, const int&s,  const int& target, vector<int> partial, vector<vector<int> >& answer)
{
    int sum = accumulate(partial.begin(), partial.end(), 0);

    if(sum == target){
        answer.push_back(partial);
    }

    if(sum >= target || partial.size() >= s){
        return;
    }

    for(auto it=numbers.begin(); it!=numbers.end(); ++it){
        partial.push_back(*it);
        vector<int> remainig(it+1, numbers.end());
        subsetSum(remainig, s, target, partial, answer);
        partial.pop_back();
    }
}