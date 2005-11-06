#include "propertysliderwidget.h"

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
}

void PropertySliderWidget::setValues( QList<QVariant*> list )
{
	m_list = list;
}
