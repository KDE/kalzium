#include "timewidget_impl.h"

#include <qslider.h>
#include <qtextedit.h>
#include <qvaluelist.h>

#include <kdebug.h>
#include <knuminput.h>
#include <klocale.h>

#include <math.h>

#include "element.h"
#include "kalziumdataobject.h"

TimeWidgetIMPL::TimeWidgetIMPL( QWidget *parent, const char* name )
	: TimeWidget( parent, name )
{
	text->setAlignment( text->alignment() | Qt::WordBreak );
	text->setTextFormat( Qt::RichText );
	text->setReadOnly( true );
	text->setPaletteBackgroundColor( paletteBackgroundColor() );
	text->setFrameStyle( QFrame::NoFrame );
}

void TimeWidgetIMPL::slotChanged( int value )
{
	( void )value;
	text->hide(  );
}

#include "timewidget_impl.moc"
