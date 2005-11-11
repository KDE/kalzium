#ifndef PROPERTYSLIDERWIDGET_H
#define PROPERTYSLIDERWIDGET_H

#include <QWidget>
#include <QList>
#include <QVariant>

#include "timewidget.h"

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 *
 * This class is the baseclass of all slider-widgets which 
 * show or hide elements based on a certain property. For
 * example, the dateslider which shows which elements where
 * known at a certain date, is based on this class.
 */
class PropertySliderWidget : public QWidget, protected Ui_TimeWidget
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
		virtual int value() const = 0;

	private:
		int m_value;

	protected slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( int value ) = 0;

	protected:
		QList<int> m_list;

	protected slots:
		/**
		 * emitted when the value changes
		 * @param the new value
		 */
		virtual void valueChanged( int value );

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
		
		/**
		 * @return the currently selected value
		 */
		virtual int value() const;

	private slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( int value );
};
#endif // PROPERTYSLIDERWIDGET_H

