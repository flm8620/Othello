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
#include <bitset>
enum Color{Black,White,Neither};
enum Direction{TopLeft,TopRight,BottomLeft,BottomRight,Left,Right,Top,Bottom,Center};
struct DiskSquare{
  static const double MAXSCORE;
  const int N;
  std::vector<double> scores;
  DiskSquare(int Nsize);
  DiskSquare(const DiskSquare& other);
  DiskSquare& operator=(const DiskSquare& other);
  static std::vector<double> triangleFormatToFullFormat(std::vector<double> triangle, int N);
  std::vector<double> getTriangleFormat() const;
  void printOut();
  void randomizeScore();
};

class GameState {
  const int N;
  std::vector<bool> isWhite;
  std::vector<bool> isBlack;
  std::pair<int,int> lastPosition;
  Color nextMoveColor;
  //neighbourDirection:
  //  123
  //  8*4
  //  765
  // direction in which we have a piece as neighbour
  std::vector<std::bitset<8>> neighbourDirection;
  std::set<std::pair<int,int>> emptyAdjacent;

  // The following two values are updated when constructed and when addPiece()
  std::set<std::pair<int,int> > nextPossibleMoves;
  std::map<std::pair<int,int>, std::vector<Direction> > moveWithDirection;

  //get relative position of neighbours (case of corner considered)
  const std::vector<std::pair<int,int> >& getNeighbourOffset(int i,int j) const;

  //for example: getDirectionOffset(Left) = pair{0,-1}
  std::pair<int,int> getDirectionOffset(Direction direction)const;

  //give the candidate positons of possible moves.
  std::vector<std::pair<std::pair<int,int>,std::vector<Direction>>> position_NextTo_Piece(Color adversary)const;

  //update this->nextPossibleMoves and this->moveWithDirection
  void updatePossibleMoves(Color player);
  void recalculateNeighbourDirection_emptyAdjacent();
 public:
  GameState(int Nsize);
  void restartGame();
  int pieceCount(Color player)const;
  bool gameIsEnd()const{return nextMoveColor==Color::Neither;}
  Color nextPlayer()const{return nextMoveColor;}
  void printBoard() const;
  void addPiece(int i,int j,Color player);
  const std::set<std::pair<int,int> >& getPossibleMovesForNextPlayer()const{return nextPossibleMoves;}
  double evaluateBoardScore(const DiskSquare& score, Color player)const;

  //for test only:
  std::vector<bool> getWhitePosition()const{return isWhite;}
  std::vector<bool> getBlackPosition()const{return isBlack;}
  void setColorPositionPlayer(std::vector<bool> black,std::vector<bool>white,Color nextPlayer);
};


#endif //OTHELLO_GAMESTATE_H
