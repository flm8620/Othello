#include <vector>
#include "bandit/bandit.h"
#include "GameState.h"
#include "OthelloAI.h"
#include "GameJudge.h"
#include "genetichost.h"
using namespace std;
using namespace bandit;
const int N = 8;
go_bandit([](){
  describe("GameState Test", [](){
    GameState gs(N);
    before_each([&](){
      gs.restartGame();
    });
    it("first step", [&](){
      //possible moves:
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---*----
      //  3 --*WB---
      //  4 ---BW*--
      //  5 ----*---
      //  6 --------
      //  7 --------
      auto moves=gs.getPossibleMovesForNextPlayer();
      vector<pair<int,int> > pos = {{2,3},{3,2},{4,5},{5,4}};
      for(auto p : pos){
        auto it = moves.find(p);
        AssertThat(it, !Equals(moves.end()));
      }
    });
    it("make one move", [&](){
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---b----
      //  3 ---BB---
      //  4 ---BW---
      //  5 --------
      //  6 --------
      //  7 --------
      gs.addPiece(2,3,Color::Black);
      vector<bool> blackPos(N*N,false);
      vector<bool> whitePos(N*N,false);
      blackPos[2*N+3]=blackPos[3*N+3]=blackPos[4*N+3]=blackPos[3*N+4]=true;
      whitePos[4*N+4]=true;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---B----
      //  3 ---BB---
      //  4 --wWW---
      //  5 --------
      //  6 --------
      //  7 --------
      gs.addPiece(4,2,Color::White);
      blackPos[4*N+3]=false;
      whitePos[4*N+3]=whitePos[4*N+2]=true;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---B----
      //  3 ---BB---
      //  4 --WBW---
      //  5 --b-----
      //  6 --------
      //  7 --------
      gs.addPiece(5,2,Color::Black);
      //gs.printBoard();
      blackPos[4*N+3]=blackPos[5*N+2]=true;
      whitePos[4*N+3]=false;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---Bw---
      //  3 ---WW---
      //  4 --WBW---
      //  5 --B-----
      //  6 --------
      //  7 --------
      gs.addPiece(2,4,Color::White);
      //gs.printBoard();
      blackPos[3*N+4]=blackPos[3*N+3]=false;
      whitePos[3*N+4]=whitePos[2*N+4]=whitePos[3*N+3]=true;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---BW---
      //  3 ---WB---
      //  4 --WBBb--
      //  5 --B-----
      //  6 --------
      //  7 --------
      gs.addPiece(4,5,Color::Black);
      //gs.printBoard();
      blackPos[4*N+5]=blackPos[4*N+4]=blackPos[3*N+4]=true;
      whitePos[4*N+4]=whitePos[3*N+4]=false;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
    });
    it("special cases", [&](){
      //    01234567
      //  0 --------
      //  1 WWWWWWWW
      //  2 WWWWWWWW
      //  3 WWWWWWWW
      //  4 WWWWWWWW
      //  5 WWWWWWWW
      //  6 WWWWWWWW
      //  7 BBBBBBBB
      vector<bool> blackPos(N*N,false);
      vector<bool> whitePos(N*N,true);
      for(int i=0;i<N;i++) whitePos[i]=false;
      for(int i=7*N+0;i<N*N;i++){ blackPos[i]=true; whitePos[i]=false;}
      gs.setColorPositionPlayer(blackPos,whitePos,Color::Black);
      gs.addPiece(0,0,Color::Black);
      //    01234567
      //  0 b-------
      //  1 BBWWWWWW
      //  2 BWBWWWWW
      //  3 BWWBWWWW
      //  4 BWWWBWWW
      //  5 BWWWWBWW
      //  6 BWWWWWBW
      //  7 BBBBBBBB
      for(int i=0;i<N;i++){
        blackPos[i*N+0]=true;
        whitePos[i*N+0]=false;
      }
      for(int i=0;i<N;i++){
        blackPos[i*N+i]=true;
        whitePos[i*N+i]=false;
      }
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
    });
    it("special cases2", [&](){
      //    01234567
      //  0 --------
      //  1 --------
      //  2 --------
      //  3 ---W-B--
      //  4 --------
      //  5 --------
      //  6 --------
      //  7 --------
      vector<bool> blackPos(N*N,false);
      vector<bool> whitePos(N*N,false);
      blackPos[3*N+5]=true;
      whitePos[3*N+3]=true;
      gs.setColorPositionPlayer(blackPos,whitePos,Color::Black);
      AssertThat(gs.nextPlayer(),Equals(Color::Neither));
    });
  });
  describe("Genetic Algo Test", [](){
    GeneticHost host(8);
    it("mutation",[&](){
      for(int i=0;i<10;i++){
        OthelloAI ai(8);
        for(int j=0;j<10;j++){
          ai=host.mutation(ai);
        }
      }
    });
    it("crossover",[&](){
      for(int i=0;i<10;i++){
        OthelloAI ai1(8),ai2(8);
        for(int j=0;j<10;j++){
          ai1=host.crossover(ai1,ai2);
        }
      }
    });

  });
});

int main(int argc, char* argv[]){
  return bandit::run(argc, argv);
}
