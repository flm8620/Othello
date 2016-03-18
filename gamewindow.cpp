#include "gamewindow.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>
#include <QMessageBox>

#include "tools.h"
using namespace std;
void GameWindow::updateButtons(Color lastColor,int i,int j)
{
  vector<bool> isWhite = gs.getWhitePosition();
  vector<bool> isBlack = gs.getBlackPosition();
  for(int i=0;i<Nsize*Nsize;i++){
    if(isWhite[i])
      this->buttons[i]->setColor(ButtonColor::b_White);
    else if(isBlack[i])
      this->buttons[i]->setColor(ButtonColor::b_Black);
    else
      this->buttons[i]->setColor(ButtonColor::b_Empty);
  }
  set<pair<int,int>> moves = gs.getPossibleMovesForNextPlayer();
  ButtonColor c = gs.nextPlayer()==Color::Black ? ButtonColor::b_BlackPossible:ButtonColor::b_WhitePossible;
  for(auto m : moves){
    int index = m.first*Nsize +m.second;
    this->buttons[index]->setColor(c);
  }
  if(lastColor==Color::Black){
    this->buttons[i*Nsize+j]->setColor(ButtonColor::b_BlackLast);
  }
  if(lastColor==Color::White){
    this->buttons[i*Nsize+j]->setColor(ButtonColor::b_WhiteLast);
  }
}

GameWindow::GameWindow(string aiFile, int Nsize, QWidget *parent) : QWidget(parent),Nsize(Nsize), gs(Nsize),worker(Nsize,OthelloAI(Nsize))
{
  auto AIs = readAIFile(aiFile,Nsize);
  worker.setAI(*AIs[0]);//best
  this->gameSeriesID = 0;
  auto boardLayout = new QGridLayout;
  for(int i=0;i<Nsize;i++){
    for(int j=0;j<Nsize;j++){
      buttons.push_back(new PieceButton(i,j,this));
      boardLayout->addWidget(buttons.back(),i,j);
      connect(buttons.back(),&PieceButton::clickedWithPos,this,&GameWindow::buttonClicked);
    }
  }
  boardLayout->setHorizontalSpacing(2);
  boardLayout->setVerticalSpacing(2);

  auto buttonLayout = new QHBoxLayout;
  auto playWhite = new QPushButton("Play as white");
  auto playBlack = new QPushButton("Play as black");
  this->spinBox = new QDoubleSpinBox();
  this->spinBox->setRange(0.1,10);
  this->spinBox->setValue(1.0);
  buttonLayout->addWidget(playWhite);
  buttonLayout->addWidget(playBlack);

  auto timeLayout = new QHBoxLayout;
  timeLayout->addWidget(new QLabel("Thinking Time(second):"));
  timeLayout->addWidget(this->spinBox);
  connect(playWhite,&PieceButton::clicked,this,&GameWindow::newGameAsWhite);
  connect(playBlack,&PieceButton::clicked,this,&GameWindow::newGameAsBlack);

  textLabel = new QLabel;

  auto totalLayout = new QVBoxLayout;
  totalLayout->addWidget(textLabel);
  totalLayout->addLayout(boardLayout);
  totalLayout->addLayout(buttonLayout);
  totalLayout->addLayout(timeLayout);
  this->setLayout(totalLayout);

  //Worker
  connect(this,&GameWindow::startNewGame,&worker,&GameWorker::startNewGame);
  connect(this,&GameWindow::humanPlayed,&worker,&GameWorker::humanPlayed);
  connect(&worker,&GameWorker::AIplayed,this,&GameWindow::AIplayed);

  this->worker.moveToThread(&this->workerThread);
  this->workerThread.start();
}

void GameWindow::endGame()
{
  if(!gs.gameIsEnd())
    throw logic_error("Game is not end");
  int humanS = gs.pieceCount(humanColor);
  int aiS = gs.pieceCount(aiColor);
  if(humanS>aiS){
    this->textLabel->setText(QString("You WIN! score: black: %1 white: %2").arg(gs.pieceCount(Color::Black)).arg(gs.pieceCount(Color::White)));
  }else if (humanS<aiS){
    this->textLabel->setText(QString("Computer WIN! score: black: %1 white: %2").arg(gs.pieceCount(Color::Black)).arg(gs.pieceCount(Color::White)));
  }else{
    this->textLabel->setText(QString("DRAW! score: black: %1 white: %2").arg(gs.pieceCount(Color::Black)).arg(gs.pieceCount(Color::White)));
  }
}

GameWindow::~GameWindow()
{
  this->workerThread.exit();
}

void GameWindow::AIplayed(int i, int j, int gameSeriesID)
{
  if(gameSeriesID!=this->gameSeriesID)return;
  gs.addPiece(i,j,aiColor);
  this->updateButtons(aiColor,i,j);
  auto s = QString("Computer played at %1%2").arg(char('A'+j)).arg(i+1);
  if(gs.nextPlayer()==this->aiColor){
    s = QString("You have no move, Computer thinking...");
    QMessageBox::information(this,"Opss!","You have no move, computer continue");
  }
  this->textLabel->setText(s);
  if(gs.gameIsEnd())
    endGame();
}

void GameWindow::buttonClicked(int i, int j)
{
  if(gs.nextPlayer()!=humanColor)return;
  set<pair<int,int>> moves = gs.getPossibleMovesForNextPlayer();
  if(moves.find(make_pair(i,j))!=moves.end()){
    gs.addPiece(i,j,humanColor);
    updateButtons(humanColor,i,j);
    emit humanPlayed(i,j,this->gameSeriesID);
    if(gs.nextPlayer()==this->humanColor){
      this->textLabel->setText(QString("Computer have to pass, you turn:"));
    }else if(gs.nextPlayer()==this->aiColor){
      this->textLabel->setText(QString("Computer thinking"));
    }
  }
  if(gs.gameIsEnd())
    endGame();
}

void GameWindow::newGameAsBlack()
{
  this->humanColor=Color::Black;
  this->aiColor = Color::White;
  this->gs.restartGame();
  this->updateButtons(Color::Neither,-1,-1);
  gameSeriesID++;
  emit startNewGame(humanColor,this->spinBox->value(),gameSeriesID);
}

void GameWindow::newGameAsWhite()
{
  this->humanColor=Color::White;
  this->aiColor = Color::Black;
  this->gs.restartGame();
  this->updateButtons(Color::Neither,-1,-1);
  this->textLabel->setText(QString("Computer thinking"));
  gameSeriesID++;
  emit startNewGame(humanColor,this->spinBox->value(),gameSeriesID);
}

