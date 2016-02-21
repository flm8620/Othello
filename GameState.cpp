//
// Created by FLM on 2016/2/20.
//

#include "GameState.h"
#include <stdexcept>
#include <chrono>
#include <random>
using namespace std;

auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 generator(seed);

void ChessBoardScore::randomizeScore() {
  uniform_real_distribution<double> randReal(-1000,1000);
  //score should be symmetic
  //score(i,j) == score(N-i-1,N-j-1)
  int NN=N*N;
  for(int i=0;i<N/2;i++){
    for(int j=0;j<N;j++){
      int index=i*N+j;
      scores[index]=scores[NN-index-1]=randReal(generator);
    }
  }
}

GameState::GameState(int Nsize):N(Nsize) {
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

void GameState::printBoard() const {
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

