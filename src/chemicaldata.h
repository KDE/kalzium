#ifndef CHEMICALDATA_H
#define CHEMICALDATA_H
/***************************************************************************

        chemicaldata.h  -  description
                             -------------------
    begin                : June 2003
    copyright            : (C) 2003 by Carsten Niehaus
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

class QString;


class ElemInfoParsed
{
	public:
		ElemInfoParsed( ElementInfo eInfo );
		ElementInfo information();

	private:
		QString beautifyOrbits( QString ) const;
		QString beautifyOxydationstages( QString ) const;
		ElementInfo info;
};
		
#endif // CHEMICALDATA_H

