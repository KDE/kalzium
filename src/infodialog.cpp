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

#include "kalzium.h"

infoDialog::infoDialog( ElementInfo Eleminfo , QWidget *parent, const char *name)
    : infoDlg( parent , name )
{
    setCaption( i18n( Eleminfo.Name.utf8() ) );
    name_label->setText( i18n( Eleminfo.Name.utf8() ) );
    symbol_label->setText(i18n( "%1" ).arg( Eleminfo.Symbol ) );
    block_label->setText( i18n( "%1" ).arg( Eleminfo.Block ) );

    if ( Eleminfo.Density == -1 )
	density_label->setText( i18n( "Unknown" ) );
    else
	density_label->setText( i18n("%1 g/cm<sup>3</sup>" ).arg( Eleminfo.Density ) );

    if ( Eleminfo.MP == -1 )
	melting_label->setText( i18n( "Unknown" ) );
    else
	melting_label->setText( i18n( "%1 C" ).arg( -273.15+Eleminfo.MP ) );

    if (Eleminfo.IE == -1)
	ion_label->setText( i18n( "Unknown" ) );
    else
	ion_label->setText( i18n( "%1 kJ/mol" ).arg( Eleminfo.IE*100) );

    elemno_label->setText( i18n( "%1" ).arg( Eleminfo.number ) );

    if (Eleminfo.Weight == "0")
		weight_label->setText( i18n( "Unknown" ) );
    else{
		weight_label->setText( i18n( "%1 u" ).arg( Eleminfo.Weight ) );
		meanweight_label->setText( i18n( "%1 u"). arg( Eleminfo.Weight.toDouble()/Eleminfo.number ) );
	}

    if (Eleminfo.date == "0")
	discovered_label->setText(i18n("was known to ancient cultures"));
    else if (Eleminfo.date == "3333")
	discovered_label->setText(i18n("not been discovered yet"));
    else
	discovered_label->setText(i18n("%1").arg(Eleminfo.date));

    if (Eleminfo.AR == -1)
	radius_label->setText( i18n( "Unknown" ) );
    else
	radius_label->setText( i18n( "%1 pm" ).arg( Eleminfo.AR ) );

    if (Eleminfo.BP == -1)
	boiling_label->setText( i18n( "Unknown" ) );
    else
	boiling_label->setText( i18n( "%1 C" ).arg(-273.15+Eleminfo.BP ) );

    if (Eleminfo.EN == -1)
	electro_label->setText( i18n( "Unknown" ) );
    else
	electro_label->setText( i18n( "%1" ).arg( Eleminfo.EN ) );

	oxlabel->setText( Eleminfo.oxstage );


	//show orbit information nicely
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	QString orbitData = Eleminfo.orbits;
	orbitData.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbitData.replace(rxb,"<b>\\1</b> "); //bold around element symbols
        orbitLabel->setText( orbitData );


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

#include "infodialog.moc"
