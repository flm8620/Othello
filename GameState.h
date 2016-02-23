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
#include <map>
enum Color{Black,White,Neither};
enum Direction{TopLeft,TopRight,BottomLeft,BottomRight,Left,Right,Top,Bottom,Center};
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
  // The following two values are updated when constructed and when addPiece()
  std::set<std::pair<int,int> > nextPossibleMoves;
  std::map<std::pair<int,int>, std::vector<Direction> > moveWithDirection;

  //get relative position of neighbours (case of corner considered)
  std::vector<std::pair<int,int> >& getNeighbourOffset(int i,int j) const;

  //for example: getDirectionOffset(Left) = pair{0,-1}
  std::pair<int,int> getDirectionOffset(Direction direction)const;




  //give the candidate positons of possible moves.
  std::vector<std::pair<int,int> > position_NextTo_Piece(Color player)const;

  //update this->nextPossibleMoves and this->moveWithDirection
  void updatePossibleMoves(Color player);

 public:
  GameState(int Nsize);
  //get piece count of player
  int pieceCount(Color player)const;
  bool gameIsEnd()const{return nextMoveColor==Color::Neither;}
  Color nextPlayer()const{return nextMoveColor;}
  void printBoard() const;
  void addPiece(int i,int j,Color player);
  std::set<std::pair<int,int> > getPossibleMovesForNextPlayer()const{return nextPossibleMoves;}
  double evaluateBoardScore(const ChessBoardScore& score, Color player)const;
};


#endif //OTHELLO_GAMESTATE_H
