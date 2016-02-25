#include <iostream>
#include "genetichost.h"

#ifndef srcPath
#define srcPath "."
#endif

using namespace std;
int main() {
//  GeneticHost host(8);
//  bool loadAiFile=true;
//  host.startEvolution(30,100,loadAiFile,srcPath "/aiSave2.txt");

//  GameJudge judge;
//  OthelloAI ai1(8);
//  OthelloAI ai2(8);
//  ai1.useRecommandedChessBoardScore();
//  ai2.useRecommandedChessBoardScore();
//  judge.PlayAGame_getScore(ai1,ai2,8,1,true);
  int N=8;
  GameState gs(N);
  gs.restartGame();
  //    01234567
  //  0 --------
  //  1 --------
  //  2 ---b----
  //  3 ---BB---
  //  4 ---BW---
  //  5 --------
  //  6 --------
  //  7 --------
  gs.addPiece(2, 3, Color::Black);
  gs.printBoard();
  cout<<endl;
  //    01234567
  //  0 --------
  //  1 --------
  //  2 ---B----
  //  3 ---BB---
  //  4 --wWW---
  //  5 --------
  //  6 --------
  //  7 --------
  gs.addPiece(4, 2, Color::White);
  gs.printBoard();
  cout<<endl;
  //    01234567
  //  0 --------
  //  1 --------
  //  2 ---B----
  //  3 ---BB---
  //  4 --WBW---
  //  5 --b-----
  //  6 --------
  //  7 --------
  gs.addPiece(5, 2, Color::Black);
  gs.printBoard();
  cout<<endl;


  return 0;
}
