//
// Created by FLM on 2016/2/20.
//

#include "OthelloAI.h"
#include <cassert>
#include <random>
#include <limits>

using namespace std;
using namespace std::chrono;

extern mt19937 generator;

OthelloAI::OthelloAI(int Nsize) : diskSquare(Nsize), N(Nsize) {
  this->ID = generateID();
  diskSquare.randomizeScore();
  scoreWeight.randomnize();
}

OthelloAI::OthelloAI(const OthelloAI &other) : diskSquare(other.diskSquare), N(other.N) {
  this->ID = generateID();
  scoreWeight = other.scoreWeight;
}

OthelloAI &OthelloAI::operator=(const OthelloAI &other) {
  diskSquare = other.diskSquare;
  ID = this->generateID();
  scoreWeight = other.scoreWeight;
  return *this;
}

void OthelloAI::useRecommandedDiskSquare() {
  double M = DiskSquare::INITIAL_SCORE_RANGE;
  vector<double> triangle = {M * 20 / 20, -M * 3 / 20, M * 11 / 20, M * 8 / 20,
                             -M * 7 / 20, -M * 4 / 20, M * 1 / 20,
                             M * 2 / 20, M * 2 / 20,
                             -M * 3 / 20};
  this->diskSquare.scores = DiskSquare::triangleFormatToFullFormat(triangle, 8);
}

OthelloAI *OthelloAI::createNewAIByMutation() const {
  int M = N / 2;
  int MM = M * (1 + M) / 2;
  uniform_int_distribution<int> randi(0, MM - 1);
  uniform_real_distribution<double> randRatio(0.66,1.5);
  std::bernoulli_distribution takeNegative(0.25);

  auto triangle = this->diskSquare.getTriangleFormat();

  uniform_int_distribution<int> modifyTimes(1, 4);
  int times = modifyTimes(generator);
  for(int i=0;i<times;i++){
    //pick one element in ChessBoardScore and modify it to a random real number
    int randomIndex = randi(generator);
    triangle[randomIndex] *= randRatio(generator);
    if(takeNegative(generator)) triangle[randomIndex] *=-1;
    //if one value is larger than the bound, we don't care!

    //  if(abs(triangle[randomIndex])>DiskSquare::MAXSCORE){
    //    double ratio = DiskSquare::MAXSCORE/triangle[randomIndex];
    //    for(auto & t : triangle)
    //      t*=ratio;
    //  }
  }


  DiskSquare ds(N);
  ds.scores = DiskSquare::triangleFormatToFullFormat(triangle, N);

  //scoreWeight
  auto newScoreWeight = this->scoreWeight;
  newScoreWeight.mutateMyself();

  OthelloAI *newAI = new OthelloAI(N);
  newAI->diskSquare = ds;
  newAI->scoreWeight = newScoreWeight;

  return newAI;
}

OthelloAI *OthelloAI::createNewAIByCrossOver(const OthelloAI *other) const {
  //diskSquare

  auto triangle1 = this->diskSquare.getTriangleFormat();
  auto triangle2 = other->diskSquare.getTriangleFormat();
  uniform_int_distribution<int> randi(0, 1);
  auto triangle = triangle1;
  //chessBoardScore is just like DNA
  //we take parents DNA randomly
  for (int i = 0; i < triangle.size(); i++) {
    if (randi(generator) == 1) {
      triangle[i] = triangle1[i];
    } else {
      triangle[i] = triangle2[i];
    }
  }
  OthelloAI *newAI = new OthelloAI(N);
  DiskSquare ds(N);
  ds.scores = DiskSquare::triangleFormatToFullFormat(triangle, N);
  newAI->diskSquare = ds;

  //scoreWeight
  auto sw = other->getScoreWeight();
  newAI->scoreWeight = scoreWeight.crossover(sw);

  return newAI;
}

int OthelloAI::generateID() {
  static uniform_int_distribution<int> prefix(1,100);
  static int staticID = prefix(generator)*10000-1;
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

  if (depth == 0) {
    return evaluateScore(gs, myColor);
  }
  if(gs.gameIsEnd()){
    int playerPieceCount = gs.pieceCount(myColor);
    Color other = myColor == Color::Black ? Color::White : Color::Black;
    int otherPieceCount = gs.pieceCount(other);
    double largeNumber = std::numeric_limits<double>::max()/1e10;
    return largeNumber*(playerPieceCount-otherPieceCount);
  }
  if (isMyTurn) {
    double a = -std::numeric_limits<double>::max();
    assert(gs.nextPlayer() == myColor);
    auto moves = gs.getPossibleMovesForNextPlayer();

    //sort moves
    vector<pair<int, int>> moveUnsorted;
    copy(moves.begin(), moves.end(), back_inserter(moveUnsorted));
    vector<double> predictScore(moveUnsorted.size());
    vector<int> indexSorted(moveUnsorted.size());
    for (int i = 0; i < moveUnsorted.size(); i++) {
      indexSorted[i] = i;
      GameState gsBranch = gs;
      gsBranch.addPiece(moveUnsorted[i].first, moveUnsorted[i].second, myColor);
      predictScore[i] = evaluateScore(gsBranch, myColor);
    }
    sort(indexSorted.begin(), indexSorted.end(), [&](int a, int b) {
      return predictScore[a] > predictScore[b];
    });
    vector<pair<int, int>> sortedMoves(moveUnsorted.size());
    for (int i = 0; i < sortedMoves.size(); i++) sortedMoves[i] = moveUnsorted[indexSorted[i]];


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
    double a = std::numeric_limits<double>::max();
    Color otherPlayer = gs.nextPlayer();
    assert(gs.nextPlayer() != myColor);
    auto moves = gs.getPossibleMovesForNextPlayer();

    //sort moves
    vector<pair<int, int>> moveUnsorted;
    copy(moves.begin(), moves.end(), back_inserter(moveUnsorted));
    vector<double> predictScore(moveUnsorted.size());
    vector<int> indexSorted(moveUnsorted.size());
    for (int i = 0; i < moveUnsorted.size(); i++) {
      indexSorted[i] = i;
      GameState gsBranch = gs;
      gsBranch.addPiece(moveUnsorted[i].first, moveUnsorted[i].second, otherPlayer);
      predictScore[i] = evaluateScore(gsBranch, myColor);
    }
    sort(indexSorted.begin(), indexSorted.end(), [&](int a, int b) {
      return predictScore[a] < predictScore[b];
    });
    vector<pair<int, int>> sortedMoves(moveUnsorted.size());
    for (int i = 0; i < sortedMoves.size(); i++) sortedMoves[i] = moveUnsorted[indexSorted[i]];


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
  //score from the article "Playing Othello with Artificial Intelligence" by  Michael J. Korman
  static normal_distribution<double> normal_dist(1.0,0.015);
  int myPieceCount = myColor == Black ? gs.pieceCount(Black) : gs.pieceCount(White);
  int hisPieceCount = myColor == White ? gs.pieceCount(Black) : gs.pieceCount(White);
  int time = myPieceCount + hisPieceCount;//current step count

  double positionScore = gs.evaluateBoardScore(this->diskSquare, myColor);
  double pieceScore;
  if (myPieceCount == hisPieceCount) pieceScore = 0;
  else if (myPieceCount > hisPieceCount) {
    pieceScore = 100.0 * myPieceCount / (myPieceCount + hisPieceCount);
  } else {
    pieceScore = -100.0 * hisPieceCount / (myPieceCount + hisPieceCount);
  }


  auto hisColor = myColor == Black ? White : Black;
  int myMobility = gs.playerMobility(myColor);
  int hisMobility = gs.playerMobility(hisColor);
  double mobilityScore;
  if (myMobility == hisMobility) mobilityScore = 0;
  else if (myMobility > hisMobility) {
    mobilityScore = 100.0 * myMobility / (myMobility + hisMobility);
  } else {
    mobilityScore = -100.0 * hisMobility / (myMobility + hisMobility);
  }

  double positionWeight, mobilityWeight, pieceWeight;
  this->scoreWeight.getThreeWeights(mobilityWeight, positionWeight, pieceWeight, time);
  assert(mobilityWeight > -1e-5 && positionWeight > -1e-5 && pieceWeight > -1e-5);
  double final = mobilityWeight * mobilityScore + positionWeight * positionScore + pieceWeight * pieceScore;
  return final*normal_dist(generator);
}

pair<int, int> OthelloAI::startMaxMin(const GameState &gs,
                                      Color myColor,
                                      int &iteration,
                                      int maxDepth,
                                      bool limitTime) const {
  double maxScore = -std::numeric_limits<double>::max();
  pair<int, int> bestMove = make_pair(-1, -1);
  const auto &moves = gs.getPossibleMovesForNextPlayer();

  //start minimax algo
  double alpha = -std::numeric_limits<double>::max();
  double beta = std::numeric_limits<double>::max();
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
    int maxDepth = 3;
    int iter = 0;
    bestMove = startMaxMin(gs, myColor, iter, maxDepth, false);
    reachedDepth = maxDepth;
    iteration = iter;

    for (int maxDepth = 4; maxDepth < 20; maxDepth++) {
      int iter = 0;
      try {
        bestMove = startMaxMin(gs, myColor, iter, maxDepth, true);
        reachedDepth = maxDepth;
        iteration += iter;
      } catch (ExceptionTimeUp &e) {
        //cout << "catched:" << iter << " " << maxDepth << endl;
        break;
      }
    }
  }

  return bestMove;
}

void OthelloAI::ScoreWeight::getThreeWeights(double &mobility, double &position, double &piecenumber, double t) const {
  assert(endTime > middleTime);
  if (t < middleTime) {
    mobility = (mobility_middle - mobility_begin) / (middleTime - 4) * (t - 4) + mobility_begin;
    position = (position_middle - position_begin) / (middleTime - 4) * (t - 4) + position_begin;
    piecenumber = 1 - mobility - position;
    assert(piecenumber >= -1e-5 && mobility >= -1e-5 && position >= -1e-5);
  } else if (t < endTime) {
    mobility = (0 - mobility_middle) / (endTime - middleTime) * (t - middleTime) + mobility_middle;
    position = (0 - position_middle) / (endTime - middleTime) * (t - middleTime) + position_middle;
    piecenumber = 1 - mobility - position;
    assert(piecenumber >= -1e-5 && mobility >= -1e-5 && position >= -1e-5);
  } else {
    mobility = position = 0;
    piecenumber = 1;
  }
}

void OthelloAI::ScoreWeight::randomnize() {
  uniform_real_distribution<double> randMobility(0.4, 0.6);
  uniform_real_distribution<double> randPosition(0.2, 0.4);
  this->mobility_begin = randMobility(generator);
  this->position_begin = randPosition(generator);
  uniform_real_distribution<double> randMobility_middle(0.2, 0.4);
  uniform_real_distribution<double> randPosition_middle(0.4, 0.6);
  this->mobility_middle = randMobility_middle(generator);
  this->position_middle = randPosition_middle(generator);
  uniform_real_distribution<double> rand_middle(20, 30);
  uniform_real_distribution<double> rand_end(45, 55);
  this->middleTime = rand_middle(generator);
  this->endTime = rand_end(generator);
}

void OthelloAI::ScoreWeight::mutateMyself() {
  uniform_real_distribution<double> deltaTime(-2, 2);
  double delta1 = deltaTime(generator);
  double delta2 = deltaTime(generator);
  middleTime += delta1;
  endTime += delta2;
  middleTime = fmin(fmax(middleTime, 4), 63);
  endTime = fmin(fmax(endTime, 4), 63);
  if (middleTime > endTime) {
    middleTime = (middleTime + endTime) / 2 - 1;
    endTime = middleTime + 1;
  }

  uniform_real_distribution<double> ratio(0.8, 1.25);
  double piececount_begin = 1 - mobility_begin - position_begin;
  assert(piececount_begin > -1e-5);
  mobility_begin *= ratio(generator);
  position_begin *= ratio(generator);
  piececount_begin *= ratio(generator);
  double sum = mobility_begin + position_begin + piececount_begin;
  mobility_begin /= sum;
  position_begin /= sum;

  double piececount_middle = 1 - mobility_middle - position_middle;
  assert(piececount_middle > -1e-5);
  mobility_middle *= ratio(generator);
  position_middle *= ratio(generator);
  piececount_middle *= ratio(generator);
  sum = mobility_middle + position_middle + piececount_middle;
  mobility_middle /= sum;
  position_middle /= sum;
}

OthelloAI::ScoreWeight OthelloAI::ScoreWeight::crossover(OthelloAI::ScoreWeight other) const {
  ScoreWeight newScoreWeight;
  uniform_int_distribution<int> randi(0, 1);
  newScoreWeight.middleTime = randi(generator) == 1 ? middleTime : other.middleTime;
  newScoreWeight.endTime = randi(generator) == 1 ? endTime : other.endTime;
  newScoreWeight.mobility_begin = randi(generator) == 1 ? mobility_begin : other.mobility_begin;
  newScoreWeight.mobility_middle = randi(generator) == 1 ? mobility_middle : other.mobility_middle;
  newScoreWeight.position_begin = randi(generator) == 1 ? position_begin : other.position_begin;
  newScoreWeight.position_middle = randi(generator) == 1 ? position_middle : other.position_middle;
  return newScoreWeight;
}
