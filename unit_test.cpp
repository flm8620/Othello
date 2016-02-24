#include <vector>
#include "bandit/bandit.h"
#include "GameState.h"
#include "OthelloAI.h"
#include "GameJudge.h"
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
      //  2 ---b----
      //  3 ---BB---
      //  4 --wWW---
      //  5 --------
      //  6 --------
      //  7 --------
      gs.addPiece(4,2,Color::White);
      blackPos[4*N+3]=false;
      whitePos[4*N+3]=true;
      whitePos[4*N+2]=true;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
    });
  });
});

int main(int argc, char* argv[]){
  return bandit::run(argc, argv);
}
