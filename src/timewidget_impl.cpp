#include "timewidget_impl.h"

#include <qlayout.h>
#include <qsizepolicy.h>
#include <qslider.h>
#include <qtextedit.h>

#include <knuminput.h>

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
	text->hide();
	verticalSpacer->changeSize( 21, 5, QSizePolicy::Fixed, QSizePolicy::Expanding );
}

#include "timewidget_impl.moc"
