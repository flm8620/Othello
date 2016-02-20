//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_GAMESTATE_H
#define OTHELLO_GAMESTATE_H
#include <iostream>
#include <vector>
#include <algorithm>

class GameState {
  const int N=8;
  std::vector<bool> isWhile;
  std::vector<bool> isBlack;
 public:
  GameState();
  void printBoard();
  void addWhite(int i,int j);//TODO:
  void addBlack(int i,int j);//TODO:
  //TODO: gives legal moves for white player
  std::vector<std::pair<int,int> > possibleMovesWhite();
  //TODO: gives legal moves for black player
  std::vector<std::pair<int,int> > possibleMovesBlack();

};


#endif //OTHELLO_GAMESTATE_H
