//
// Created by FLM on 2016/2/21.
//

#ifndef OTHELLO_GAMEJUDGE_H
#define OTHELLO_GAMEJUDGE_H
#include "GameState.h"
#include "OthelloAI.h"

class GameJudge {
 public:
  int PlayAGame_getScore(const OthelloAI & black, const OthelloAI& white, int Nsize,double roundTime, bool showGame=false);
};


#endif //OTHELLO_GAMEJUDGE_H
