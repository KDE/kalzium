#ifndef MOLCALCWIDGET_H
#define MOLCALCWIDGET_H
/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "molcalcwidgetbase.h"

#include <qmap.h>

#include "moleculeparser.h"

class KToggleAction;
class KLineEdit;

class Element;
class MolcalcDialog;
class MoleculeParser;

/**
 * This widget is a small calculator for moleculas.
 *
 * @author Carsten Niehaus
 * @author Pino Toscano
 */
class MolcalcWidget : public MolcalcWidgetBase
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 * @param parent parent widget
		 * @param name name of this widget
		 */
		MolcalcWidget( QWidget *parent = 0, const char *name = 0 );

	private slots:
		void clear();

	private:
		MoleculeParser m_parser;
		
		double m_mass;
		QMap<Element*, int> m_elements;

		enum KIND
		{
			ADD = 0,
			REMOVE = 1
		};
		
		/**
		 * updates the list of elements and calls \ref updateUI()
		 */
		void updateData( int number, KIND kind );

		/**
		 * @return the HTML code of an element symbol and its
		 * subscripted amount. Eg: Mg<sub>2</sub>
		 */
		QString composition( QMap<Element*,int> );

		/**
		 * recalculates the mass
		 */
		void recalculate();

		/**
		 * this methods gathers all the data and updates the
		 * data
		 */
		void updateUI();

	protected slots:
		void slotCalcButtonClicked();
};


#endif // MOLCALC_IMPL_H
