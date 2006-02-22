#include "propertysliderwidget.h"
#include <QSlider>

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
	setupUi( this );

	// add all properties
	combo->addItem( i18n( "Mass" ), "mass" );
	combo->addItem( i18n( "Density" ), "boilingpoint" );
	
	connect( slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotValueChanged( int ) ) );
 	connect( combo, SIGNAL( textChanged( QString ) ),
 	         this, SLOT( slotSliderKindChanged( QString ) ) );
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

void PropertySliderWidget::slotSliderKindChanged( QString kind )
{
}

#include "propertysliderwidget.moc"
