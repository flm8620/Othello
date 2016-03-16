#ifndef GENETICHOST_H
#define GENETICHOST_H

#include "GameState.h"
#include "OthelloAI.h"
#include "GameJudge.h"
#include <string>

#ifndef srcPath
#define srcPath "."
#endif

class GeneticHost {
  //AIs
  std::vector<OthelloAI *> zoo;
  //score of AIs during one generation
  std::vector<int> zooScores;
  const int N;
 public:
  GeneticHost(int Nsize);
  ~GeneticHost() {
    clearAllAi();
  }
  void clearAllAi() {
    for (auto p : zoo) delete p;
    zoo.clear();
    zooScores.clear();
  }

  void startEvolution
      (int animalSize, int generation, bool loadAiFile, std::string aiFile, std::string recordFile, double roundTime);

  //mutate a AI to create a new one
  OthelloAI *mutation(const OthelloAI *ai);
  //two parents give birth to a new baby
  OthelloAI *crossover(const OthelloAI *ai1, const OthelloAI *ai2);
};

std::vector<int> rouletteWheelSelection(std::vector<double> fitness, int selectNum);
#endif // GENETICHOST_H
