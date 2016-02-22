//
// Created by FLM on 2016/2/20.
//

#include "OthelloAI.h"

OthelloAI::OthelloAI(int Nsize):chessBoardScore(Nsize), N(Nsize)
{
  chessBoardScore.randomizeScore();
  lambdas.resize(N*N);
  for(int i=5;i<N*N;i++){
    lambdas[i]=fabs(N*N*0.8-i)/(N*N*0.8-5);
  }
}

double OthelloAI::max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor)
{
  if(depth==0){

  }
}

double OthelloAI::evaluateScore(const GameState &gs, Color myColor)
{
  return gs.evaluateBoardScore(this->chessBoardScore,myColor);
}

double OthelloAI::lambda(int steps)
{
  // 4<steps<=64

}

std::pair<int, int> OthelloAI::giveNextMove(const GameState &gs, Color myColor,
                                            const std::vector<std::pair<int, int> > &possibleMoves) const
{
  const int maxDepth=5;
}


