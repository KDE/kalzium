/***************************************************************************
                          elementkp.cpp  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001, 2002, 2003 by Carsten Niehaus
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
#include <khtmlview.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstatusbar.h>
#include <kstandarddirs.h>
#include <kdebug.h>

//QT-Includes
#include <qdragobject.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qtable.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qwhatsthis.h>

#include <qdialog.h>

#include "elementkp.h"
#include "eleminfo.h"
#include "elementkp.moc"
#include "kalzium.h" 
#include "infodlg.h"
#include "fastinfo.h"

ElementKP::ElementKP(QWidget *parent, ElementInfo ElemInfo, const char *name, int AElemNo, KStatusBar *zeiger, Kalzium *kalzium_tmp)	
: ElementButton(parent,name)

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

	/** The first [] is the row, the second [] is the column. */
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

	if ( kalzium->showFastInfo ) kalzium->fastinfo->show();
	kalzium->fastinfo->setInfo( ElemNo );
}

//when the mousepointer leaves the button
void ElementKP::leaveEvent(QEvent *)
{
	zeigerle->message(i18n("The Kalzium-version","Kalzium %1").arg( KALZIUM_VERSION ));
	kalzium->fastinfo->hide();
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

QSize ElementKP::sizeHint() const
{
  QFontMetrics fm = fontMetrics();
  return QSize (fm.size( ShowPrefix, QString ("XXXX\nXXXX") ));
}


//******** Slots *****************************************************

void ElementKP::lookup() const
{
	KHTMLPart *html = new KHTMLPart();

	KConfig *mainc = KGlobal::config();
	mainc->setGroup( "WLU" );
	QString url = mainc->readEntry( "adress" ) + Data.Symbol.lower()  + ".html";
	if ( mainc->readEntry( "adress" ).contains( "pearl1" ) ) 
		url = mainc->readEntry( "adress" )+QString::number( Data.number )+".html";

	const KURL site(url);
	html->openURL(site);
	html->show();
	html->view()->resize(html->view()->contentsWidth() + html->view()->frameWidth() ,400);
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
	show_data2->density_label->setText( i18n("%1 g/cm<sup>3</sup>" ).arg( Data.Density ) );

    if ( Data.MP == -1 )
	show_data2->melting_label->setText( i18n( "Unknown" ) );
    else
	show_data2->melting_label->setText( i18n( "%1 C" ).arg( -273.15+Data.MP ) );

    if (Data.IE == -1)
	show_data2->ion_label->setText( i18n( "Unknown" ) );
    else
	show_data2->ion_label->setText( i18n( "%1 kJ/mol" ).arg( Data.IE*100) );

    show_data2->elemno_label->setText( i18n( "%1" ).arg( Data.number ) );

    if (Data.Weight == "0")
		show_data2->weight_label->setText( i18n( "Unknown" ) );
    else{
		show_data2->weight_label->setText( i18n( "%1 u" ).arg( Data.Weight ) );
		show_data2->meanweight_label->setText( i18n( "%1 u"). arg( Data.Weight.toDouble()/Data.number ) );
	}

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

	show_data2->oxlabel->setText( Data.oxstage );


	//show orbit information nicely
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	QString orbitData = Data.orbits;
	orbitData.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbitData.replace(rxb,"<b>\\1</b> "); //bold around element symbols
        show_data2->orbitLabel->setText( orbitData );


//    show_data2->general_label->setFont( topic_font );
//    show_data2->states_label->setFont( topic_font );
//    show_data2->energy_label->setFont( topic_font );

	// The table 
	show_data2->neighbourTable->horizontalHeader()->hide();
	show_data2->neighbourTable->verticalHeader()->hide();
	show_data2->neighbourTable->setTopMargin( 0 );
	show_data2->neighbourTable->setLeftMargin( 0 );

//	getNeighbours( ElemNo );
//X 	for( int zeile=0 ; zeile < 3 ; zeile++ )
//X 	{
//X 		for( int spalte=0 ; spalte < 3 ; spalte++ )
//X 		{
//X 			show_data2->neighbourTable->setText( zeile, spalte, neighbourArray[zeile][spalte] );
//X 			if (neighbourArray[zeile][spalte] == "leer") show_data2->neighbourTable->setText(zeile,spalte,"");
//X 		}
//X 	}

	// click on this button to load webpage for element
	QObject::connect(show_data2->weblookup, SIGNAL(clicked()), this , SLOT(lookup()));

}

void ElementKP::drawButtonLabel(QPainter *p)
{
  QFontMetrics  metricsNumber ( kalzium->generalFont() );
  QFontMetrics  metricsText ( kalzium->generalBoldFont() );
  p->setPen(QPen (black,0));
  p->setFont( kalzium->generalFont());
  p->drawText(7, metricsNumber.height(),QString::number(Data.number), -1);
  p->setFont( kalzium->generalBoldFont() );
  p->drawText ( width() - metricsText.width(Data.Symbol)-7, height()-7 , Data.Symbol);
}
