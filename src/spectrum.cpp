#include "spectrum.h"

SpectrumWidget::SpectrumWidget( QWidget* parent, const char* name )
	: QWidget( parent,name )
{
}

SpectrumWidget::~SpectrumWidget(){}

void SpectrumWidget::paintEvent( QPaintEvent *e )
{
	QPainter p;
	p.begin( this );
	p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() ); 
	drawLines(  &p );
}

void SpectrumWidget::drawLines( QPainter *p )
{
	//the spectrum goes from about 780nm to about 400nm
	//750 is a dark red 
	//660 yellow
	//580 green
	//500 light blue
	//400 dark blue

	QImage img = KImageEffect::gradient ( QSize( width(), height() ), Qt::blue, Qt::red, 
										  KImageEffect::HorizontalGradient );
	QPixmap pm( img );

	p->drawPixmap( width(), height(), pm );
}

QColor SpectrumWidget::linecolor( double spectrum )
{
	QColor c( 128,128,128 );
	return c;
}
