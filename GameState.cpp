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

vector<pair<int, int> > &GameState::getNeighbourOffset(int i, int j)const
{
  // for exemple:
  //  topRight:   ----@*
  //              ----@@
  //              ------
  //          * is  (i,j)
  //          @ are (i,j-1) (i+1,j-1) (i+1,j)
  //  so offset are (0, -1) (1,-1)    (1,0)
  typedef vector<pair<int, int> > Positions;
  static Positions topLeft =      {{0, 1},{1, 1},{1,0}};
  static Positions topRight =     {{0,-1},{1,-1},{1,0}};
  static Positions bottomLeft =   {{-1,0},{-1,1},{0,1}};
  static Positions bottomRight =  {{-1,-1},{-1,0},{0,-1}};
  static Positions left =         {{-1,0},{-1,1},{0,1},{1,1},{1,0}};
  static Positions right =        {{-1,0},{-1,-1},{0,-1},{1,-1},{1,0}};
  static Positions top =          {{0,-1},{1,-1},{1,0},{1,1},{0,1}};
  static Positions bottom =       {{0,-1},{-1,-1},{-1,0},{-1,1},{0,1}};
  static Positions center =       {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
  assert(i>=0&&i<N&&j>=0&&j<N);
  if(i==0){
    if(j==0) return topLeft;
    if(j==N-1) return topRight;
    return top;
  }else if(i==N-1){
    if(j==0) return bottomLeft;
    if(j==N-1) return bottomRight;
    return bottom;
  }else{
    if(j==0) return left;
    if(j==N-1) return right;
    return center;
  }
}

std::pair<int, int> GameState::getDirectionOffset(Direction direction) const
{
  switch (direction) {
    case TopLeft:    return make_pair(-1,-1);
    case TopRight:   return make_pair(-1, 1);
    case BottomLeft: return make_pair( 1,-1);
    case BottomRight:return make_pair( 1, 1);
    case Top:        return make_pair(-1, 0);
    case Bottom:     return make_pair( 1, 0);
    case Left:       return make_pair( 0,-1);
    case Right:      return make_pair( 0, 1);
  }
}

GameState::GameState(int Nsize):N(Nsize),nextMoveColor(Color::Black) {
  if (N%2==1){
    throw invalid_argument("size of chess board must be even");
  }
  this->restartGame();
}

void GameState::restartGame()
{
  nextMoveColor=Color::Black;
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
  isWhite.resize(N * N, false);
  isBlack.resize(N * N, false);
  isWhite[(N / 2 - 1) * N + (N / 2 - 1)] = true;
  isWhite[(N / 2) * N + (N / 2)] = true;
  isBlack[(N / 2) * N + (N / 2 - 1)] = true;
  isBlack[(N / 2 - 1) * N + (N / 2)] = true;
  this->updatePossibleMoves(nextMoveColor);
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
  Color otherPlayer = player == Color::Black ? Color::White : Color::Black;
  if(i<0 || i>=N || j<0 || j>=N){
    throw invalid_argument("position out of bound");
  }
  if(isWhite[i*N+j] || isBlack[i*N+j]){
    throw invalid_argument("position already taken by a piece");
  }
  if(nextPlayer()==Color::Neither){
    throw logic_error("Game has ended, why you still want to play?");
  }
  if(nextPlayer()==otherPlayer){
    throw logic_error("It's not your turn!");
  }
  vector<bool>& hasAdversary = player == Color::Black ? isWhite : isBlack;
  vector<bool>& hasPlayer = player == Color::Black ? isBlack : isWhite;

  hasPlayer[i*N+j]=true;

  auto positionPlayed = make_pair(i, j);

  vector<bool> Direction_played (8);
  auto it = this->nextPossibleMoves.find(positionPlayed);
  if(it==this->nextPossibleMoves.end())
    throw invalid_argument("illegal move");

  vector<Direction> directions = this->moveWithDirection[positionPlayed];
  for(Direction d : directions){
    pair<int,int> offset = getDirectionOffset(d);
    int ii=i+offset.first;
    int jj=j+offset.second;
    assert(hasAdversary[ii*N+jj]);
    while(ii>=0&&ii<N&&jj>=0&&jj<N){
      if(hasAdversary[ii*N+jj]){
        hasAdversary[ii*N+jj]=false;
        hasPlayer[ii*N+jj]=true;
      }else{
        break;
      }
      ii+=offset.first;
      jj+=offset.second;
    }
  }

  this->updatePossibleMoves(otherPlayer);
  nextMoveColor = otherPlayer;
  if(nextPossibleMoves.empty()){
    nextMoveColor = player;
    this->updatePossibleMoves(player);
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
  const vector<bool>& hasColor = player == Color::Black ? isBlack : isWhite;
  for (int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      if (!isBlack[i*N+j] && !isWhite[i*N+j]){
        const vector<pair<int, int> >& neighbours = getNeighbourOffset(i,j);
        for(auto offset : neighbours){
          if(hasColor[(i+offset.first)*N+j+offset.second]){
            Candidate_P.push_back(make_pair(i,j));
            break;
          }
        }
      }
    }
  }
  return Candidate_P;
}

void GameState::updatePossibleMoves(Color player){
  Color adversary = player == Color::Black ? Color::White : Color::Black;
  vector<pair<int, int>> Candidate_P = position_NextTo_Piece(adversary);
  set<pair<pair<int,int>, vector<Direction>> > possibleMoves;
  const vector<bool>& hasAdversary = player == Color::Black ? isWhite : isBlack;
  const vector<bool>& hasPlayer = player == Color::Black ? isBlack : isWhite;

  this->nextPossibleMoves.clear();
  this->moveWithDirection.clear();

  for (const auto &move : Candidate_P) {
    int i=move.first;
    int j=move.second;
    assert(!isBlack[i*N+j] && !isWhite[i*N+j]);
    vector<Direction> directions =
    {Direction::TopLeft, Direction::Top, Direction::TopRight,
     Direction::Left,                    Direction::Right,
     Direction::BottomLeft,Direction::Bottom,Direction::BottomRight};

    bool isLegalMove = false;

    for(auto d : directions){
      pair<int,int> offset = getDirectionOffset(d);
      int ii=move.first+offset.first;
      int jj=move.second+offset.second;
      bool hasMetAdversary=false;
      while(ii>=0&&ii<N&&jj>=0&&jj<N){
        if(hasAdversary[ii*N+jj]){
          hasMetAdversary=true;
        }
        else if(hasPlayer[ii*N+jj]){
          if(hasMetAdversary){
            isLegalMove=true;
            this->moveWithDirection[move].push_back(d);
            break;
          }else{
            break;
          }
        }else{
          break;
        }
        ii+=offset.first;
        jj+=offset.second;
      }
    }
    if(isLegalMove) this->nextPossibleMoves.insert(move);
  }
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

void GameState::setColorPositionPlayer(std::vector<bool> black, std::vector<bool> white, Color nextPlayer)
{
  if(black.size()!=N*N || white.size()!=N*N)
    throw invalid_argument("wrong size");
  for(int i=0;i<N*N;i++){
    if(black[i]&&white[i]) throw invalid_argument("black white can't take the same place");
  }
  this->isBlack=black;
  this->isWhite=white;
  this->updatePossibleMoves(nextPlayer);
  Color otherPlayer = nextPlayer == Color::Black ? Color::White : Color::Black;
  nextMoveColor = nextPlayer;
  if(nextPossibleMoves.empty()){
    nextMoveColor = otherPlayer;
    this->updatePossibleMoves(otherPlayer);
    if(nextPossibleMoves.empty()){
      nextMoveColor=Color::Neither;
    }
  }
}


