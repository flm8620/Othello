//
// Created by FLM on 2016/2/20.
//

#include "OthelloAI.h"
#include <cassert>
#include <algorithm>
using namespace std;
OthelloAI::OthelloAI(int Nsize):chessBoardScore(Nsize), N(Nsize)
{
  this->ID = generateID();
  chessBoardScore.randomizeScore();
  this->lambdas.resize(N*N);
  for(int i=4;i<N*N;i++){
    lambdas[i]=fmax(N*N*0.8-i,0)/(N*N*0.8-4);//lambdas is a decreasing function
  }
}

OthelloAI::OthelloAI(const OthelloAI &other):chessBoardScore(other.chessBoardScore), N(other.N)
{
  this->ID = generateID();
  lambdas=other.lambdas;
}

OthelloAI &OthelloAI::operator=(const OthelloAI &other)
{
  chessBoardScore=other.chessBoardScore;
  ID=this->getID();
  lambdas=other.lambdas;
}

int OthelloAI::generateID()
{
  static int staticID = -1;
  staticID++;
  return staticID;
}

double OthelloAI::max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor, double alpha, double beta,int& iter)const
{
  iter++;
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
      bool isMyTurnAgain = gsBranch.nextPlayer() == myColor;
      a=fmax(a,max_min(gsBranch,depth-1,isMyTurnAgain,myColor,alpha,beta,iter));
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
      bool isMyTurnNow = gsBranch.nextPlayer() == myColor;
      a=fmin(a,max_min(gsBranch,depth-1,isMyTurnNow,myColor,alpha,beta,iter));
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
  int step=whiteCount+blackCount;//current step count
  double positionScore = gs.evaluateBoardScore(this->chessBoardScore,myColor);
  double pieceCountScore = whiteCount-blackCount;
  if(myColor==Color::Black) pieceCountScore=-pieceCountScore;
  //lambda is a function of time(step)
  double lambda = lambdas[step];
  if(gs.gameIsEnd()) lambda=0;
  //At the end of the game, lambda should be small
  return pieceCountScore*100.0+lambda*positionScore;
}

std::pair<int, int> OthelloAI::giveNextMove(const GameState &gs, Color myColor, int &iteration) const
{
  const int maxDepth=4;
  assert(gs.nextPlayer()==myColor);
  double maxScore = -1e10;
  pair<int,int> bestMove = make_pair(-1,-1);
  auto moves = gs.getPossibleMovesForNextPlayer();
  //start minimax algo
  for(auto &move : moves){
    GameState gsBranch=gs;
    gsBranch.addPiece(move.first,move.second,myColor);
    bool isMyTurnAgain = gsBranch.nextPlayer() == myColor;
    //alpha=-infinite
    //beta=+infinite
    double s = max_min(gsBranch,maxDepth-1,isMyTurnAgain,myColor,-1e10,1e10,iteration);
    if(s>maxScore){
      bestMove=move;
      maxScore=s;
    }
  }
  assert(bestMove!=make_pair(-1,-1));
  return bestMove;
}


