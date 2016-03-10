#include <iostream>
#include "genetichost.h"
#include <vector>
#include "tools.h"
#ifndef srcPath
#define srcPath "."
#endif

using namespace std;
int main() {
//  GeneticHost host(8);
//  bool loadAiFile=false;
//  host.startEvolution(25,100,loadAiFile,srcPath "/aiSave3.txt");

  GameJudge judge;
  vector<OthelloAI*> zoo;
  zoo=readAIFile(srcPath "/aiSave3.txt",8);

  judge.HumanVsAi(*zoo[0],8,3,Color::Black);

  for(auto z:zoo) delete z;


  return 0;
}
