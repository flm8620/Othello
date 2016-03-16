#ifndef GAMEWORKER_H
#define GAMEWORKER_H

#include <QObject>
#include "GameState.h"
#include "OthelloAI.h"
class GameWorker : public QObject
{
  Q_OBJECT
  OthelloAI ai;
  Color humanColor;
  Color aiColor;
  GameState gs;
  double thinkTime;
  void letAIplay();
public:
  explicit GameWorker(int Nsize, OthelloAI ai,QObject *parent = 0);
  void setAI(const OthelloAI& ai){this->ai=ai;};
signals:
  void AIplayed(int i,int j);
public slots:
  void startNewGame(Color humanColor, double thinkTime);
  void humanPlayed(int i,int j);
  void setThinkTime(double thinkTime){this->thinkTime=thinkTime;}
};

#endif // GAMEWORKER_H
