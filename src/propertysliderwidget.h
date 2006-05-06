/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PROPERTYSLIDERWIDGET_H
#define PROPERTYSLIDERWIDGET_H

#include <QWidget>
#include <QList>

#include <chemicaldataobject.h>

#include "ui_propertysliderwidgetbase.h"

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 *
 * This class is the baseclass of all slider-widgets which 
 * show or hide elements based on a certain property. For
 * example, the dateslider which shows which elements where
 * known at a certain date, is based on this class.
 *
 * The value of such a slider is a single integer. The set of
 * values from which the integer is taken can be set with
 * setValues().
 *
 * Only for the boiling- and melting point this class cannot be used as
 * for that we need a more complex user interface and compare two
 * values instead of one.
 *
 * @see SOMWidgetIMPL
 */
class PropertySliderWidget : public QWidget, protected Ui_PropertySliderWidgetBase
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
		int value() const{
			return slider->value();
		}

	private:
		int m_value;
		ChemicalDataObject::BlueObelisk m_type;

	protected slots:
		/**
		 * this slot reacts to a new value
		 */
		virtual void slotValueChanged( int value );

	protected:
		QList<int> m_list;

	protected slots:
		/**
		 * Proxy slot when the value changes
		 * @param value the new value
		 */
		virtual void valueChanged( int value );

		virtual void setValueText( int value );

		void slotSliderKindChanged( int index );
	
	signals:
		/**
		 * emitted when the value changes
		 * @param value the new value
		 */
		void valueHasChanged( int value );

		/**
		 * emitted when the value changes
		 * @param value the new value
		 * @param type the type of the new value
		 */
		void valueHasChanged( ChemicalDataObject::BlueObelisk type, int value );
};
#endif // PROPERTYSLIDERWIDGET_H

