#include <iostream>
#include "GameState.h"
#include "GameJudge.h"
#include "OthelloAI.h"
#include "genetichost.h"
using namespace std;
int main() {
  GeneticHost host(8);
  host.startEvolution(10,10);
  return 0;
}
