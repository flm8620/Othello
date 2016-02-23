//
// Created by FLM on 2016/2/21.
//

#include "GameJudge.h"
using namespace std;
Color GameJudge::PlayAGame_getWinner(const OthelloAI & black, const OthelloAI& white, int Nsize) {
  GameState gs(Nsize);
  vector<pair<int,int> > possibleMoves;


  do{
    const OthelloAI& nextPlayer = gs.nextPlayer()== Color::Black ? black : white;
    pair<int,int> move = nextPlayer.giveNextMove(gs,Color::Black);
    gs.addPiece(move.first,move.second,Color::Black);
  }while(!gs.gameIsEnd());

  int blackScore=gs.pieceCount(Color::Black);
  int whiteScore=gs.pieceCount(Color::White);

  if(blackScore>whiteScore)
    return Color::Black;
  else if(blackScore<whiteScore)
    return Color::White;
  else
    return Color::Neither;
}
