//
// Created by FLM on 2016/2/21.
//

#include "GameJudge.h"
#include <iostream>
using namespace std;
int GameJudge::PlayAGame_getScore(const OthelloAI & black, const OthelloAI& white, int Nsize, bool showGame) {
  GameState gs(Nsize);
  int count = 1;
  do{

    const OthelloAI& nextPlayer = gs.nextPlayer()== Color::Black ? black : white;
    int thinkSteps=0;
    pair<int,int> move = nextPlayer.giveNextMove(gs,gs.nextPlayer(),thinkSteps);
    gs.addPiece(move.first,move.second,gs.nextPlayer());
    if(showGame){
      cout<<" Round "<<count<<" "<<thinkSteps<<" steps of think"<<endl;
      gs.printBoard();
      cout<<endl;
    }else{
      cout<<count<<".";
    }
    count++;
  }while(!gs.gameIsEnd());
  cout<<endl;

  int blackScore=gs.pieceCount(Color::Black);
  int whiteScore=gs.pieceCount(Color::White);
  return blackScore-whiteScore;
}
