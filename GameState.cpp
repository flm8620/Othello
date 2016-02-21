//
// Created by FLM on 2016/2/20.
//

#include "GameState.h"

using namespace std;
GameState::GameState(int Nsize):N(Nsize) {
    if (N%2==1){
        throw invalid_argument("size of chess board must be even");
    }

  isWhite.resize(N * N, false);
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
  isWhite[(N / 2 - 1) * N + (N / 2 - 1)] = true;
  isWhite[(N / 2) * N + (N / 2)] = true;
  isBlack[(N / 2) * N + (N / 2 - 1)] = true;
  isBlack[(N / 2 - 1) * N + (N / 2)] = true;
}

void GameState::printBoard() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (isWhite[i * N + j]) std::cout << 'O';
      else if (isBlack[i * N + j]) cout << '@';
      else cout << '-';
    }
    cout << endl;
  }
}
void GameState::addPiece(int i, int j, Color player) {
  if(i<0 || i>=N || j<0 || j>=N){
    throw invalid_argument("position out of bound");
  }
  if(isWhite[i*N+j] || isBlack[i*N+j]){
    throw invalid_argument("position already taken by a piece");
  }
  if(player==Color::White)
    isWhite[i*N+j]=true;
  else
    isBlack[i*N+j]=true;
}
