//
// Created by FLM on 2016/2/20.
//

#include "OthelloAI.h"
#include <cassert>

using namespace std;
using namespace std::chrono;
OthelloAI::OthelloAI(int Nsize) : diskSquare(Nsize), N(Nsize) {
  this->ID = generateID();
  diskSquare.randomizeScore();
  this->lambdas.resize(N * N);
  for (int i = 4; i < N * N; i++) {
    lambdas[i] = fmax(N * N * 0.8 - i, 0) / (N * N * 0.8 - 4);//lambdas is a decreasing function
  }
}

OthelloAI::OthelloAI(const OthelloAI &other) : diskSquare(other.diskSquare), N(other.N) {
  this->ID = generateID();
  lambdas = other.lambdas;
}

OthelloAI &OthelloAI::operator=(const OthelloAI &other) {
  diskSquare = other.diskSquare;
  ID = this->generateID();
  lambdas = other.lambdas;
}

void OthelloAI::useRecommandedChessBoardScore() {
  double M = DiskSquare::MAXSCORE;
  vector<double> triangle = {M * 20 / 20, -M * 3 / 20, M * 11 / 20, M * 8 / 20,
                             -M * 7 / 20, -M * 4 / 20, M * 1 / 20,
                             M * 2 / 20, M * 2 / 20,
                             -M * 3 / 20};
  this->diskSquare.scores = DiskSquare::triangleFormatToFullFormat(triangle, 8);
}

int OthelloAI::generateID() {
  static int staticID = -1;
  staticID++;
  return staticID;
}

double OthelloAI::max_min(const GameState &gs,
                          int depth,
                          bool isMyTurn,
                          Color myColor,
                          double alpha,
                          double beta,
                          int &iter,
                          bool limitTime) const {

  if (limitTime) {
    auto time = std::chrono::high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(time - this->startTime);
    if (time_span.count() > this->thinkTime) throw ExceptionTimeUp();
  }
  iter++;

  if (depth == 0 || gs.gameIsEnd()) {
    return evaluateScore(gs, myColor);
  }
  if (isMyTurn) {
    double a = -1e10;
    assert(gs.nextPlayer() == myColor);
    auto moves = gs.getPossibleMovesForNextPlayer();

    //sort moves
    vector<pair<int,int>> moveUnsorted;
    copy(moves.begin(),moves.end(),back_inserter(moveUnsorted));
    vector<double> predictScore(moveUnsorted.size());
    vector<int> indexSorted(moveUnsorted.size());
    for(int i=0; i<moveUnsorted.size(); i++){
      indexSorted[i]=i;
      GameState gsBranch = gs;
      gsBranch.addPiece(moveUnsorted[i].first, moveUnsorted[i].second, myColor);
      predictScore[i]=evaluateScore(gsBranch,myColor);
    }
    sort(indexSorted.begin(),indexSorted.end(),[&](int a,int b){
      return predictScore[a]>predictScore[b];
    });
    vector<pair<int,int>> sortedMoves(moveUnsorted.size());
    for(int i=0;i<sortedMoves.size();i++) sortedMoves[i] = moveUnsorted[indexSorted[i]];


    for (auto &move : sortedMoves) {
      GameState gsBranch = gs;
      gsBranch.addPiece(move.first, move.second, myColor);
      bool isMyTurnAgain = gsBranch.nextPlayer() == myColor;
      a = fmax(a, max_min(gsBranch, depth - 1, isMyTurnAgain, myColor, alpha, beta, iter));
      alpha = fmax(alpha, a);
      if (beta <= alpha)break;//beta cut off
    }
    return a;
  } else {
    double a = 1e10;
    Color otherPlayer = gs.nextPlayer();
    assert(gs.nextPlayer() != myColor);
    auto moves = gs.getPossibleMovesForNextPlayer();

    //sort moves
    vector<pair<int,int>> moveUnsorted;
    copy(moves.begin(),moves.end(),back_inserter(moveUnsorted));
    vector<double> predictScore(moveUnsorted.size());
    vector<int> indexSorted(moveUnsorted.size());
    for(int i=0; i<moveUnsorted.size(); i++){
      indexSorted[i]=i;
      GameState gsBranch = gs;
      gsBranch.addPiece(moveUnsorted[i].first, moveUnsorted[i].second, otherPlayer);
      predictScore[i]=evaluateScore(gsBranch,myColor);
    }
    sort(indexSorted.begin(),indexSorted.end(),[&](int a,int b){
      return predictScore[a]<predictScore[b];
    });
    vector<pair<int,int>> sortedMoves(moveUnsorted.size());
    for(int i=0;i<sortedMoves.size();i++) sortedMoves[i] = moveUnsorted[indexSorted[i]];


    for (auto &move : sortedMoves) {
      GameState gsBranch = gs;
      gsBranch.addPiece(move.first, move.second, otherPlayer);
      bool isMyTurnNow = gsBranch.nextPlayer() == myColor;
      a = fmin(a, max_min(gsBranch, depth - 1, isMyTurnNow, myColor, alpha, beta, iter));
      beta = fmin(beta, a);
      if (beta <= alpha) break; //alpha cut off
    }
    return a;
  }
}

double OthelloAI::evaluateScore(const GameState &gs, Color myColor) const {
  int whiteCount = gs.pieceCount(Color::White);
  int blackCount = gs.pieceCount(Color::Black);
  int step = whiteCount + blackCount;//current step count
  double positionScore = gs.evaluateBoardScore(this->diskSquare, myColor);
  double pieceCountScore = whiteCount - blackCount;
  if (myColor == Color::Black) pieceCountScore = -pieceCountScore;
  //lambda is a function of time(step)
  double lambda = lambdas[step];
  if (gs.gameIsEnd()) lambda = 0;
  //At the end of the game, lambda should be small
  return pieceCountScore * 100.0 + lambda * positionScore;
}

pair<int, int> OthelloAI::startMaxMin(const GameState &gs,
                                      Color myColor,
                                      int &iteration,
                                      int maxDepth,
                                      bool limitTime) const {
  double maxScore = -1e10;
  pair<int, int> bestMove = make_pair(-1, -1);
  const auto &moves = gs.getPossibleMovesForNextPlayer();

  //start minimax algo
  double alpha = -1e10;
  double beta = 1e10;
  for (const auto &move : moves) {
    GameState gsBranch = gs;
    gsBranch.addPiece(move.first, move.second, myColor);
    bool isMyTurnAgain = gsBranch.nextPlayer() == myColor;
    double s = max_min(gsBranch, maxDepth - 1, isMyTurnAgain, myColor, alpha, beta, iteration, limitTime);
    alpha = fmax(alpha, s);
    if (s > maxScore) {
      bestMove = move;
      maxScore = s;
    }
  }
  assert(bestMove != make_pair(-1, -1));

  return bestMove;
}

std::pair<int, int> OthelloAI::giveNextMove(const GameState &gs,
                                            Color myColor,
                                            int &iteration,
                                            int &reachedDepth,
                                            double thinkTime,
                                            int thinkDepth) const {
  this->thinkTime = thinkTime;

  assert(gs.nextPlayer() == myColor);
  startTime = std::chrono::high_resolution_clock::now();
  pair<int, int> bestMove = make_pair(-1, -1);

  if (thinkTime == 0.0) {//no time limit
    assert(thinkDepth >= 1);
    iteration = 0;
    int maxDepth = thinkDepth;
    int iter = 0;
    bestMove = startMaxMin(gs, myColor, iter, maxDepth, false);
    reachedDepth = maxDepth;
    iteration = iter;
  } else {
    iteration = 0;
    int maxDepth = 2;
    int iter = 0;
    bestMove = startMaxMin(gs, myColor, iter, maxDepth, false);
    reachedDepth = maxDepth;
    iteration = iter;

    for (int maxDepth = 3; maxDepth < 20; maxDepth++) {
      int iter = 0;
      try {
        bestMove = startMaxMin(gs, myColor, iter, maxDepth, true);
        reachedDepth = maxDepth;
        iteration += iter;
        cout << "normal: " << iter << endl;
      } catch (ExceptionTimeUp &e) {
        cout << "catched:" << iter << " " << maxDepth << endl;
        reachedDepth = maxDepth;
        iteration += iter;
        break;
      }
    }
  }

  return bestMove;
}


