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
basic class for the specific PSEs
@author Carsten Niehaus
*/


class PSE : public QWidget
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 * @param data the object in which all data is stored
		 */
		PSE( KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~PSE();

		KalziumDataObject *d;

		virtual void activeColorScheme( int );
		
		/**
		 * four lists, on for each block in the periodic table
		 */
		QPtrList<ElementButton> sBlockList,
					dBlockList,
					pBlockList,
					fBlockList;

		/**
		 * this sets up the 4 blocklists. Every PSE has 4 lists for
		 * the 4 blocks: s,p,d,f
		 **/
		void setupBlockLists();
		
		/**
		 * this list includes all elementbuttons which are in the 
		 * specific PSE. For example. in the simple PSE
		 * the elements of the f- and d-Block would not
		 * be in this list
		 **/
		QPtrList<ElementButton> m_PSEElementButtons;

		/**
		 * this method sets up the m_PSEElementButtons-list
		 **/
		virtual void setupPSEElementButtonsList();

	protected:
		virtual void updatePSE();

	protected slots:
		void slotElementClicked( int );
};

class RegularPSE : public PSE
{
	Q_OBJECT
	public:
		RegularPSE(KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~RegularPSE();

		void setupPSEElementButtonsList();
		void activeColorScheme( int );

	protected:
		virtual void updatePSE();
};

class SimplifiedPSE : public PSE
{
	Q_OBJECT
	public:
		SimplifiedPSE(KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
		~SimplifiedPSE();
		
		void setupPSEElementButtonsList();
		void activeColorScheme( int );
};

class MendeljevPSE : public PSE
{
	Q_OBJECT

	public:
		MendeljevPSE(KalziumDataObject *data, QWidget *parent = 0, const char *name = 0);
//		void activeColorScheme( int );
		~MendeljevPSE();
};
		

#endif
