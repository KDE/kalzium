/***************************************************************************

                  infodialog.cpp  -  description
                             -------------------
    begin                : Tue Apr 8 2003 
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
#include "infodialog.h"
#include "infodlg.h"
#include "eleminfo.h"

#include <klocale.h>
#include <qlabel.h>
#include <qregexp.h>
#include <qtable.h>

#include <khtml_part.h>
#include <khtmlview.h>
#include <kpushbutton.h>
#include <kconfig.h>
#include <kdebug.h>

#include "kalzium.h"

infoDialog::infoDialog( ElementInfo Eleminfo , QWidget *parent, const char *name)
    : infoDlg( parent , name )
{
    setCaption( i18n( Eleminfo.Name.utf8() ) );

	ElemInfoParsed *eInfo = new ElemInfoParsed( Eleminfo );
	Eleminfo = eInfo->information();
	
	electro_label->setText( Eleminfo._EN );
	weight_label->setText( Eleminfo.Weight );
	melting_label->setText( Eleminfo._MP );
	boiling_label->setText( Eleminfo._BP );
	name_label->setText( Eleminfo.Name );
	symbol_label->setText( Eleminfo.Symbol );
	radius_label->setText( Eleminfo._AR );
	discovered_label->setText( Eleminfo.date );
	elemno_label->setText( QString::number( Eleminfo.number ));
	
	// click on this button to load webpage for element
	QObject::connect(weblookup, SIGNAL(clicked()), this , SLOT(lookup()));
}

void infoDialog::lookup() const
{
//X 	KHTMLPart *html = new KHTMLPart();
//X 
//X 	KConfig *mainc = KGlobal::config();
//X 	mainc->setGroup( "WLU" );
//X 	QString url = mainc->readEntry( "adress" ) + Data.Symbol.lower()  + ".html";
//X 	if ( mainc->readEntry( "adress" ).contains( "pearl1" ) ) 
//X 		url = mainc->readEntry( "adress" )+QString::number( Data.number )+".html";
//X 
//X 	const KURL site(url);
//X 	html->openURL(site);
//X 	html->show();
//X 	html->view()->resize(html->view()->contentsWidth() + html->view()->frameWidth() ,400);
}

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
	else if (info.date == "3333")
		info.date = i18n("not been discovered yet");
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

	return info;
};

QString ElemInfoParsed::beautifyOrbits( QString orbits ) const
{
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	orbits.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbits.replace(rxb,"<b>\\1</b> "); //bold around element symbols
	return orbits;
}



#include "infodialog.moc"
