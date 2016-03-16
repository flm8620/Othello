#include "genetichost.h"
#include <cassert>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <list>
#include "tools.h"
#include <random>
using namespace std;

extern mt19937 generator;


GeneticHost::GeneticHost(int Nsize) : N(Nsize) {

}

vector<int> rouletteWheelSelection(vector<double> fitness, int selectNum){
  list<pair<int,double> > fitStack;
  for(int i=0;i<fitness.size();i++){
    fitStack.push_back(make_pair(i,fitness[i]));
  }
  double total=0;
  for(auto a : fitness) total+=a;

  vector<int> selection;
  for(int i=0;i<selectNum;i++){
    uniform_real_distribution<double> randReal(0,total);
    double random = randReal(generator);
    double sum=0;
    for(auto it = fitStack.begin();it!=fitStack.end();it++){
      sum+=it->second;
      if(sum>random){
        selection.push_back(it->first);
        total-=it->second;
        fitStack.erase(it);
        break;
      }
    }
  }
  return selection;
}

void GeneticHost::startEvolution(int animalSize, int generation, bool loadAiFile, string aiFile,string recordFile,double roundTime) {
  //create random AIs
  zoo.clear();
  if (loadAiFile) {
    clearAllAi();
    this->zoo = readAIFile(aiFile,N);
  } else {
    for (int i = 0; i < animalSize; i++) {
      this->zoo.push_back(new OthelloAI(N));
    }
  }

  GameJudge judge;

  for (int k = 0; k < generation; k++) {
    zooScores.resize(zoo.size(), 0);
    vector<int> winCount(zoo.size(), 0);
    vector<int> drawCount(zoo.size(), 0);
    int count = 0;
    for (int i = 0; i < zoo.size(); i++) {
      for (int j = 0; j < zoo.size(); j++) {
        if (i == j)continue;
        count++;
        cout << "AI_" << zoo[i]->getID() << " vs AI_" << zoo[j]->getID() << endl;
        int score = judge.PlayAGame_getScore(*zoo[i], *zoo[j], N, roundTime, false);
        zooScores[i] += score;
        zooScores[j] -= score;
        if (score > 0) {
          winCount[i]++;
          //cout<<"AI_"<<zoo[i]->getID()<<" win, score: "<<score<<endl;
        } else if (score < 0) {
          winCount[j]++;
          //cout<<"AI_"<<zoo[j]->getID()<<" win, score: "<<-score<<endl;
        } else {
          drawCount[i]++;
          drawCount[j]++;
          //cout<<"Draw"<<endl;
        }
      }
      cout << double(count) / (zoo.size() * (zoo.size() - 1)) * 100 << "% finished" << endl;
    }
    //index of AIs to sort (0,1,2,3,4,5...)
    vector<int> index(zoo.size());
    for (int i = 0; i < zoo.size(); i++)index[i] = i;
    //sort to get good AIs
    sort(index.begin(), index.end(), [&](int a, int b) {
      return winCount[a] + drawCount[a]/2.0 > winCount[b] + drawCount[b]/2.0;
    });

    //output
    ofstream file(recordFile, ios::app);
    file << "Generation " << k << endl;
    cout << "5 Best AIs:" << endl;
    file << "5 Best AIs:" << endl;
    for (int i = 0; i < min(5, (int) zoo.size()); i++) {
      cout << "AI " << zoo[index[i]]->getID();
      cout << "\tWin: " << winCount[index[i]] << "/" << (zoo.size() - 1) * 2;
      cout << "\tDraw: " << drawCount[index[i]] << "/" << (zoo.size() - 1) * 2 << endl;
      file << "AI " << zoo[index[i]]->getID();
      file << "\tWin: " << winCount[index[i]] << "/" << (zoo.size() - 1) * 2;
      file << "\tDraw: " << drawCount[index[i]] << "/" << (zoo.size() - 1) * 2 << endl;
    }
    cout << "Best DiskSquare:" << endl;
    auto s = zoo[index[0]]->getDiskSquare();
    s.printOut();

    for (int i = 0; i < N / 2; i++) {
      for (int j = 0; j < i; j++) file << setw(10) << ' ';
      for (int j = i; j < N / 2; j++) {
        file << setw(10) << (int) s.scores[i * N + j];
      }
      file << endl;
    }
    file << "Best Weight:" << endl;
    auto sw = zoo[index[0]]->getScoreWeight();
    file << "MiddleTime:" << setw(7) << sw.middleTime << '\t'
         << "EndTime:" << setw(7) << sw.endTime << '\t'
         << "Mobility_begin_middle:" << sw.mobility_begin << '\t' << sw.mobility_middle << '\t'
         << "Position_begin_middle:" << sw.position_begin << '\t' << sw.position_middle;
    file << endl;

    file.close();

    //save AI
    vector<OthelloAI *> sortedZoo;
    vector<int> sortedWinCount;
    vector<int> sortedDrawCount;
    vector<double> fitness;
    for (int i = 0; i < index.size(); i++) {
      sortedZoo.push_back(zoo[index[i]]);
      sortedWinCount.push_back(winCount[index[i]]);
      sortedDrawCount.push_back(drawCount[index[i]]);
      fitness.push_back(sortedWinCount[i]+sortedDrawCount[i]/2.0);
    }
    zoo = sortedZoo;
    saveAIFile(zoo,aiFile,N);

    //from now zoo is sorted by performance

    vector<OthelloAI *> newZoo;
    //we take the 1/3 best AIs with random, throw the 2/3 (always keep the best one)
    vector<int> selection = rouletteWheelSelection(fitness,zoo.size()/3);
    bool bestOneIsKept=false;
    for(auto index : selection){
      if(index == 0) bestOneIsKept=true;
      newZoo.push_back(new OthelloAI(*(zoo[index])));
    }
    if(!bestOneIsKept) newZoo.push_back(new OthelloAI(*(newZoo[0])));

    for (auto z : zoo)
      delete z;
    if (newZoo.size() < 3)
      throw invalid_argument("too few animals");

    zoo.clear();
    for (auto animal : newZoo) {
      zoo.push_back(animal);
      //mutate every AI to create a new one
      zoo.push_back(mutation(animal));
    }

    uniform_int_distribution<int> randi(0, newZoo.size() - 1);
    //marriage between AIs randomly and have babies
    while (zoo.size() < animalSize-2) {
      int i = randi(generator);
      int j = randi(generator);
      if (i == j) continue;
      zoo.push_back(crossover(newZoo[i], newZoo[j]));
    }
    zoo.push_back(new OthelloAI(8));
    zoo.push_back(new OthelloAI(8));
  }

}

OthelloAI *GeneticHost::mutation(const OthelloAI *ai) {
  return ai->createNewAIByMutation();
}

OthelloAI *GeneticHost::crossover(const OthelloAI *ai1, const OthelloAI *ai2) {
  return ai1->createNewAIByCrossOver(ai2);
}

