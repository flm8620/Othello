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
  void saveAIToFile(std::string file);
  void loadAIFromFile(std::string file);
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
      (int animalSize, int generation, bool loadAiFile = false, std::string aiFile = srcPath "/aiSave.txt");

  //mutate a AI to create a new one
  OthelloAI *mutation(const OthelloAI *ai);
  //two parents give birth to a new baby
  OthelloAI *crossover(const OthelloAI *ai1, const OthelloAI *ai2);
};

#endif // GENETICHOST_H
