#ifndef ELEMENTBUTTON_H
#define ELEMENTBUTTON_H

#include <qbutton.h>

class ElementButton : public QButton
{
  Q_OBJECT

  public:
    ElementButton ( QWidget *parent, const char* name=0 );
  protected:
    void  drawButton( QPainter * );
    virtual void  drawButtonLabel( QPainter * ) = 0;
};

#endif

