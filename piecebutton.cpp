#include "piecebutton.h"

IconLoader iconLoader;

PieceButton::PieceButton(int i, int j, QWidget *parent) : QPushButton(parent),i(i),j(j)
{
  iconLoader.loadImage();
  this->setColor(ButtonColor::b_Empty);
  this->setStyleSheet("QPushButton { border: none; }");
  this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  this->setMinimumHeight(40);
  this->setMinimumWidth(40);
  this->setIconSize(QSize(40,40));
  connect(this,&PieceButton::clicked,this,&PieceButton::sendPosition);
}

void PieceButton::setColor(ButtonColor c)
{
  this->color=c;
  switch(c){
    case ButtonColor::b_White:
      this->setIcon(QIcon(*iconLoader.whiteImage));
      break;
    case ButtonColor::b_Black:
      this->setIcon(QIcon(*iconLoader.blackImage));
      break;
    case ButtonColor::b_Empty:
      this->setIcon(QIcon(*iconLoader.emptyImage));
      break;
    case ButtonColor::b_WhitePossible:
      this->setIcon(QIcon(*iconLoader.whitePossibleImage));
      break;
    case ButtonColor::b_BlackPossible:
      this->setIcon(QIcon(*iconLoader.blackPossibleImage));
      break;
    case ButtonColor::b_WhiteLast:
      this->setIcon(QIcon(*iconLoader.whiteLastImage));
      break;
    case ButtonColor::b_BlackLast:
      this->setIcon(QIcon(*iconLoader.blackLastImage));
      break;
  }
}


void IconLoader::loadImage()
{
  static bool loaded=false;
  if(loaded) return;
  loaded=true;
  emptyImage          =new QPixmap(":/myIcons/empty.png");
  whiteImage          =new QPixmap(":/myIcons/white.png");
  blackImage          =new QPixmap(":/myIcons/black.png");
  whitePossibleImage  =new QPixmap(":/myIcons/whitePossible.png");
  blackPossibleImage  =new QPixmap(":/myIcons/blackPossible.png");
  whiteLastImage      =new QPixmap(":/myIcons/whiteLast.png");
  blackLastImage      =new QPixmap(":/myIcons/blackLast.png");
}

IconLoader::~IconLoader()
{
  delete emptyImage         ;
  delete whiteImage         ;
  delete blackImage         ;
  delete whitePossibleImage ;
  delete blackPossibleImage ;
  delete whiteLastImage     ;
  delete blackLastImage     ;
}
