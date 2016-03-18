#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QVector>
#include <QThread>
#include <QLabel>
#include <QDoubleSpinBox>
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
  QDoubleSpinBox* spinBox;
  int gameSeriesID;
  void updateButtons(Color lastColor, int i, int j);
public:
  explicit GameWindow(std::string aiFile,int Nsize,QWidget *parent = 0);
  void endGame();
  ~GameWindow();
  void newGameAsBlack();
  void newGameAsWhite();
signals:
  void startNewGame(Color humanColor, double thinkTime, int gameSeriesID);
  void humanPlayed(int i,int j,int gameSeriesID);

public slots:
  void AIplayed(int i,int j,int gameSeriesID);
  void buttonClicked(int i,int j);

};

#endif // GAMEWINDOW_H
