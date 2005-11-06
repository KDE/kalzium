#ifndef PROPERTYSLIDERWIDGET_H
#define PROPERTYSLIDERWIDGET_H

#include <QWidget>
#include <QList>
#include <QVariant>

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 *
 * This class is the baseclass of all slider-widgets which 
 * show or hide elements based on a certain property. For
 * example, the dateslider which shows which elements where
 * known at a certain date, is based on this class.
 */
class PropertySliderWidget : public QWidget
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
		~PropertySliderWidget();

		/**
		 * Set the list @p list as the internal valuelist
		 * @param list the list of the values of the property
		 */
		void setValues( QList<QVariant*> list );

		/**
		 * @return the currently selected value
		 */
		virtual QVariant value() const = 0;

	private slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( QVariant value ) = 0;

	private:
		QList<QVariant*> m_list;

};
#endif // PROPERTYSLIDERWIDGET_H

