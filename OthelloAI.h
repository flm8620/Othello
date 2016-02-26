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
  DiskSquare diskSquare;
  const int N;
  int ID;
  struct ScoreWeight{
    double middleTime, endTime;
    double mobility_begin;
    double mobility_middle;
    double position_begin;
    double position_middle;
    //piecenumber_begin = 1-mobility_begin-position_begin
    //piecenumber_middle = 1-mobility_middle-position_middle
    //piecenumber_end = 1;
    void getThreeWeights(double& mobility,double& position,double& piecenumber,double t)const;
    void randomnize();
    void mutateMyself();
    ScoreWeight crossover(ScoreWeight other)const;
  } scoreWeight;

  mutable std::chrono::high_resolution_clock::time_point startTime;
  mutable double thinkTime;


  int generateID();
  //recursive minimax with alpha-beta
  double max_min(const GameState &gs, int depth, bool isMyTurn, Color myColor, double alpha, double beta, int &iter, bool limitTime=false)const;
  //evaluate the score of current GameState for player
  double evaluateScore(const GameState &gs, Color myColor)const;
  std::pair<int, int> startMaxMin(const GameState &gs, Color myColor, int &iteration, int maxDepth, bool limitTime=false) const;
 public:
  //let AI to decide next move
  std::pair<int,int> giveNextMove(const GameState &gs, Color myColor, int &iteration, int &reachedDepth, double thinkTime=1.0, int thinkDepth=0) const;

  OthelloAI(int Nsize);
  OthelloAI(const OthelloAI& other);
  OthelloAI& operator=(const OthelloAI& other);

  void useRecommandedDiskSquare();

  //used for genetic modification
  DiskSquare getDiskSquare()const{return diskSquare;}
  void setDiskSquare(const DiskSquare &ds){ this->diskSquare.scores=ds.scores;}
  ScoreWeight getScoreWeight()const{return scoreWeight;}
  void setScoreWeight(ScoreWeight sw){scoreWeight=sw;}
  OthelloAI *createNewAIByMutation()const;
  OthelloAI *createNewAIByCrossOver(const OthelloAI* other)const;
  int getID()const{return ID;}
  void setID(int id){ID=id;}

};


#endif //OTHELLO_OTHELLOAI_H
