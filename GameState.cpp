//
// Created by FLM on 2016/2/20.
//

#include "GameState.h"
#include <stdexcept>
#include <chrono>
#include <random>
#include <cassert>
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

int GameState::pieceCount(Color player) const
{
  int sum=0;
  if(player==Color::Black){
    for(auto b : isBlack) if(b)sum++;
  }else if(player==Color::White){
    for(auto b : isWhite) if(b)sum++;
  }else{
    assert(false);
  }
  return sum;
}

vector<pair<int,int> > GameState::position_NextTo_Piece(Color player){
    vector<pair<int, int>> Candidate_P;
    if (player==Color::White){
        for (int i=0;i<N;i++){
            for(int j =0; j <N; j++){
                if (isBlack[i*N+j]==true){
                    if(i>0){
                        if(isBlack[(i-1)*N+j]==false && isWhite[(i-1)*N+j]==false){
                            Candidate_P.push_back(make_pair(i-1,j));
                        }
                    }
                    if(i<N-1){
                        if(isBlack[(i+1)*N+j]==false && isWhite[(i+1)*N+j]==false){
                            Candidate_P.push_back(make_pair(i+1,j));
                        }
                    }
                    if(j>0){
                        if(isBlack[i*N+j-1]==false && isWhite[i*N+j-1]==false){
                            Candidate_P.push_back(make_pair(i,j-1));
                        }
                    }
                    if(j<N-1){
                        if(isBlack[i*N+j+1]==false && isWhite[i*N+j+1]==false){
                            Candidate_P.push_back(make_pair(i,j+1));
                        }
                    }
                }
            }
        }
    }
    if (player==Color::Black){
        for (int i=0;i<N;i++){
            for(int j =0; j <N; j++){
                if (isWhite[i*N+j]==true){
                    if(i>0){
                        if(isBlack[(i-1)*N+j]==false && isWhite[(i-1)*N+j]==false){
                            pair<int, int> p(i-1,j);
                            Candidate_P.push_back(p);
                        }
                    }
                    if(i<N-1){
                        if(isBlack[(i+1)*N+j]==false && isWhite[(i+1)*N+j]==false){
                            pair<int, int> p(i+1,j);
                            Candidate_P.push_back(p);
                        }
                    }
                    if(j>0){
                        if(isBlack[i*N+j-1]==false && isWhite[i*N+j-1]==false){
                            pair<int, int> p(i,j-1);
                            Candidate_P.push_back(p);
                        }
                    }
                    if(j<N-1){
                        if(isBlack[i*N+j+1]==false && isWhite[i*N+j+1]==false){
                            pair<int, int> p(i,j+1);
                            Candidate_P.push_back(p);
                        }
                    }
                }
            }
        }
    }
    return Candidate_P;
}


