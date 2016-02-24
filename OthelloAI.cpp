//
// Created by FLM on 2016/2/20.
//

#include "OthelloAI.h"
#include <cassert>
#include <algorithm>
using namespace std;
OthelloAI::OthelloAI(int Nsize):chessBoardScore(Nsize), N(Nsize)
{
  chessBoardScore.randomizeScore();
  this->lambdas.resize(N*N);
  for(int i=5;i<N*N;i++){
    lambdas[i]=fabs(N*N*0.8-i)/(N*N*0.8-5);
  }
}

double OthelloAI::max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor, double alpha, double beta)const
{
  if(depth==0 || gs.gameIsEnd()){
    return evaluateScore(gs,myColor);
  }
  if(isMyTurn){
    double a = -1e10;
    assert(gs.nextPlayer()==myColor);
    auto moves = gs.getPossibleMovesForNextPlayer();
    for(auto &move : moves){
      GameState gsBranch=gs;
      gsBranch.addPiece(move.first,move.second,myColor);
      bool isMyTurnAgain = gs.nextPlayer() == myColor;
      a=fmax(a,max_min(gsBranch,depth-1,isMyTurnAgain,myColor,alpha,beta));
      alpha = fmax(alpha,a);
      if(beta <= alpha)break;//beta cut off
    }
    return a;
  }else{
    double a = 1e10;
    Color otherPlayer=gs.nextPlayer();
    assert(gs.nextPlayer()!=myColor);
    auto moves = gs.getPossibleMovesForNextPlayer();
    for(auto &move : moves){
      GameState gsBranch=gs;
      gsBranch.addPiece(move.first,move.second,otherPlayer);
      bool isMyTurnNow = gs.nextPlayer() == myColor;
      a=fmin(a,max_min(gsBranch,depth-1,isMyTurnNow,myColor,alpha,beta));
      beta = min(beta,a);
      if(beta <= alpha) break; //alpha cut off
    }
    return a;
  }
}

double OthelloAI::evaluateScore(const GameState &gs, Color myColor)const
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

std::pair<int, int> OthelloAI::giveNextMove(const GameState &gs, Color myColor) const
{
  const int maxDepth=5;
  assert(gs.nextPlayer()==myColor);
  double maxScore = -1e10;
  pair<int,int> bestMove = make_pair(-1,-1);
  auto moves = gs.getPossibleMovesForNextPlayer();
  for(auto &move : moves){
    GameState gsBranch=gs;
    gsBranch.addPiece(move.first,move.second,myColor);
    bool isMyTurnAgain = gs.nextPlayer() == myColor;
    double s = max_min(gsBranch,maxDepth-1,isMyTurnAgain,myColor,-1e10,1e10);
    if(s>maxScore){
      bestMove=move;
      maxScore=s;
    }
  }
  assert(bestMove!=make_pair(-1,-1));
  return bestMove;
}


