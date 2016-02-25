#include <iostream>
#include "GameState.h"
#include "GameJudge.h"
#include "OthelloAI.h"
#include "genetichost.h"

#ifndef srcPath
#define srcPath "."
#endif

using namespace std;
int main() {
  //GeneticHost host(8);
  //bool loadAiFile=true;
  //host.startEvolution(50,100,loadAiFile,srcPath "/aiSave.txt");

  GameJudge judge;
  OthelloAI ai1(8);
  OthelloAI ai2(8);
  ai1.useRecommandedChessBoardScore();
  ai2.useRecommandedChessBoardScore();
  judge.PlayAGame_getScore(ai1,ai2,8,1,true);

  return 0;
}
