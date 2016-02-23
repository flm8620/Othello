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

GameState::GameState(int Nsize):N(Nsize),nextMoveColor(Color::Black) {
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
    
    nextPossibleMoves = this->possibleMoves(nextMoveColor);
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
    if(player==Color::White){
        isWhite[i*N+j]=true;
    }else{
        isBlack[i*N+j]=true;
    }
    
    Color otherPlayer = player==Color::Black ? Color::White : Color::Black;
    nextPossibleMoves=this->possibleMoves(otherPlayer);
    if(nextPossibleMoves.empty()){
        nextMoveColor = player;
        nextPossibleMoves=this->possibleMoves(player);
        if(nextPossibleMoves.empty()){
            nextMoveColor=Color::Neither;
        }
    }
    
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

vector<pair<int,int> > GameState::position_NextTo_Piece(Color player) const{
    vector<pair<int, int>> Candidate_P;
    vector<bool> P_color = isColor_I(player);
    for (int i=0;i<N;i++){
        for(int j =0; j <N; j++){
            if (P_color[i*N+j]==true){
                if(i>0){
                    if(isBlack[(i-1)*N+j]==false && isWhite[(i-1)*N+j]==false){
                        Candidate_P.push_back(make_pair(i-1,j));
                    }
                    if(j>0){
                        if(isBlack[(i-1)*N+j-1]==false && isWhite[(i-1)*N+j-1]==false){
                            Candidate_P.push_back(make_pair(i-1,j-1));
                        }
                    }
                    if(j<N-1){
                        if(isBlack[(i-1)*N+j+1]==false && isWhite[(i-1)*N+j+1]==false){
                            Candidate_P.push_back(make_pair(i-1,j+1));
                        }
                        
                    }
                }
                if(i<N-1){
                    if(isBlack[(i+1)*N+j]==false && isWhite[(i+1)*N+j]==false){
                        Candidate_P.push_back(make_pair(i+1,j));
                    }
                    if(j>0){
                        if(isBlack[(i+1)*N+j-1]==false && isWhite[(i+1)*N+j-1]==false){
                            Candidate_P.push_back(make_pair(i-1,j-1));
                        }
                    }
                    if(j<N-1){
                        if(isBlack[(i+1)*N+j+1]==false && isWhite[(i+1)*N+j+1]==false){
                            Candidate_P.push_back(make_pair(i-1,j+1));
                        }
                        
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
    
    return Candidate_P;
}

set<pair<pair<int,int>, vector<bool>> > GameState::possibleMoves(Color player) const{
    vector<pair<int, int>> Candidate_P = position_NextTo_Piece(player);
    set<pair<pair<int,int>, vector<bool>> > P_moves;
    vector<bool>P_color_I = isColor_I(player);
    vector<bool>P_color = isColor(player);
    //int N=P_moves.size();
    for (auto const &it:Candidate_P) {
        int i=it.first;
        int j=it.second;
        vector<bool> Direction(8);
        if(i>0){
            if(P_color_I[(i-1)*N+j]==true){
                for(int k=i-1;k>=0;k--){
                    if(P_color[k*N+j]==true){       //direction 7
                        Direction[7]=true;
                        break;
                    }
                }
            }
            if(j>0){
                if(P_color_I[(i-1)*N+j-1]==true){    //direction 6
                    int l=min(i,j);
                    for(int k=l;k>0;k--){
                        if(P_color[(i-k)*N+(j-k)]==true){
                            Direction[6]=true;
                            break;
                        }
                    }
                }
            }
            if(j<N-1){
                if(P_color_I[(i-1)*N+j+1]==true){   //direction 0
                    int l=min(i,N-j-1);
                    for (int k= 1;k<=l;k++){
                        if(P_color[(i-k)*N+j+k]==true){
                            Direction[0]=true;
                            break;
                        }
                    }
                }
            }
        }
        if(i<N-1){
            if(P_color_I[(i+1)*N+j]==true){
                for(int k=i+2;k<N;k++){
                    if(P_color[k*N+j]==true){       //direction 3
                        Direction[3]=true;
                        break;
                    }
                }
            }
            if(j>0){
                if(P_color_I[(i+1)*N+j-1]==true){    //direction 4
                    int l=min(N-i-1,j);
                    for(int k=l;k>0;k--){
                        if(P_color[(i+k)*N+(j-k)]==true){
                            Direction[4]=true;
                            break;
                        }
                    }
                }
            }
            if(j<N-1){
                if(P_color_I[(i+1)*N+j+1]==true){   //direction 2
                    int l=min(N-i-1,N-j-1);
                    for (int k= 2;k<=l;k++){
                        if(P_color[(i+k)*N+j+k]==true){
                            Direction[2]=true;
                            break;
                        }
                    }
                }
            }
            
        }
        if(j>0){
            if(P_color_I[i*N+j-1]==true){           //direction 5
                for(int k=2; k<=j ; k++){
                    if(P_color[i*N+j-k]==true){
                        Direction[5]=true;
                        break;
                    }
                }
            }
        }
        if(j<N-1){
            if(P_color_I[i*N+j+1]==true){           //direction 1
                for(int k=j+2;k<N;k++){
                    if(P_color[i*N+k]==true){
                        Direction[1]=true;
                        break;
                    }
                }
            }
        }
        for(auto i = Direction.begin(); i!=Direction.end();i++){
            if(*i==true){
                P_moves.insert(make_pair(it,Direction));
                break;
            }
        }
    }
    return P_moves;
}


double GameState::evaluateBoardScore(const ChessBoardScore &score, Color player) const
{
    if(score.N!=this->N) throw invalid_argument("Chess Board size is not compatible!");
    double blackScore=0;
    double whiteScore=0;
    for(int i=0;i<N*N;i++){
        if(isBlack[i]) blackScore+=score.scores[i];
        if(isWhite[i]) whiteScore+=score.scores[i];
    }
    if(player==Color::Black) return blackScore-whiteScore;
    if(player==Color::White) return -blackScore+whiteScore;
}


