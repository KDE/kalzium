/***************************************************************************

        chemicaldata.cpp  -  description
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

#include "elementkp.h"
#include "chemicaldata.h"

#include <klocale.h>
#include <qregexp.h>

#include <kdebug.h>

ElemInfoParsed::ElemInfoParsed( ElementInfo eInfo )
{
	info = eInfo;
}

ElementInfo ElemInfoParsed::information()
{
	if ( info.Density == -1 )
		info._Density = i18n( "Unknown" );
	else
		info._Density = i18n("%1 g/cm<sup>3</sup>" ).arg( info.Density );

	if ( info.MP == -1 )
		info._MP = i18n( "Unknown" ) ;
	else
		info._MP = i18n( "%1 C" ).arg( -273.15+info.MP );

	if (info.IE == -1)
		info._IE = i18n( "Unknown" ) ;
	else
		info._IE = i18n( "%1 kJ/mol" ).arg( info.IE*100) ;

	if (info.Weight == "0")
	{
		info.Weight =  i18n( "Unknown" );
		info.meanweight = i18n( "Unknown" );
	}
	else
	{
		info.Weight = i18n( "%1 u" ).arg( info.Weight );
		info.meanweight = i18n( "%1 u").arg( info.Weight.toDouble()/info.number );
	}

	if (info.date == "0")
		info.date = i18n("was known to ancient cultures");
	else
		info.date = i18n("%1").arg(info.date);

	if (info.AR == -1)
		info._AR = i18n( "Unknown" );
	else
		info._AR = i18n( "%1 pm" ).arg( info.AR );

	if (info.BP == -1)
		info._BP = i18n( "Unknown" );
	else
		info._BP = i18n( "%1 C" ).arg(-273.15+info.BP );

	if (info.EN == -1)
		info._EN = i18n( "Unknown" );
	else
		info._EN = i18n( "%1" ).arg( info.EN );

	//show orbit information nicely
	info.orbits = beautifyOrbits( info.orbits );
	info.oxstage = beautifyOxydationstages( info.oxstage );

	return info;
}

QString ElemInfoParsed::beautifyOrbits( QString orbits ) const
{
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	orbits.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbits.replace(rxb,"<b>\\1</b> "); //bold around element symbols
	return orbits;
}

//TODO
//this reqexp is not working
QString ElemInfoParsed::beautifyOxydationstages( QString ox ) const
{
	QRegExp rxa("([0-9]");
	QRegExp rxc("[0-9])");
	ox.replace(rxa,"123123"); //bold around element symbols
	ox.replace(rxc,"12321312"); //bold around element symbols
	kdDebug() << ox << endl;
	return ox;
}



