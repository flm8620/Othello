//
// Created by FLM on 2016/2/20.
//

#ifndef OTHELLO_GAMESTATE_H
#define OTHELLO_GAMESTATE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <set>
enum Color{Black,White,Neither};

struct ChessBoardScore{
  const int N;
  std::vector<double> scores;
  ChessBoardScore(int Nsize):N(Nsize){
    scores.resize(Nsize,0);
  }
  void randomizeScore();
};

class GameState {
  const int N;
  std::vector<bool> isWhite;
  std::vector<bool> isBlack;
  Color nextMoveColor;
  std::vector<std::pair<int,int> > nextPossibleMoves;
 public:
    std::vector<bool> isColor(Color player) const{
        return player == Color::Black ? isBlack : isWhite;
    }
  GameState(int Nsize);
  bool gameIsEnd()const{return nextMoveColor==Color::Neither;}
  Color nextPlayer()const{return nextMoveColor;}
  void printBoard() const;
  void addPiece(int i,int j,Color player);//TODO:
  //TODO: gives legal moves for player
  std::vector<std::pair<int,int> > possibleMoves(Color player)const;
  //TODO: get piece number of player
  int pieceCount(Color player)const;
  std::vector<std::pair<int,int> > position_NextTo_Piece(Color player)const;
    //TODO: give the candidate positons of possible moves.

  double evaluateBoardScore(const ChessBoardScore& score, Color player)const;
};


#endif //OTHELLO_GAMESTATE_H
