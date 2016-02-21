//
// Created by FLM on 2016/2/21.
//

#include "GameJudge.h"
using namespace std;
Color GameJudge::PlayAGame_getWinner(const OthelloAI & black, const OthelloAI& white, int Nsize) {
  GameState gs(Nsize);
  Color currentPlayerColor=Color::Black;
  vector<pair<int,int> > possibleMoves;


  do{
    bool noMoveBlack = false;
    possibleMoves=gs.possibleMoves(Color::Black);
    if(!possibleMoves.empty()){
      pair<int,int> move = black.giveNextMove(gs,currentPlayerColor,possibleMoves);
      gs.addPiece(move.first,move.second,currentPlayerColor);
    }else{
      noMoveBlack=true;
    }

    possibleMoves=gs.possibleMoves(Color::White);
    if(!possibleMoves.empty()){
      pair<int,int> move = white.giveNextMove(gs,currentPlayerColor,possibleMoves);
      gs.addPiece(move.first,move.second,currentPlayerColor);
    }else if(noMoveBlack){
      break;
    }
  }while(true);

  int blackScore=gs.pieceCount(Color::Black);
  int whiteScore=gs.pieceCount(Color::White);

  if(blackScore>whiteScore)
    return Color::Black;
  else if(blackScore<whiteScore)
    return Color::White;
  else
    return Color::Neither;

}
