//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_OTHELLOAI_H
#define OTHELLO_OTHELLOAI_H
#include "GameState.h"

class OthelloAI {
  ChessBoardScore chessBoardScore;
 public:
  std::pair<int,int> giveNextMove(const GameState &gs,Color currentMoveColor,
                                  const std::vector<std::pair<int,int> >& possibleMoves) const;


};


#endif //OTHELLO_OTHELLOAI_H
