//
// Created by FLM on 2016/2/21.
//

#include "GameJudge.h"
#include <iostream>
#include <string>
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

int GameJudge::HumanVsAi(const OthelloAI &ai, int Nsize, double roundTime, Color humanColor)
{
  GameState gs(Nsize);
  gs.printBoard();
  int count = 1;
  do {
    if(gs.nextPlayer()==humanColor){
      do{
        cout<<"Please type in a move(such as 'a3'):";
        string s;
        cin>>s;
        if(s.length()==2){
          int j=s[0]-'a';
          int i=s[1]-'0';
          if(i>=0&&i<Nsize&&j>=0&&j<Nsize){
            try {
              gs.addPiece(i, j, gs.nextPlayer());
              break;
            } catch (std::exception& e) {
              cout<<"error: "<<e.what()<<endl;
            }
          }
        }
        cout<<"Input is invalid!"<<endl;
      }while(true);
    }else{
      cout << "Computer thinking ... "<<endl;
      int thinkStepsCount = 0;
      int reachedDepth = -1;
      pair<int, int> move = ai.giveNextMove(gs, gs.nextPlayer(), thinkStepsCount, reachedDepth, roundTime);
      gs.addPiece(move.first, move.second, gs.nextPlayer());
      cout << thinkStepsCount << " steps of think, reached depth: " << reachedDepth << endl;
    }

    gs.printBoard();
    cout<<endl;
    count++;
  } while (!gs.gameIsEnd());


  int blackScore = gs.pieceCount(Color::Black);
  int whiteScore = gs.pieceCount(Color::White);
  int humanScore = humanColor == Color::Black ? blackScore : whiteScore;
  int aiScore = humanColor == Color::Black ? whiteScore : blackScore;
  if(humanScore>aiScore){
    cout<<"You WIN!"<<endl;
  }else if(humanScore<aiScore){
    cout<<"Computer WIN."<<endl;
  }else{
    cout<<"DRAW!"<<endl;
  }
  cout<<"human pieces: "<<humanScore<<endl;
  cout<<"AI pieces: "<<aiScore<<endl;
  return blackScore - whiteScore;
}
