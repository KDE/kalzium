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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "molcalcwidgetbase.h"

#include "moleculeparser.h"

class KToggleAction;
class KLineEdit;

class Element;
class MolcalcDialog;
class MoleculeParser;

/**
 * This widget is a small calculator for molecules.
 *
 * @author Carsten Niehaus
 * @author Pino Toscano
 * @author Inge Wallin
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

	protected slots:
		void slotCalcButtonClicked();

	private slots:
		void clear();

    private:
		/**
		 * @return the HTML code of an element symbol and its
		 * subscripted amount. Eg: Mg<sub>2</sub>
		 */
		QString compositionString( ElementCountMap &_map );

		/**
		 * This methods gathers all the data and updates the 
		 * contents of the widget.
		 */
		void updateUI();

	private:
		MoleculeParser   m_parser;
		
		double           m_mass;
		bool 			 m_validInput;
		ElementCountMap  m_elementMap;
};


#endif // MOLCALC_IMPL_H
