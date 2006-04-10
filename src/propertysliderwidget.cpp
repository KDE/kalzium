#include "propertysliderwidget.h"
#include <QSlider>

#include "kalziumgradienttype.h"

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
	setupUi( this );

	// add all properties
	QStringList list = KalziumGradientTypeFactory::instance()->gradients();
	foreach( QString s, list )
		combo->addItem( s );

	connect( slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotValueChanged( int ) ) );
 	connect( combo, SIGNAL( activated( int ) ),
 	         this, SLOT( slotSliderKindChanged( int ) ) );
	connect( slider, SIGNAL( valueChanged( int ) ),
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
	valueChanged( value );

	emit valueHasChanged( m_value );
	emit valueHasChanged( m_type, m_value );
}

void PropertySliderWidget::slotSliderKindChanged( int index )
{
	KalziumGradientType *mytype = KalziumGradientTypeFactory::instance()->build( index );
	int min = ( int )mytype->minValue();
	int max = ( int )mytype->maxValue();
	slider->setMaximum(min );
	slider->setMinimum( max );

	text->setText( i18n( "min: %1, max: %2", min, max ) );
}

#include "propertysliderwidget.moc"
