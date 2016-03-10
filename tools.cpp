#include "tools.h"
#include "GameState.h"
#include <fstream>
#include <iomanip>
using namespace std;
std::vector<OthelloAI *> readAIFile(std::string fileName, int Nsize)
{
  ifstream f(fileName);
  vector<OthelloAI*> zoo;
  int n;
  f >> n;
  for (int k = 0; k < n; k++) {
    auto p = new OthelloAI(Nsize);
    zoo.push_back(p);
    int id;
    f >> id;
    p->setID(id);
    DiskSquare ds(Nsize);
    auto triangle = ds.getTriangleFormat();
    for (auto &t : triangle) f >> t;
    ds.scores = DiskSquare::triangleFormatToFullFormat(triangle, ds.N);
    p->setDiskSquare(ds);

    auto sw = p->getScoreWeight();
    f >> sw.middleTime >> sw.endTime >> sw.mobility_begin
        >> sw.mobility_middle >> sw.position_begin >> sw.position_middle;
    p->setScoreWeight(sw);
  }
  if (f.fail()){
    for(auto z:zoo) delete z;
    throw invalid_argument("file read failed!");
  }
  f.close();
  return zoo;
}

void saveAIFile(const std::vector<OthelloAI *> &AIs, string fileName, int Nsize)
{
  ofstream f(fileName);
  f << AIs.size() << endl;
  for (auto p : AIs) {
    f << p->getID() << endl;
    auto ds = p->getDiskSquare();
    auto triangle = ds.getTriangleFormat();
    int k = 0;
    for (int i = 0; i < Nsize / 2; i++) {
      for (int j = 0; j < i; j++) f << setw(10) << ' ';
      for (int j = i; j < Nsize / 2; j++) {
        f << setw(10) << triangle[k];
        k++;
      }
      f << endl;
    }
    auto sw = p->getScoreWeight();
    f << sw.middleTime << '\t' << sw.endTime << '\t' << sw.mobility_begin
        << '\t' << sw.mobility_middle << '\t' << sw.position_begin << '\t' << sw.position_middle;
    f << endl;
  }
  f.close();
}
