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
#include "infodialog.h"
#include "fastinfo.h"
#include "detailinfodlg.h"

#include <iostream>

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
		slotShowDetailedData();
		slotShowData(); //böse
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


//böse böse
//
void ElementKP::slotShowDetailedData()
{
	DetailedInfoDlg *DIDlg = new DetailedInfoDlg( Data , this , "foo" );
	DIDlg->show();
}


void ElementKP::slotShowData()
{
    infoDialog *show_data2 = new infoDialog( Data , this );
	show_data2->show();
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
