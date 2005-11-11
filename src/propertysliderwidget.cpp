#include "propertysliderwidget.h"
#include <QSlider>

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
	setupUi( this );
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

void TimeSliderWidget::slotValueChanged( int value )
{
	emit valueHasChanged( value );
}

TimeSliderWidget::TimeSliderWidget( QWidget * parent )
	: PropertySliderWidget( parent )
{
}

TimeSliderWidget::~TimeSliderWidget()
{}

int TimeSliderWidget::value() const
{
	return time_slider->value();
}

#include "propertysliderwidget.moc"
