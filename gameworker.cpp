#include "gameworker.h"
#include <stdexcept>
using namespace std;
void GameWorker::letAIplay()
{
  while(true){
    Color next = gs.nextPlayer();
    if(next==Color::Neither)
      throw logic_error("Game Ended!");
    if(next==this->humanColor)
      throw logic_error("It's not AI's turn!");
    int reachedDepth;
    int iteration;
    std::pair<int,int> move = this->ai.giveNextMove(gs,this->aiColor,iteration,reachedDepth,this->thinkTime);
    gs.addPiece(move.first,move.second,aiColor);
    emit AIplayed(move.first,move.second,this->gameSeriesID);
    if(gs.nextPlayer()!=aiColor)
      return;
  }
}

GameWorker::GameWorker(int Nsize, OthelloAI ai, QObject *parent) : QObject(parent), ai(ai), gs(Nsize)
{

}

void GameWorker::startNewGame(Color humanColor, double thinkTime, int gameSeriesID)
{
  this->gameSeriesID=gameSeriesID;
  this->humanColor=humanColor;
  this->aiColor = humanColor == Color::White ? Color::Black : Color::White;
  this->gs.restartGame();
  this->thinkTime=thinkTime;
  if(this->aiColor == Color::Black)
    letAIplay();
}

void GameWorker::humanPlayed(int i, int j, int gameSeriesID)
{
  if(gameSeriesID!=this->gameSeriesID)return;
  Color next = gs.nextPlayer();
  if(next==Color::Neither)
    throw logic_error("Game Ended!");
  if(next!=this->humanColor)
    throw logic_error("It's not human turn!");
  gs.addPiece(i,j,humanColor);
  if(gs.gameIsEnd())return;
  if(gs.nextPlayer()==aiColor)
    letAIplay();
  else
    return;
}

