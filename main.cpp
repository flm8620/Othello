#include <iostream>
#include "genetichost.h"

#ifndef srcPath
#define srcPath "."
#endif

using namespace std;
int main() {
  GeneticHost host(8);
  bool loadAiFile=false;
  host.startEvolution(30,100,loadAiFile,srcPath "/aiSave3.txt");

//  GameJudge judge;
//  OthelloAI ai1(8);
//  OthelloAI ai2(8);
//  ai1.useRecommandedChessBoardScore();
//  ai2.useRecommandedChessBoardScore();
//  judge.PlayAGame_getScore(ai1,ai2,8,1,true);


  return 0;
}
