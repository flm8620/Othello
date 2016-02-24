#ifndef GENETICHOST_H
#define GENETICHOST_H

#include "GameState.h"
#include "OthelloAI.h"
#include "GameJudge.h"
class GeneticHost
{
  std::vector<OthelloAI> zoo;
  std::vector<int> zooScores;
  const int N;
public:
  GeneticHost(int Nsize);
  void startEvolution(int animalSize, int generation);
  OthelloAI mutation(const OthelloAI& ai);
  OthelloAI crossover(const OthelloAI& ai1,const OthelloAI& ai2);
};

#endif // GENETICHOST_H
