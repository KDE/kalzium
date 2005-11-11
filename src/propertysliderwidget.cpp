#include "propertysliderwidget.h"
#include "propertysliderwidgetbase.h"
#include <QSlider>

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
	setupUi( this );
	
	connect( time_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotValueChanged( int ) ) );
	connect( Number1, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotValueChanged( int ) ) );
	connect( time_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( setValueText( int ) ) );
}

void PropertySliderWidget::setValueText( int value )
{
	valueLabel->setText( QString::number( value ) );
}

PropertySliderWidget::~PropertySliderWidget()
{
}

void PropertySliderWidget::setValues( QList<int> list )
{
	m_list = list;
}

void PropertySliderWidget::valueChanged( int value )
{
	m_value = value;
}

void PropertySliderWidget::slotValueChanged( int value )
{
	emit valueHasChanged( value );
}

/*********************************************************/

TimeSliderWidget::TimeSliderWidget( QWidget * parent )
	: PropertySliderWidget( parent )
{
}


void TimeSliderWidget::slotValueChanged( int value )
{
	emit valueHasChanged( value );
}

TimeSliderWidget::~TimeSliderWidget()
{}

/*** DOUBLE *****************************************************/
DoubleSliderWidget::DoubleSliderWidget( QWidget* parent )
	: PropertySliderWidget( parent )
{
}

DoubleSliderWidget::~DoubleSliderWidget(){}

void DoubleSliderWidget::setValueText( int value )
{
	double realValue = value / m_digits;

	valueLabel->setText( QString::number( realValue ) );
}

void DoubleSliderWidget::slotValueChanged( int value )
{
	emit valueHasChanged( value, m_digits );
}

#include "propertysliderwidget.moc"
