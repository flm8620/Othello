#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include "bandit/bandit.h"
#include "GameState.h"
#include "OthelloAI.h"
#include "genetichost.h"
using namespace std;
using namespace bandit;
using namespace std::chrono;
const int N = 8;
auto ticTime = std::chrono::high_resolution_clock::now();
void tic() {
  ticTime = std::chrono::high_resolution_clock::now();
}
void toc(string info) {
  auto tocTime = std::chrono::high_resolution_clock::now();
  duration<double> time_span = duration_cast<duration<double>>(tocTime - ticTime);
  cout << info << " : " << time_span.count() << " seconds" << endl;
}

go_bandit([]() {
  describe("GameState Test", []() {
    GameState gs(N);
    before_each([&]() {
      gs.restartGame();
    });
    it("first step", [&]() {
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
      auto moves = gs.getPossibleMovesForNextPlayer();
      vector<pair<int, int> > pos = {{2, 3}, {3, 2}, {4, 5}, {5, 4}};
      for (auto p : pos) {
        auto it = moves.find(p);
        AssertThat(it, !Equals(moves.end()));
      }
    });
    it("make one move", [&]() {
      //    01234567
      //  0 --------
      //  1 --------
      //  2 ---b----
      //  3 ---BB---
      //  4 ---BW---
      //  5 --------
      //  6 --------
      //  7 --------
      gs.addPiece(2, 3, Color::Black);
      vector<bool> blackPos(N * N, false);
      vector<bool> whitePos(N * N, false);
      blackPos[2 * N + 3] = blackPos[3 * N + 3] = blackPos[4 * N + 3] = blackPos[3 * N + 4] = true;
      whitePos[4 * N + 4] = true;
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
      gs.addPiece(4, 2, Color::White);
      blackPos[4 * N + 3] = false;
      whitePos[4 * N + 3] = whitePos[4 * N + 2] = true;
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
      gs.addPiece(5, 2, Color::Black);
      //gs.printBoard();
      blackPos[4 * N + 3] = blackPos[5 * N + 2] = true;
      whitePos[4 * N + 3] = false;
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
      gs.addPiece(2, 4, Color::White);
      //gs.printBoard();
      blackPos[3 * N + 4] = blackPos[3 * N + 3] = false;
      whitePos[3 * N + 4] = whitePos[2 * N + 4] = whitePos[3 * N + 3] = true;
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
      gs.addPiece(4, 5, Color::Black);
      //gs.printBoard();
      blackPos[4 * N + 5] = blackPos[4 * N + 4] = blackPos[3 * N + 4] = true;
      whitePos[4 * N + 4] = whitePos[3 * N + 4] = false;
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
    });
    it("special cases", [&]() {
      //    01234567
      //  0 --------
      //  1 WWWWWWWW
      //  2 WWWWWWWW
      //  3 WWWWWWWW
      //  4 WWWWWWWW
      //  5 WWWWWWWW
      //  6 WWWWWWWW
      //  7 BBBBBBBB
      vector<bool> blackPos(N * N, false);
      vector<bool> whitePos(N * N, true);
      for (int i = 0; i < N; i++) whitePos[i] = false;
      for (int i = 7 * N + 0; i < N * N; i++) {
        blackPos[i] = true;
        whitePos[i] = false;
      }
      gs.setColorPositionPlayer(blackPos, whitePos, Color::Black);
      gs.addPiece(0, 0, Color::Black);
      //    01234567
      //  0 b-------
      //  1 BBWWWWWW
      //  2 BWBWWWWW
      //  3 BWWBWWWW
      //  4 BWWWBWWW
      //  5 BWWWWBWW
      //  6 BWWWWWBW
      //  7 BBBBBBBB
      for (int i = 0; i < N; i++) {
        blackPos[i * N + 0] = true;
        whitePos[i * N + 0] = false;
      }
      for (int i = 0; i < N; i++) {
        blackPos[i * N + i] = true;
        whitePos[i * N + i] = false;
      }
      AssertThat(blackPos, Equals(gs.getBlackPosition()));
      AssertThat(whitePos, Equals(gs.getWhitePosition()));
    });
    it("special cases2", [&]() {
      //    01234567
      //  0 --------
      //  1 --------
      //  2 --------
      //  3 ---W-B--
      //  4 --------
      //  5 --------
      //  6 --------
      //  7 --------
      vector<bool> blackPos(N * N, false);
      vector<bool> whitePos(N * N, false);
      blackPos[3 * N + 5] = true;
      whitePos[3 * N + 3] = true;
      gs.setColorPositionPlayer(blackPos, whitePos, Color::Black);
      AssertThat(gs.nextPlayer(), Equals(Color::Neither));
    });
  });
  describe("Genetic Algo Test", []() {
    GeneticHost host(8);
    it("mutation", [&]() {
      for (int i = 0; i < 10; i++) {
        auto ai = new OthelloAI(8);
        for (int j = 0; j < 10; j++) {
          auto newAI = host.mutation(ai);
          delete ai;
          ai = newAI;
        }
        delete ai;
      }
    });
    it("crossover", [&]() {
      for (int i = 0; i < 10; i++) {
        auto ai1 = new OthelloAI(8);
        auto ai2 = new OthelloAI(8);
        for (int j = 0; j < 10; j++) {
          auto newAI = host.crossover(ai1, ai2);
          delete ai1;
          ai1 = newAI;
        }
        delete ai1;
        delete ai2;
      }
    });

  });
  describe("ChessBoardScore Test", []() {
    vector<double> triangle = {1, 2, 3, 4,
                               5, 6, 7,
                               8, 9,
                               0};
    vector<double> full = {1, 2, 3, 4, 4, 3, 2, 1,
                           2, 5, 6, 7, 7, 6, 5, 2,
                           3, 6, 8, 9, 9, 8, 6, 3,
                           4, 7, 9, 0, 0, 9, 7, 4,
                           4, 7, 9, 0, 0, 9, 7, 4,
                           3, 6, 8, 9, 9, 8, 6, 3,
                           2, 5, 6, 7, 7, 6, 5, 2,
                           1, 2, 3, 4, 4, 3, 2, 1};
    it("triangle", [&]() {
      AssertThat(full, Equals(DiskSquare::triangleFormatToFullFormat(triangle, 8)));
      DiskSquare ds(8);
      ds.scores = full;
      AssertThat(triangle, Equals(ds.getTriangleFormat()));
    });

  });
  describe("Running time", []() {
    GameState gs(8);
    OthelloAI ai(8);
    ai.useRecommandedChessBoardScore();
    int iter, reachedDepth, iterTotal = 0;
    tic();
    auto move = ai.giveNextMove(gs, gs.nextPlayer(), iter, reachedDepth, 0.0, 9);
    toc("9 depth time");
    // special case
    //    01234567
    //  0 --------
    //  1 WWWWWWWW
    //  2 WWWWWWWW
    //  3 WWWWWWWW
    //  4 WWWWWWWW
    //  5 WWWWWWWW
    //  6 WWWWWWWW
    //  7 BBBBBBBB
    vector<bool> blackPos(N * N, false);
    vector<bool> whitePos(N * N, true);
    for (int i = 0; i < N; i++) whitePos[i] = false;
    for (int i = 7 * N + 0; i < N * N; i++) {
      blackPos[i] = true;
      whitePos[i] = false;
    }
    gs.setColorPositionPlayer(blackPos, whitePos, Color::Black);



    cout << iter << endl;
    cout << reachedDepth << endl;
  });
});

int main(int argc, char *argv[]) {
  return bandit::run(argc, argv);
}
