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
    return evaluateScore(gs,myColor);
  }
  if(gs.gameIsEnd()){
    return
  }
}

double OthelloAI::evaluateScore(const GameState &gs, Color myColor)
{
  int whiteCount=gs.pieceCount(Color::White);
  int blackCount=gs.pieceCount(Color::Black);
  int step=whiteCount+blackCount;
  double positionScore = gs.evaluateBoardScore(this->chessBoardScore,myColor);
  double pieceCountScore = whiteCount-blackCount;
  if(myColor==Color::Black) pieceCountScore=-pieceCountScore;
  double lambda = lambdas[whiteCount+blackCount];
  if(gs.gameIsEnd()) lambda=0;
  return pieceCountScore*100.0+lambda*positionScore;
}

std::pair<int, int> OthelloAI::giveNextMove(const GameState &gs, Color myColor,
                                            const std::vector<std::pair<int, int> > &possibleMoves) const
{
  const int maxDepth=5;
}


