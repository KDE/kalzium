#include "propertysliderwidget.h"
#include "propertysliderwidgetbase.h"
#include <QSlider>

PropertySliderWidget::PropertySliderWidget( QWidget * parent )
	: QWidget( parent )
{
	setupUi( this );

	// add all properties
	sliderKind->addItem( i18n( "Mass" ), "mass" );
	sliderKind->addItem( i18n( "Boiling Point" ), "boilingpoint" );
	sliderKind->addItem( i18n( "Melting Point" ), "meltingpoint" );
	
	connect( time_slider, SIGNAL( valueChanged( int ) ),
	         this, SLOT( slotValueChanged( int ) ) );
 	connect( sliderKind, SIGNAL( textChanged( QString ) ),
 	         this, SLOT( slotSliderKindChanged( QString ) ) );
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

void PropertySliderWidget::slotSliderKindChanged( QString kind )
{
/*	switch( kind )
	{
		case "mass":
			break;
		case "boilingpoint":
			break;
		case "meltingpoint":
			break;
	}*/
	// change the QWhatThis
	// change the QLabel description
	
	// change the QLabel unit
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
