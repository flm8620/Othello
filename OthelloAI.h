//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_OTHELLOAI_H
#define OTHELLO_OTHELLOAI_H
#include "GameState.h"
#include <vector>
class OthelloAI {
  ChessBoardScore chessBoardScore;
  std::vector<double> lambdas;
  const int N;
  double max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor,double alpha,double beta)const;
  double evaluateScore(const GameState &gs, Color myColor)const;
 public:
  std::pair<int,int> giveNextMove(const GameState &gs,Color myColor,
                                  const std::vector<std::pair<int,int> >& possibleMoves) const;
  OthelloAI(int Nsize);


};


#endif //OTHELLO_OTHELLOAI_H
