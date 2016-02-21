//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_GAMESTATE_H
#define OTHELLO_GAMESTATE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
enum Color{Black,White};

struct ChessBoardScore{
  const int N;
  std::vector<double> scores;
  ChessBoardScore(int Nsize):N(Nsize){
    scores.resize(Nsize);
  }
};

class GameState {
  const int N;
  std::vector<bool> isWhite;
  std::vector<bool> isBlack;
 public:
  GameState(int Nsize);
  void printBoard();
  void addPiece(int i,int j,Color player);//TODO:
  //TODO: gives legal moves for white player
  std::vector<std::pair<int,int> > possibleMovesWhite();
  //TODO: gives legal moves for black player
  std::vector<std::pair<int,int> > possibleMovesBlack();
  std::vector<std::pair<int,int> > position_NextTo_Piece(Color player);
    //TODO: give the candidate positons of possible moves.
};


#endif //OTHELLO_GAMESTATE_H
