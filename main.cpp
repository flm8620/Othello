#include <iostream>
#include "GameState.h"
#include "GameJudge.h"
#include "OthelloAI.h"
using namespace std;
int main() {
  OthelloAI AI1(8);
  OthelloAI AI2(8);
  GameJudge judge;
  judge.PlayAGame_getWinner(AI1,AI2,8,true);
  return 0;
}
