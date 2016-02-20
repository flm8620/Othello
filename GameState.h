//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_GAMESTATE_H
#define OTHELLO_GAMESTATE_H
#include <iostream>
#include <vector>
#include <algorithm>



struct ChessBoardScore{
  const int N;
  std::vector<double> scores;
  ChessBoardScore(int Nsize):N(Nsize){
    scores.resize(Nsize);
  }
};

class GameState {
  const int N;
  std::vector<bool> isWhile;
  std::vector<bool> isBlack;
 public:
  GameState(int Nsize);
  void printBoard();
  void addWhite(int i,int j);//TODO:
  void addBlack(int i,int j);//TODO:
  //TODO: gives legal moves for white player
  std::vector<std::pair<int,int> > possibleMovesWhite();
  //TODO: gives legal moves for black player
  std::vector<std::pair<int,int> > possibleMovesBlack();

};


#endif //OTHELLO_GAMESTATE_H
