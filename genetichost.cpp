#include "genetichost.h"
#include "GameJudge.h"
#include <iostream>
#include <chrono>
#include <random>
#include <cassert>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

auto seed2 = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 generator2(seed2);
string outputFile = srcPath "/record.txt";
void GeneticHost::saveAIToFile(string file)
{
  ofstream f(file);
  f<<zoo.size()<<endl;
  for(auto p : zoo){
    f<<p->getID()<<endl;
    auto cbs = p->getChessBoardScore();
    auto triangle = cbs.getTriangleFormat();
    int k=0;
    for(int i=0;i<N/2;i++){
      for(int j=0;j<i;j++) f<<setw(10)<<' ';
      for(int j=i;j<N/2;j++){
        f<<setw(10)<<triangle[k];
        k++;
      }
      f<<endl;
    }
    auto l=p->getLambdas();
    for(auto la:l)f<<la<<'\t';
    f<<endl;
  }
  f.close();
}

void GeneticHost::loadAIFromFile(string file)
{
  ifstream f(file);
  clearAllAi();
  int n;
  f>>n;
  for(int k=0;k<n;k++){
    auto p=new OthelloAI(N);
    zoo.push_back(p);
    int id;
    f>>id;
    p->setID(id);
    ChessBoardScore cbs(N);
    auto triangle = cbs.getTriangleFormat();
    for(auto &t : triangle) f>>t;
    cbs.scores=ChessBoardScore::triangleFormatToFullFormat(triangle,cbs.N);
    p->setChessBoardScore(cbs);

    vector<double> lambdas(N*N);
    for(auto& l : lambdas) f>>l;
    p->setLambdas(lambdas);
  }
  if(f.fail())
    throw invalid_argument("file read failed!");
  f.close();
}

GeneticHost::GeneticHost(int Nsize):N(Nsize)
{

}

void GeneticHost::startEvolution(int animalSize, int generation, bool loadAiFile, string aiFile)
{
  //create random AIs
  zoo.clear();
  if(loadAiFile){
    this->loadAIFromFile(aiFile);
  }else{
    for(int i=0;i<animalSize;i++){
      this->zoo.push_back(new OthelloAI(N));
    }
  }

  GameJudge judge;

  for(int k=0;k<generation;k++){
    zooScores.resize(zoo.size(),0);
    vector<int> winCount(zoo.size(),0);
    vector<int> drawCount(zoo.size(),0);
    int count=0;
    for(int i=0;i<zoo.size();i++){
      for(int j=0;j<zoo.size();j++){
        if(i==j)continue;
        count++;
        cout<<"AI_"<<zoo[i]->getID()<<" vs AI_"<<zoo[j]->getID()<<endl;
        int score = judge.PlayAGame_getScore(*zoo[i],*zoo[j],N,0.01,false);
        zooScores[i]+=score;
        zooScores[j]-=score;
        if(score>0){
          winCount[i]++;
          //cout<<"AI_"<<zoo[i]->getID()<<" win, score: "<<score<<endl;
        }else if(score<0){
          winCount[j]++;
          //cout<<"AI_"<<zoo[j]->getID()<<" win, score: "<<-score<<endl;
        }else{
          drawCount[i]++;drawCount[j]++;
          //cout<<"Draw"<<endl;
        }
      }
      cout<<double(count)/(zoo.size()*(zoo.size()-1))*100<<"% finished"<<endl;
    }
    //index of AIs to sort (0,1,2,3,4,5...)
    vector<int> index(zoo.size());
    for(int i=0;i<zoo.size();i++)index[i]=i;
    //sort to get good AIs
    sort(index.begin(),index.end(),[&](int a,int b){
      return winCount[a]>winCount[b];
    });

    //output
    ofstream file(outputFile,ios::app);
    file<<"Generation "<<k<<endl;
    cout<<"5 Best AIs:"<<endl;
    file<<"5 Best AIs:"<<endl;
    for(int i=0;i<min(5,(int)zoo.size());i++){
      cout<<"AI "<<zoo[index[i]]->getID();
      cout<<"\tWin: "<<winCount[index[i]]<<"/"<<(zoo.size()-1)*2;
      cout<<"\tDraw: "<<drawCount[index[i]]<<"/"<<(zoo.size()-1)*2<<endl;
      file<<"AI "<<zoo[index[i]]->getID();
      file<<"\tWin: "<<winCount[index[i]]<<"/"<<(zoo.size()-1)*2;
      file<<"\tDraw: "<<drawCount[index[i]]<<"/"<<(zoo.size()-1)*2<<endl;
    }
    cout<<"Best ChessBoardScore:"<<endl;
    auto s = zoo[index[0]]->getChessBoardScore();
    auto l = zoo[index[0]]->getLambdas();
    s.printOut();

    for(int i=0;i<N/2;i++){
      for(int j=0;j<i;j++) file<<setw(10)<<' ';
      for(int j=i;j<N/2;j++){
        file<<setw(10)<<(int)s.scores[i*N+j]<<'\t';
      }
      file<<endl;
    }
    file<<"Best lambdas:"<<endl;
    for(auto la : l)file<<la<<'\t';
    file<<endl;

    file.close();

    //save AI
    vector<OthelloAI*> sortedZoo;
    for(int i=0;i<index.size();i++){
      sortedZoo.push_back(zoo[index[i]]);
    }
    zoo=sortedZoo;
    this->saveAIToFile(aiFile);

    //from now zoo is sorted by performance

    std::vector<OthelloAI*> newZoo;
    //we take the 1/3 best AIs, throw the 2/3
    for(int i=0;i<zoo.size();i++){
      if(i<zoo.size()/4)
        newZoo.push_back(zoo[i]);
      else
        delete zoo[i];
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

OthelloAI *GeneticHost::mutation(const OthelloAI *ai)
{

  int M=N/2;
  int MM=M*(1+M)/2;
  uniform_int_distribution<int> randi(0,MM-1);
  uniform_real_distribution<double> randReal(-ChessBoardScore::MAXSCORE,ChessBoardScore::MAXSCORE);
  auto triangle = ai->getChessBoardScore().getTriangleFormat();

  //pick one element in ChessBoardScore and modify it to a random real number
  triangle[randi(generator2)]=randReal(generator2);
  ChessBoardScore cbs(N);
  cbs.scores=ChessBoardScore::triangleFormatToFullFormat(triangle,N);


  //lambda:
  uniform_real_distribution<double> randRatio(0.8,1.25);
  double ratio = randRatio(generator2);
  vector<double> lambdas = ai->getLambdas();
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

  OthelloAI *newAI = new OthelloAI(N);
  newAI->setChessBoardScore(cbs);
  newAI->setLambdas(lambdas);

  return newAI;
}

OthelloAI *GeneticHost::crossover(const OthelloAI *ai1, const OthelloAI *ai2)
{

  ChessBoardScore score(N);
  auto triangle1 = ai1->getChessBoardScore().getTriangleFormat();
  auto triangle2 = ai2->getChessBoardScore().getTriangleFormat();
  uniform_int_distribution<int> randi(0,1);
  auto triangle = triangle1;
  //chessBoardScore is just like DNA
  //we take parents DNA randomly
  for(int i=0;i<triangle.size();i++){
    if(randi(generator2)==1){
      triangle[i]=triangle1[i];
    }else{
      triangle[i]=triangle2[i];
    }
  }

  score.scores=ChessBoardScore::triangleFormatToFullFormat(triangle,N);

  //TODO:
  //take mean of lambdas.... maybe to be improved later

  vector<double> lambdas1 = ai1->getLambdas();
  vector<double> lambdas2 = ai2->getLambdas();
  for(int i=0;i<lambdas1.size();i++){
    lambdas1[i]=(lambdas1[i]+lambdas2[i])/2;
  }

  OthelloAI* newAI = new OthelloAI(N);
  newAI->setChessBoardScore(score);
  newAI->setLambdas(lambdas1);

  return newAI;

}

