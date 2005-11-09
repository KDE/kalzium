#include "propertysliderwidget.h"

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
}

PropertySliderWidget::~PropertySliderWidget()
{
}

void PropertySliderWidget::setValues( QList<QVariant*> list )
{
	m_list = list;
}

#include "propertysliderwidget.moc"
