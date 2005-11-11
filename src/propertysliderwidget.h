#ifndef PROPERTYSLIDERWIDGET_H
#define PROPERTYSLIDERWIDGET_H

#include <QWidget>
#include <QList>
#include <QVariant>

#include "timewidget.h"
#include "propertysliderwidgetbase.h"

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 *
 * This class is the baseclass of all slider-widgets which 
 * show or hide elements based on a certain property. For
 * example, the dateslider which shows which elements where
 * known at a certain date, is based on this class.
 */
class PropertySliderWidget : public QWidget, protected PropertySliderWidgetBase
{
	Q_OBJECT

	public:
		/**
		 * Constructor
		 * @param parent the parent widget
		 */
		PropertySliderWidget( QWidget* parent );

		/**
		 * Destructor
		 */
		virtual ~PropertySliderWidget();

		/**
		 * Set the list @p list as the internal valuelist
		 * @param list the list of the values of the property
		 */
		void setValues( QList<int> list );

		/**
		 * @return the currently selected value
		 */
		virtual int value() const{
			return time_slider->value();
		}

	private:
		int m_value;

	protected slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( int value );

	protected:
		QList<int> m_list;

	protected slots:
		/**
		 * emitted when the value changes
		 * @param the new value
		 */
		virtual void valueChanged( int value );

		virtual void setValueText( int value );

	signals:
		void valueHasChanged( int value );
};

class TimeSliderWidget : public PropertySliderWidget
{
	Q_OBJECT

	public:
		TimeSliderWidget( QWidget * parent );
		
		/**
		 * Destructor
		 */
		virtual ~TimeSliderWidget();
		
	private slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( int value );
};

class DoubleSliderWidget : public PropertySliderWidget
{
	Q_OBJECT

	public:
		DoubleSliderWidget( QWidget * parent );
		
		/**
		 * Destructor
		 */
		virtual ~DoubleSliderWidget();

		void setDigits( int digits ){
			m_digits = digits;
		}

		void setCaption( const QString& caption ){
			propertyLabel->setText( caption );
		}

		void setExplanation( const QString& explanation ){
//X 			valueLabel->setText( explanationLabel );
		}
		
	private slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( int value );
		
		virtual void setValueText( int value );

	private:
		int m_digits;
	
	signals:
		void valueHasChanged( int value, int digits );
};
#endif // PROPERTYSLIDERWIDGET_H

