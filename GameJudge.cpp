//
// Created by FLM on 2016/2/21.
//

#include "GameJudge.h"
#include <iostream>
using namespace std;
int GameJudge::PlayAGame_getScore(const OthelloAI &black,
                                  const OthelloAI &white,
                                  int Nsize,
                                  double roundTime,
                                  bool showGame) {
  GameState gs(Nsize);
  int count = 1;
  do {
    const OthelloAI &nextPlayer = gs.nextPlayer() == Color::Black ? black : white;
    int thinkStepsCount = 0;
    int reachedDepth = -1;
    pair<int, int> move = nextPlayer.giveNextMove(gs, gs.nextPlayer(), thinkStepsCount, reachedDepth, roundTime);
    gs.addPiece(move.first, move.second, gs.nextPlayer());
    if (showGame) {
      cout << " Round " << count << " " << thinkStepsCount << " steps of think, depth: " << reachedDepth << endl;
      gs.printBoard();
      //cout<<endl;
    }
    count++;
  } while (!gs.gameIsEnd());

  int blackScore = gs.pieceCount(Color::Black);
  int whiteScore = gs.pieceCount(Color::White);
  return blackScore - whiteScore;
}
