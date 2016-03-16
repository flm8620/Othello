#include <iostream>
#include "genetichost.h"
#include <vector>
#include "tools.h"
#ifndef srcPath
#define srcPath "."
#endif

using namespace std;

string recordFile = srcPath "/record4.txt";
string aiFile = srcPath "/aiSave4.txt";

int main() {
  GeneticHost host(8);
  bool loadAiFile=true;
  double roundTime = 0.001;
  int numIndividual = 30;
  int numGeneration=1000;
  host.startEvolution(numIndividual,numGeneration,loadAiFile,aiFile,recordFile,roundTime);

  //  GameJudge judge;
  //  vector<OthelloAI*> zoo;
  //  zoo=readAIFile(srcPath "/aiSave3.txt",8);

  //  judge.HumanVsAi(*zoo[0],8,10,Color::White);

  //  for(auto z:zoo) delete z;


  return 0;
}
