/***************************************************************************
                          elementkp.cpp  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001 by Carsten Niehaus
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

//KDE-Includes
#include <kconfig.h>
#include <kglobalsettings.h>
#include <khtml_part.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kstddirs.h>

//QT-Includes
#include <qdragobject.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qtable.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qwhatsthis.h>

#include "elementkp.h"
#include "eleminfo.h"
#include "elementkp.moc"
#include "kalzium.h" 
#include "infodlg.h"

	ElementKP::ElementKP(QWidget *parent, ElementInfo ElemInfo, const char *name, int AElemNo, KStatusBar *zeiger, Kalzium *kalzium_tmp)	
: KPushButton(parent,name)

{
	kalzium = kalzium_tmp;
	ElemNo = AElemNo;
	zeigerle=zeiger;
	Data = ElemInfo; 
}

void ElementKP::getNeighbours( int Current )
{
	int Nr=Current, ze=0, sp=0;
	position(Nr,ze,sp);

	/** The first [] is the row, the second [] is the colum. */
	neighbourArray[0][1]=kalzium->helpArray[sp/40-1][ze/40];
	neighbourArray[1][1]=kalzium->helpArray[sp/40][ze/40];
	neighbourArray[2][1]=kalzium->helpArray[sp/40+1][ze/40];
	if ( ze/40 != 17 )
	{
		neighbourArray[0][2]=kalzium->helpArray[sp/40-1][ze/40+1];
		neighbourArray[1][2]=kalzium->helpArray[sp/40][ze/40+1];
		neighbourArray[2][2]=kalzium->helpArray[sp/40+1][ze/40+1];
	}
	if (ze/40 == 0 ) return;
	neighbourArray[0][0]=kalzium->helpArray[sp/40-1][ze/40-1];
	neighbourArray[1][0]=kalzium->helpArray[sp/40][ze/40-1];
	neighbourArray[2][0]=kalzium->helpArray[sp/40+1][ze/40-1];
}

//when the mousepointer is over a button
void ElementKP::enterEvent(QEvent *)
{
	setFocus();
	showName();	
}

//when the mousepointer leaves the button
void ElementKP::leaveEvent(QEvent *)
{
	zeigerle->message(i18n("Kalzium ")+KALZIUM_VERSION);
}

void ElementKP::mouseMoveEvent( QMouseEvent * )
{
	QDragObject *d = new QTextDrag(parseElementInfo(), this);
	d->dragCopy();
}

void ElementKP::mouseReleaseEvent( QMouseEvent *mouse )
{
	pmenu = new QPopupMenu();
	if (mouse->button() == RightButton)
	{
		if (Data.Name == "0")
			pmTitle = new QLabel( i18n( "Name: Unnamed") , pmenu );
		else
			pmTitle = new QLabel( i18n( "Name: %1").arg(i18n(Data.Name.utf8())) , pmenu );
		if (Data.Weight == "0")
			pmWeight = new QLabel( i18n( "Atomic Weight: Unknown") , pmenu );
		else
			pmWeight = new QLabel( i18n( "Atomic Weight: %1 u").arg(Data.Weight) , pmenu );
		pmenu->clear();
		pmenu->insertItem( pmTitle );
		pmenu->insertSeparator();
		pmenu->insertItem( pmWeight );
		pmenu->popup( QCursor::pos() );
	}
	else
	{
		slotShowData();
	}
}

QString ElementKP::parseElementInfo()
{
	QString parse = i18n("General") 
		+ "\n"
		+ "Name: " + Data.Name  
		+ "\t" + "Element Number: " + QString::number(Data.number)
		+ "\n"
		+ "Symbol: " + Data.Symbol 
		+ "\t" + "Atomic Weight: " + Data.Weight + " u"
		+ "\n"
		+ "Block: " + Data.Block
		+ "\t" + "Discovered: " + Data.date
		+ "\n"
		+ "Density: " + QString::number(Data.Density)
		+ "\t" + "Atomic Radius: " + QString::number(Data.AR)
		+ "\n\n"
		+ "States" + "\n"
		+ "Melting point: " + QString::number(Data.MP) + " C"
		+ "\t" + "Boiling point: " + QString::number(Data.BP) + " C"
		+ "\n\n"
		+ "Energies" + "\n"
		+ "Ionization: " + QString::number(Data.IE)
		+ "\t" + "Electronegativity: " + QString::number(Data.EN)

		+ "\n";
	return parse;
}

void ElementKP::showName() const
{
	//shows the name now in the statusbar
	zeigerle->message(i18n(Data.Name.utf8()));
}

//******** Slots *****************************************************

void ElementKP::lookup() const
{
	KHTMLPart *html = new KHTMLPart();

	KConfig *mainc = KGlobal::config();
	mainc->setGroup( "WLU" );
	QString url = mainc->readEntry( "adress" ) + Data.Symbol.lower()  + ".html";

	const KURL site(url);
	html->openURL(site);
	html->show();
}


void ElementKP::slotShowData()
{
	QFont bold_font=KGlobalSettings::generalFont();
	bold_font.setBold(true);
	QFont topic_font=bold_font;
	topic_font.setPointSize(bold_font.pointSize()+2);

	infoDlg *show_data2 = new infoDlg();
	show_data2->show();
	show_data2->setCaption( i18n( Data.Name.utf8() ) );
	show_data2->name_label->setText( i18n( Data.Name.utf8() ) );
	show_data2->symbol_label->setText(i18n( "%1" ).arg( Data.Symbol ) );
	show_data2->block_label->setText( i18n( "%1" ).arg( Data.Block ) );

	if ( Data.Density == -1 )
		show_data2->density_label->setText( i18n( "Unknown" ) );
	else
		show_data2->density_label->setText( i18n("%1 g/cm^3" ).arg( Data.Density ) );

	if ( Data.MP == -1 )
		show_data2->melting_label->setText( i18n( "Unknown" ) );
	else
		show_data2->melting_label->setText( i18n( "%1 C" ).arg( -273.15+Data.MP ) );

	if (Data.IE == -1)
		show_data2->ion_label->setText( i18n( "Unknown" ) );
	else
		show_data2->ion_label->setText( i18n( "%1 kJ/mole" ).arg( Data.IE*100) );

	show_data2->elemno_label->setText( i18n( "%1" ).arg( Data.number ) );

	if (Data.Weight == "0")
		show_data2->weight_label->setText( i18n( "Unknown" ) );
	else
		show_data2->weight_label->setText( i18n( "%1 u" ).arg( Data.Weight ) );

    if (Data.date == "0")
        show_data2->discovered_label->setText(i18n("was known to ancient cultures"));
    else if (Data.date == "3333")
        show_data2->discovered_label->setText(i18n("not been discovered yet"));
    else
        show_data2->discovered_label->setText(i18n("%1").arg(Data.date));

	if (Data.AR == -1)
		show_data2->radius_label->setText( i18n( "Unknown" ) );
	else
		show_data2->radius_label->setText( i18n( "%1 pm" ).arg( Data.AR ) );

	if (Data.BP == -1)
		show_data2->boiling_label->setText( i18n( "Unknown" ) );
	else
		show_data2->boiling_label->setText( i18n( "%1 C" ).arg(-273.15+Data.BP ) );

	if (Data.EN == -1)
		show_data2->electro_label->setText( i18n( "Unknown" ) );
	else
		show_data2->electro_label->setText( i18n( "%1" ).arg( Data.EN ) );


	show_data2->general_label->setFont( topic_font );
	show_data2->states_label->setFont( topic_font );
	show_data2->energy_label->setFont( topic_font );
	
/*	// The table 
	show_data2->neighbourTable->horizontalHeader()->hide();
	show_data2->neighbourTable->verticalHeader()->hide();
	show_data2->neighbourTable->setTopMargin( 0 );
	show_data2->neighbourTable->setLeftMargin( 0 );

	getNeighbours( ElemNo );
	for( int zeile=0 ; zeile < 3 ; zeile++ )
	{
		for( int spalte=0 ; spalte < 3 ; spalte++ )
		{
			show_data2->neighbourTable->setText( zeile, spalte, neighbourArray[zeile][spalte] );
			if (neighbourArray[zeile][spalte] == "leer") show_data2->neighbourTable->setText(zeile,spalte,"");
		}

	}
*/
	// click on this button to load webpage for element
	QObject::connect(show_data2->weblookup, SIGNAL(clicked()), this , SLOT(lookup()));

	//click on this button to close the Dialog
	QObject::connect (show_data2->exit, SIGNAL(clicked()), show_data2, SLOT(hide()));

}

void ElementKP::drawButtonLabel(QPainter *p)
{
    QFont general = KGlobalSettings::generalFont();
    QFont general_bold = general;
    general_bold.setBold(TRUE);
    general.setPointSize(general.pointSize()-2);
    general_bold.setPointSize(general_bold.pointSize()+1);

    p->setPen(QPen (black,0));

    QFontMetrics zahl( general );
    QFontMetrics text( general_bold );
    
    p->setFont( general );
    p->drawText(7,zahl.height(),QString::number(Data.number), -1);
    p->setFont( general_bold );
    p->drawText ( width() - text.width(Data.Symbol)-7, height()-7 , Data.Symbol);
}
