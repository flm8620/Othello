//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_OTHELLOAI_H
#define OTHELLO_OTHELLOAI_H
#include "GameState.h"
#include <vector>
class OthelloAI {
  //the score for position, which determines the quality of this AI
  ChessBoardScore chessBoardScore;
  //lambda is the ratio between position score and piece number score
  std::vector<double> lambdas;
  const int N;
  int ID;
  int generateID();
  //recursive minimax with alpha-beta
  double max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor, double alpha, double beta, int &iter)const;
  //evaluate the score of current GameState for player
  double evaluateScore(const GameState &gs, Color myColor)const;
 public:
  //let AI to decide next move
  std::pair<int,int> giveNextMove(const GameState &gs, Color myColor, int &iteration) const;
  OthelloAI(int Nsize);
  OthelloAI(const OthelloAI& other);

  //used for genetic modification
  ChessBoardScore getChessBoardScore()const{return chessBoardScore;}
  void setChessBoardScore(const ChessBoardScore& cbs){this->chessBoardScore.scores=cbs.scores;}
  std::vector<double> getLambdas()const{return lambdas;}
  void setLambdas(std::vector<double>& newLambdas){this->lambdas=newLambdas;}
  int getID()const{return ID;}
  void modifyChessBoardScore(int i,int j,double value){
    chessBoardScore.scores[i*N+j]=value;
    chessBoardScore.scores[N*N-i*N-j-1]=value;
  }
  double getChessBoardScoreValue(int i,int j)const{
    return chessBoardScore.scores[i*N+j];
  }

};


#endif //OTHELLO_OTHELLOAI_H
