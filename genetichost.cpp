#include "genetichost.h"
#include "GameJudge.h"
#include <iostream>
#include <chrono>
#include <random>
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;

auto seed2 = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 generator2(seed2);

GeneticHost::GeneticHost(int Nsize):N(Nsize)
{

}

void GeneticHost::startEvolution(int animalSize, int generation)
{
  //create random AIs
  for(int i=0;i<animalSize;i++){
    this->zoo.push_back(OthelloAI(N));
  }
  GameJudge judge;

  for(int k=0;k<generation;k++){
    zooScores.resize(zoo.size(),0);
    for(int i=0;i<zoo.size();i++){
      for(int j=i+1;j<zoo.size();j++){
        cout<<"AI_"<<zoo[i].getID()<<" vs AI_"<<zoo[j].getID()<<endl;
        int score = judge.PlayAGame_getScore(zoo[i],zoo[j],N,false);
        zooScores[i]+=score;
        zooScores[j]-=score;
        if(score>0){
          cout<<"AI_"<<zoo[i].getID()<<" win, score: "<<score<<endl;
        }else if(score<0){
          cout<<"AI_"<<zoo[j].getID()<<" win, score: "<<-score<<endl;
        }else{
          cout<<"Draw"<<endl;
        }
        cout<<endl;
      }
    }
    //index of AIs to sort (0,1,2,3,4,5...)
    vector<int> index(zoo.size());
    for(int i=0;i<zoo.size();i++)index[i]=i;
    //sort to get good AIs
    sort(index.begin(),index.end(),[&](int a,int b){
      return zooScores[a]>zooScores[b];
    });

    cout<<"Best ChessBoardScore for now:"<<endl;
    zoo[index[0]].getChessBoardScore().printOut();

    std::vector<OthelloAI> newZoo;
    //we take the 1/3 best AIs, throw the 2/3
    for(int i=0;i<animalSize/3;i++){
      newZoo.push_back(zoo[index[i]]);
    }
    if(newZoo.size()<3)
      throw invalid_argument("too few animals");

    zoo.clear();
    for(auto animal : newZoo){
      zoo.push_back(animal);
      //mutate every AI to create a new one
      zoo.push_back(mutation(animal));
    }

    uniform_int_distribution<int> randi(0,newZoo.size()-1);
    //marriage between AIs randomly and have babies
    while(zoo.size()<animalSize){
      int i=randi(generator2);
      int j=randi(generator2);
      if(i==j) continue;
      zoo.push_back(crossover(newZoo[i],newZoo[j]));
    }
  }
}

OthelloAI GeneticHost::mutation(const OthelloAI &ai)
{
  OthelloAI newAI(ai);
  uniform_int_distribution<int> randi(0,N-1);
  uniform_int_distribution<int> randj(0,N/2-1);
  uniform_real_distribution<double> randReal(-1000,1000);
  int i=randi(generator2);
  int j=randj(generator2);
  //pick one element in ChessBoardScore and modify it to a random real number
  newAI.modifyChessBoardScore(i,j,randReal(generator2));

  uniform_real_distribution<double> randRatio(0.8,1.25);
  double ratio = randRatio(generator2);
  vector<double> lambdas = newAI.getLambdas();
  //multiply all lambda by a random ratio between 80%-125%
  for(auto &v : lambdas) v*=ratio;

  double maxi = 0;
  for(auto &v : lambdas) maxi = max(v,maxi);

  //TODO:
  //modify lambdas randomly.... maybe to be improved later
  uniform_int_distribution<int> randTime(4,62);
  int time = randTime(generator2);
  double delta = lambdas[time]-lambdas[time+1];
  assert(delta>-1e-5);
  if(delta<-1e-5) throw logic_error("delta can't be negative");

  uniform_real_distribution<double> randDelta(-delta,max(delta,maxi/N));
  double modify=randDelta(generator2);
  for(int i=4;i<=time;i++) lambdas[i]+=modify;
  newAI.setLambdas(lambdas);

  return newAI;
}

OthelloAI GeneticHost::crossover(const OthelloAI &ai1, const OthelloAI &ai2)
{
  OthelloAI newAI(N);
  ChessBoardScore score(N);
  ChessBoardScore s1=ai1.getChessBoardScore();
  ChessBoardScore s2=ai2.getChessBoardScore();
  uniform_int_distribution<int> randi(0,1);

  //chessBoardScore is just like DNA
  //we take parents DNA randomly
  for(int i=0;i<N*N/2;i++){
    if(randi(generator2)==1){
      score.scores[i]=score.scores[N*N-i-1]=s1.scores[i];
    }else{
      score.scores[i]=score.scores[N*N-i-1]=s2.scores[i];
    }
  }

  //TODO:
  //take mean of lambdas.... maybe to be improved later

  vector<double> lambdas1 = ai1.getLambdas();
  vector<double> lambdas2 = ai2.getLambdas();
  for(int i=0;i<lambdas1.size();i++){
    lambdas1[i]=(lambdas1[i]+lambdas2[i])/2;
  }
  newAI.setChessBoardScore(score);
  newAI.setLambdas(lambdas1);

  return newAI;

}

