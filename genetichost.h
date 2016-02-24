#ifndef GENETICHOST_H
#define GENETICHOST_H

#include "GameState.h"
#include "OthelloAI.h"
#include "GameJudge.h"
class GeneticHost
{
  //AIs
  std::vector<OthelloAI> zoo;
  //score of AIs during one generation
  std::vector<int> zooScores;
  const int N;
public:
  GeneticHost(int Nsize);
  void startEvolution(int animalSize, int generation);

  //mutate a AI to create a new one
  OthelloAI mutation(const OthelloAI& ai);
  //two parents give birth to a new baby
  OthelloAI crossover(const OthelloAI& ai1,const OthelloAI& ai2);
};

#endif // GENETICHOST_H
