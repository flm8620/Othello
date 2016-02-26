//
// Created by FLM on 2016/2/20.
//

#include "GameState.h"
#include <chrono>
#include <cassert>
#include <iomanip>
using namespace std;

auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 generator(seed);

const double DiskSquare::MAXSCORE = 50.0;

static map<Direction, pair<int, int>> dirToOffset =
    {{TopLeft, {-1, -1}}, {Top, {-1, 0}}, {TopRight, {-1, 1}},
     {Left, {0, -1}}, {Right, {0, 1}},
     {BottomLeft, {1, -1}}, {Bottom, {1, 0}}, {BottomRight, {1, 1}}};

static map<pair<int, int>, Direction> offsetToDir =
    {{{-1, -1}, TopLeft}, {{-1, 0}, Top}, {{-1, 1}, TopRight},
     {{0, -1}, Left}, {{0, 1}, Right},
     {{1, -1}, BottomLeft}, {{1, 0}, Bottom}, {{1, 1}, BottomRight}};

static map<pair<int, int>, int> offsetToDirInt =
    {{{-1, -1}, 0}, {{-1, 0}, 1}, {{-1, 1}, 2},
     {{0, -1}, 7}, {{0, 1}, 3},
     {{1, -1}, 6}, {{1, 0}, 5}, {{1, 1}, 4}};

DiskSquare::DiskSquare(int Nsize) : N(Nsize) {
  scores.resize(N * N, 0);
}

DiskSquare::DiskSquare(const DiskSquare &other) : N(other.N) {
  scores = other.scores;
}

DiskSquare &DiskSquare::operator=(const DiskSquare &other) {
  this->scores = other.scores;
}

std::vector<double> DiskSquare::triangleFormatToFullFormat(std::vector<double> triangle, int N) {
  // by symmetry, we only need to consider a triangle
  // ****----
  // -***----
  // --**----
  // ---*----
  // --------
  // --------
  // --------
  // --------
  int M = N / 2;
  int sizeTriangle = M * (1 + M) / 2;
  if (triangle.size() != sizeTriangle) throw invalid_argument("triangle size incorrect");
  vector<double> full(N * N);
  int k = 0;
  for (int i = 0; i < N / 2; i++) {
    for (int j = i; j < N / 2; j++) {
      full[i * N + j] = full[j * N + i] = triangle[k];
      full[(N - i - 1) * N + (N - j - 1)] = full[(N - i - 1) * N + j] = full[i * N + (N - j - 1)] = triangle[k];
      full[(N - j - 1) * N + (N - i - 1)] = full[(N - j - 1) * N + i] = full[j * N + (N - i - 1)] = triangle[k];
      k++;
    }
  }
  return full;
}

std::vector<double> DiskSquare::getTriangleFormat() const {
  vector<double> t;
  for (int i = 0; i < N / 2; i++)
    for (int j = i; j < N / 2; j++)
      t.push_back(scores[i * N + j]);
  return t;
}

void DiskSquare::printOut() {
  for (int i = 0; i < N / 2; i++) {
    for (int j = 0; j < i; j++)cout << setw(10) << ' ';
    for (int j = i; j < N / 2; j++) {
      cout << setw(10) << scores[i * N + j];
    }
    std::cout << std::endl;
  }
}

void DiskSquare::randomizeScore() {
  uniform_real_distribution<double> randReal(-MAXSCORE, MAXSCORE);
  //score should be symmetic
  //score(i,j) == score(N-i-1,N-j-1)
  vector<double> triangle;
  int M = N / 2;
  for (int i = 0; i < M * (1 + M) / 2; i++)triangle.push_back(randReal(generator));
  scores = triangleFormatToFullFormat(triangle, N);
}

const vector<pair<int, int> > &GameState::getNeighbourOffset(int i, int j) const {
  // for exemple:
  //  topRight:   ----@*
  //              ----@@
  //              ------
  //          * is  (i,j)
  //          @ are (i,j-1) (i+1,j-1) (i+1,j)
  //  so offset are (0, -1) (1,-1)    (1,0)
  typedef vector<pair<int, int> > Positions;
  static Positions topLeft = {{0, 1}, {1, 1}, {1, 0}};
  static Positions topRight = {{0, -1}, {1, -1}, {1, 0}};
  static Positions bottomLeft = {{-1, 0}, {-1, 1}, {0, 1}};
  static Positions bottomRight = {{-1, -1}, {-1, 0}, {0, -1}};
  static Positions left = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}};
  static Positions right = {{-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}};
  static Positions top = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}};
  static Positions bottom = {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}};
  static Positions center = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
  assert(i >= 0 && i < N && j >= 0 && j < N);
  if (i == 0) {
    if (j == 0) return topLeft;
    if (j == N - 1) return topRight;
    return top;
  } else if (i == N - 1) {
    if (j == 0) return bottomLeft;
    if (j == N - 1) return bottomRight;
    return bottom;
  } else {
    if (j == 0) return left;
    if (j == N - 1) return right;
    return center;
  }
}

std::pair<int, int> GameState::getDirectionOffset(Direction direction) const {
  switch (direction) {
    case TopLeft:
      return make_pair(-1, -1);
    case TopRight:
      return make_pair(-1, 1);
    case BottomLeft:
      return make_pair(1, -1);
    case BottomRight:
      return make_pair(1, 1);
    case Top:
      return make_pair(-1, 0);
    case Bottom:
      return make_pair(1, 0);
    case Left:
      return make_pair(0, -1);
    case Right:
      return make_pair(0, 1);
  }
}

GameState::GameState(int Nsize) : N(Nsize), nextMoveColor(Color::Black) {
  if (N % 2 == 1) {
    throw invalid_argument("size of chess board must be even");
  }
  this->restartGame();
}

void GameState::restartGame() {
  isWhite.clear();
  isBlack.clear();
  lastPosition = make_pair(-1, -1);
  nextPossibleMovesBlack.clear();
  nextPossibleMovesWhite.clear();
  moveWithDirectionBlack.clear();
  moveWithDirectionWhite.clear();

  nextMoveColor = Color::Black;
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
      if (isWhite[i * N + j]) {
        if (lastPosition == make_pair(i, j))
          std::cout << 'G';
        else
          std::cout << 'O';
      }
      else if (isBlack[i * N + j]) {
        if (lastPosition == make_pair(i, j))
          std::cout << '#';
        else
          std::cout << '@';
      }

      else cout << '-';
    }
    cout << endl;
  }
}

void GameState::addPiece(int i, int j, Color player) {
  Color otherPlayer = player == Color::Black ? Color::White : Color::Black;
  if (i < 0 || i >= N || j < 0 || j >= N) {
    throw invalid_argument("position out of bound");
  }
  if (isWhite[i * N + j] || isBlack[i * N + j]) {
    throw invalid_argument("position already taken by a piece");
  }
  if (nextPlayer() == Color::Neither) {
    throw logic_error("Game has ended, why you still want to play?");
  }
  if (nextPlayer() == otherPlayer) {
    throw logic_error("It's not your turn!");
  }
  vector<bool> &hasAdversary = player == Color::Black ? isWhite : isBlack;
  vector<bool> &hasPlayer = player == Color::Black ? isBlack : isWhite;
  auto &nextPossibleMoves = player == Color::Black ? nextPossibleMovesBlack : nextPossibleMovesWhite;
  auto &moveWithDirection = player == Color::Black ? moveWithDirectionBlack : moveWithDirectionWhite;
  auto &nextPossibleMovesOther = player == Color::White ? nextPossibleMovesBlack : nextPossibleMovesWhite;
  auto positionPlayed = make_pair(i, j);

  //verify move
  auto it = nextPossibleMoves.find(positionPlayed);
  if (it == nextPossibleMoves.end())
    throw invalid_argument("illegal move");

  //put piece
  hasPlayer[i * N + j] = true;

  //record as lastPosition
  lastPosition = positionPlayed;

  vector<Direction> directions = moveWithDirection[positionPlayed];
  for (Direction d : directions) {
    pair<int, int> offset = getDirectionOffset(d);
    int ii = i + offset.first;
    int jj = j + offset.second;
    assert(hasAdversary[ii * N + jj]);
    while (ii >= 0 && ii < N && jj >= 0 && jj < N) {
      if (hasAdversary[ii * N + jj]) {
        hasAdversary[ii * N + jj] = false;
        hasPlayer[ii * N + jj] = true;
      } else {
        break;
      }
      ii += offset.first;
      jj += offset.second;
    }
  }

  this->updatePossibleMoves(Color::Black);
  this->updatePossibleMoves(Color::White);
  nextMoveColor = otherPlayer;
  if (nextPossibleMovesOther.empty()) {
    nextMoveColor = player;
    if (nextPossibleMoves.empty()) {
      nextMoveColor = Color::Neither;
    }
  }
}


int GameState::pieceCount(Color player) const {
  int sum = 0;
  if (player == Color::Black) {
    for (auto b : isBlack) if (b)sum++;
  } else if (player == Color::White) {
    for (auto b : isWhite) if (b)sum++;
  } else {
    assert(false);
  }
  return sum;
}

vector<pair<int, int>> GameState::position_NextTo_Piece(Color adversary) const {
  vector<pair<int, int>> candidatePosDir;
  const auto &hasAdversary = adversary == Color::Black ? isBlack : isWhite;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (!isBlack[i * N + j] && !isWhite[i * N + j]) {
        const vector<pair<int, int> > &neighbours = getNeighbourOffset(i, j);
        for (auto offset : neighbours) {
          if (hasAdversary[(i + offset.first) * N + j + offset.second]) {
            candidatePosDir.push_back(make_pair(i, j));
            break;
          }
        }
      }
    }
  }
  return candidatePosDir;
}

void GameState::updatePossibleMoves(Color player) {
  Color adversary = player == Color::Black ? Color::White : Color::Black;
  auto &nextPossibleMoves = player == Color::Black ? nextPossibleMovesBlack : nextPossibleMovesWhite;
  auto &moveWithDirection = player == Color::Black ? moveWithDirectionBlack : moveWithDirectionWhite;

  vector<pair<int, int>> Candidate_P = position_NextTo_Piece(adversary);
  const vector<bool> &hasAdversary = player == Color::Black ? isWhite : isBlack;
  const vector<bool> &hasPlayer = player == Color::Black ? isBlack : isWhite;

  nextPossibleMoves.clear();
  moveWithDirection.clear();

  for (const auto &move : Candidate_P) {
    int i = move.first;
    int j = move.second;
    assert(!isBlack[i * N + j] && !isWhite[i * N + j]);
    vector<Direction> directions =
        {Direction::TopLeft, Direction::Top, Direction::TopRight,
         Direction::Left, Direction::Right,
         Direction::BottomLeft, Direction::Bottom, Direction::BottomRight};

    bool isLegalMove = false;

    for (auto d : directions) {
      pair<int, int> offset = getDirectionOffset(d);
      int ii = move.first + offset.first;
      int jj = move.second + offset.second;
      bool hasMetAdversary = false;
      while (ii >= 0 && ii < N && jj >= 0 && jj < N) {
        if (hasAdversary[ii * N + jj]) {
          hasMetAdversary = true;
        }
        else if (hasPlayer[ii * N + jj]) {
          if (hasMetAdversary) {
            isLegalMove = true;
            moveWithDirection[move].push_back(d);
            break;
          } else {
            break;
          }
        } else {
          break;
        }
        ii += offset.first;
        jj += offset.second;
      }
    }
    if (isLegalMove) nextPossibleMoves.insert(move);
  }
}


double GameState::evaluateBoardScore(const DiskSquare &score, Color player) const {
  if (score.N != this->N) throw invalid_argument("Chess Board size is not compatible!");
  double blackScore = 0;
  double whiteScore = 0;
  for (int i = 0; i < N * N; i++) {
    if (isBlack[i]) blackScore += score.scores[i];
    if (isWhite[i]) whiteScore += score.scores[i];
  }
  if (player == Color::Black) return blackScore - whiteScore;
  if (player == Color::White) return -blackScore + whiteScore;
}

void GameState::setColorPositionPlayer(std::vector<bool> black, std::vector<bool> white, Color nextPlayer) {
  if (black.size() != N * N || white.size() != N * N)
    throw invalid_argument("wrong size");
  for (int i = 0; i < N * N; i++) {
    if (black[i] && white[i]) throw invalid_argument("black white can't take the same place");
  }
  this->isBlack = black;
  this->isWhite = white;


  Color otherPlayer = nextPlayer == Color::Black ? Color::White : Color::Black;

  auto &nextPossibleMoves = nextPlayer == Color::Black ? nextPossibleMovesBlack : nextPossibleMovesWhite;
  auto &nextPossibleMovesOther = nextPlayer == Color::White ? nextPossibleMovesBlack : nextPossibleMovesWhite;

  nextMoveColor = nextPlayer;
  this->updatePossibleMoves(Color::Black);
  this->updatePossibleMoves(Color::White);
  if (nextPossibleMoves.empty()) {
    nextMoveColor = otherPlayer;
    if (nextPossibleMovesOther.empty()) {
      nextMoveColor = Color::Neither;
    }
  }
}
