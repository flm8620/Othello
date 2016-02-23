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
    
  std::set<std::pair<std::pair<int,int>,std::vector<bool> > > nextPossibleMoves;
 public:
    std::vector<bool> isColor_I(Color player) const{  // if color = white, return the positon of black
        return player == Color::White ? isBlack : isWhite;
    }
    std::vector<bool> isColor(Color player) const{  // if color = white, return the positon of white
        return player == Color::Black ? isBlack : isWhite;
    }
  GameState(int Nsize);
  bool gameIsEnd()const{return nextMoveColor==Color::Neither;}
  Color nextPlayer()const{return nextMoveColor;}
  void printBoard() const;
  void addPiece(int i,int j,Color player);//TODO:
  //TODO: gives legal moves for player
  std::set<std::pair< std::pair<int,int>,std::vector<bool> > > possibleMoves (Color player)const;
    //in which directions we change the piece:
    //   6  7  0
    //   5  .  1
    //   4  3  2
    
  //TODO: get piece number of player
  int pieceCount(Color player)const;
  std::vector<std::pair<int,int> > position_NextTo_Piece(Color player)const;
    //TODO: give the candidate positons of possible moves.

  double evaluateBoardScore(const ChessBoardScore& score, Color player)const;
};


#endif //OTHELLO_GAMESTATE_H
