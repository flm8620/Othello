#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QVector>
#include <QThread>
#include <QLabel>
#include "GameState.h"
#include "piecebutton.h"
#include "gameworker.h"
#include <string>



class GameWindow : public QWidget
{
  Q_OBJECT
  QVector<PieceButton*> buttons;
  const int Nsize;
  GameState gs;
  Color humanColor;
  Color aiColor;
  GameWorker worker;
  QThread workerThread;
  QLabel *textLabel;
  double thinkTime;
  void updateButtons(Color lastColor, int i, int j);
public:
  explicit GameWindow(std::string aiFile,int Nsize,QWidget *parent = 0);
  void endGame();
  ~GameWindow();

signals:
  void startNewGame(Color humanColor, double thinkTime);
  void humanPlayed(int i,int j);

public slots:
  void AIplayed(int i,int j);
  void buttonClicked(int i,int j);
  void newGameAsBlack();
  void newGameAsWhite();
};

#endif // GAMEWINDOW_H
