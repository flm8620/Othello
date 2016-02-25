//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_OTHELLOAI_H
#define OTHELLO_OTHELLOAI_H
#include "GameState.h"
#include <vector>
#include <chrono>
class ExceptionTimeUp{

};

class OthelloAI {
  //the score for position, which determines the quality of this AI
  ChessBoardScore chessBoardScore;
  //lambda is the ratio between position score and piece number score
  std::vector<double> lambdas;
  const int N;
  int ID;
  mutable std::chrono::high_resolution_clock::time_point startTime;
  mutable double thinkTime;

  int generateID();
  //recursive minimax with alpha-beta
  double max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor, double alpha, double beta, int &iter)const;
  //evaluate the score of current GameState for player
  double evaluateScore(const GameState &gs, Color myColor)const;
  std::pair<int, int> startMaxMin(const GameState &gs, Color myColor, int &iteration, int maxDepth) const;
 public:
  //let AI to decide next move
  std::pair<int,int> giveNextMove(const GameState &gs, Color myColor, int &iteration, int &reachedDepth, double thinkTime=1.0) const;

  OthelloAI(int Nsize);
  OthelloAI(const OthelloAI& other);
  OthelloAI& operator=(const OthelloAI& other);

  void useRecommandedChessBoardScore();

  //used for genetic modification
  ChessBoardScore getChessBoardScore()const{return chessBoardScore;}
  void setChessBoardScore(const ChessBoardScore& cbs){this->chessBoardScore.scores=cbs.scores;}
  std::vector<double> getLambdas()const{return lambdas;}
  void setLambdas(std::vector<double>& newLambdas){this->lambdas=newLambdas;}
  int getID()const{return ID;}
  void setID(int id){ID=id;}

};


#endif //OTHELLO_OTHELLOAI_H
