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
#ifndef ELEMENT_H
#define ELEMENT_H

#include <qstring.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>
#include <qptrlist.h>

class Element;

typedef QValueList<Element*> EList;

class KalziumDataObject
{
	public:
		KalziumDataObject();
		~KalziumDataObject();

		EList ElementList;

};

/**
An 'Element' is the represention of a chemical element

@author Carsten Niehaus
*/
class Element{
public:
    Element( int );
    ~Element();
    
	int number() const {return m_number;}
	QString symbol() const {return m_symbol;}
	QString name() const {return m_name;}
	QString block() const {return m_block;}
    	double weight() const {return m_weight;}
	double meanweight();

    int x, y; //for the RegularPSE
    int s_x, s_y; //for the SimplifiedPSE
    
private:
    void setupXY();

    double  m_weight,
			m_MP, 
			m_BP, 
			m_EN, 
			m_Density, 
			m_IE, 
			m_IE2, 
			m_AR;
	
    int     m_number, 
			m_biological;
			
	QString m_symbol,
			m_name,
			m_oxstage,
			m_az,
			m_block,
			m_Group,
			m_date,
			m_acidbeh,
			m_orbits;
};


#endif
