//
// Created by FLM on 2016/2/20.
//

#include "GameState.h"
using namespace std;
GameState::GameState() {
  isWhile.resize(N * N, false);
  isBlack.resize(N * N, false);
  //four pieces is put to start
  //    01234567
  //  0 OOOOOOOO
  //  1 OOOOOOOO
  //  2 OOOOOOOO
  //  3 OOOWBOOO
  //  4 OOOBWOOO
  //  5 OOOOOOOO
  //  6 OOOOOOOO
  //  7 OOOOOOOO
  //
  // index of position (i,j) is [i*N+j]
  isWhile[(N / 2 - 1) * N + (N / 2 - 1)] = true;
  isWhile[(N / 2) * N + (N / 2)] = true;
  isBlack[(N / 2) * N + (N / 2 - 1)] = true;
  isBlack[(N / 2 - 1) * N + (N / 2)] = true;
}

void GameState::printBoard() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (isWhile[i * N + j]) std::cout << 'O';
      else if (isBlack[i * N + j]) cout << '@';
      else cout << '-';
    }
    cout << endl;
  }
}