/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef PSE_H
#define PSE_H

#include <qwidget.h>
#include <qvaluelist.h>
#include <kpushbutton.h>
#include <qptrlist.h>

#include "element.h"
#include "elementbutton.h"

/**
 * @short basic class for the specific PSEs
 * @author Carsten Niehaus
 */
class PSE : public QWidget
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 * @param data is the object in which all data is stored
		 */
		PSE( KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~PSE();

		KalziumDataObject *d;

		/**
		 * This method sets the colors of the PSE. 
		 @param nr takes 5 different values:
		 @li normal view
		 @li groups
		 @li blocks
		 @li state-of-matter
		 @li acidic behavior
		 */
		void activateColorScheme( const int nr);

		/**
		 * In this list all buttons corresponding the the block
		 * in the periodic table are stored.
		 */
		QPtrList<ElementButton> sBlockList,
		dBlockList,
		pBlockList,
		fBlockList;

		/**
		 * This method set the color for the buttons corresponding to
		 * the given temperature @p temp
		 * @param temp is the temperature to which all buttons will be set
		 */
		void setTemperature( const double temp );

		/**
		 * This list includes all elementbuttons which are in the 
		 * specific PSE. For example. in the simple PSE
		 * the elements of the f- and d-Block would not
		 * be in this list
		 */
		QPtrList<ElementButton> m_PSEElementButtons;

	private:

		/**
		 * this sets up the 4 blocklists. Every PSE has 4 lists for
		 * the 4 blocks: s,p,d,f
		 */
		void setupBlockLists();

		/**
		 * Sets up all elements to the correct buttonlists
		 * @see m_PSEElementButtons
		 */
		virtual void setupPSEElementButtonsList();

		public slots:
			void slotUpdatePSE();

		/**
		 * this method hides all elements which have not been know
		 * before the @p date.
		 * @param date is time where the user wants to see whether
		 * or not the element has already been kown
		 */
		void setDate( int date );
};

/**
 * This class represents the periodic table most persons know.
 * All elements are included.
 * @short The regular periodic table with all elements
 * @author Carsten Niehaus
 */
class RegularPSE : public PSE
{
	Q_OBJECT
	public:
		RegularPSE(KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~RegularPSE();

		/**
		 * Sets up all elements to the correct buttonlists
		 * @see m_PSEElementButtons
		 */
		void setupPSEElementButtonsList();
};

/**
 * This class includes only the elements in the s- and block. It is intended for
 * pupil in lower classes or perhaps people who don't know very much about
 * chemistry.
 * @short A simplified periodic table with only the elements of the s- and p-block
 * @author Carsten Niehaus
 */
class SimplifiedPSE : public PSE
{
	Q_OBJECT
	public:
		SimplifiedPSE(KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~SimplifiedPSE();
		
		/**
		 * Sets up all elements to the correct buttonlists
		 * @see m_PSEElementButtons
		 */
		void setupPSEElementButtonsList();
};

/**
 * This class represents the periodic table most persons know.
 * All elements are included.
 * @short The regular periodic table with all elements
 * @author Carsten Niehaus
 */
class MendeljevPSE : public PSE
{
	Q_OBJECT

	public:
		MendeljevPSE(KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		
		/**
		 * Sets up all elements to the correct buttonlists
		 * @see m_PSEElementButtons
		 */
		void setupPSEElementButtonsList();
		
		~MendeljevPSE();
};
		

#endif
