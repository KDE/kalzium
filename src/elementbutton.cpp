#include "elementbutton.h"
#include "elementbutton.moc"

ElementButton::ElementButton ( QWidget *parent, const char* name)
  : QButton (parent, name)
{

}
void ElementButton::drawButton( QPainter *paint )
{
  drawButtonLabel ( paint );
  
}
