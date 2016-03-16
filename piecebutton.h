#ifndef PIECEBUTTON_H
#define PIECEBUTTON_H
#include <QPushButton>
#include <QPixmap>

struct IconLoader{
  QPixmap* emptyImage;
  QPixmap* whiteImage;
  QPixmap* blackImage;
  QPixmap* whitePossibleImage;
  QPixmap* blackPossibleImage;
  QPixmap* whiteLastImage;
  QPixmap* blackLastImage;
  void loadImage();
  ~IconLoader();
};

enum ButtonColor{
  b_White,b_Black,b_Empty,b_WhitePossible,b_BlackPossible,b_WhiteLast,b_BlackLast
};

class PieceButton : public QPushButton
{
  Q_OBJECT

  ButtonColor color;
  int i,j;
public:
  explicit PieceButton(int i,int j,QWidget *parent = 0);
  void setColor(ButtonColor c);
  ButtonColor getColor(){return color;}
  void loadPixmap();
  void sendPosition(){emit clickedWithPos(i,j);}
signals:
  void clickedWithPos(int i,int j);


};

#endif // PIECEBUTTON_H
