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
#include <kdialog.h>
#include <kglobalsettings.h>
#include <khtml_part.h>
#include <klocale.h>
#include <kstatusbar.h>
#include <kdebug.h>
#include <kstddirs.h>
#include <kpushbutton.h>

//QT-Includes
#include <qdragobject.h>
#include <qfont.h>
#include <qtable.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qwhatsthis.h>

#include "elementkp.h"
#include "eleminfo.h"
#include "elementkp.moc"
#include "kalzium.h" 

	ElementKP::ElementKP(const QString& text, QWidget *parent, ElementInfo ElemInfo, const char *name, int AElemNo, KStatusBar *zeiger, Kalzium *kalzium_tmp)	
: KPushButton(text,parent,name)

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

void ElementKP::showName()
{
	//shows the name now in the statusbar
	zeigerle->message(i18n(Data.Name.utf8()));
}

//******** Slots *****************************************************

void ElementKP::lookup()
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

	QWidget *show_data = new QWidget (0L,"show_data");
	show_data->setCaption(i18n(Data.Name.utf8()));
	QGridLayout *grid = new QGridLayout(show_data, 10, 4, 8);

	// *** General ***
	general_label = new QLabel ( show_data );
	general_label->setText(i18n("General"));
	general_label->setFont(topic_font);
	grid->addMultiCellWidget(general_label,0,0,0,1,Qt::AlignHCenter);

	QWidget *general_group = new QWidget(show_data);
	QGridLayout *general_grid = new QGridLayout(general_group, 4, 5, 8);
	grid->addMultiCellWidget(general_group, 1,1,0,2);

	// Name
	name_label = new QLabel (general_group);
	name_label->setText(i18n("Name:"));
	name_label->setFont(bold_font);
	general_grid->addWidget(name_label, 0,0);

	namecont_label = new QLabel (general_group);
	namecont_label->setText(i18n(Data.Name.utf8()));
	general_grid->addWidget(namecont_label, 0,1);

	// Element Number
	elemno_label = new QLabel (general_group);
	elemno_label->setText(i18n("Element number:"));
	elemno_label->setFont(bold_font);
	general_grid->addWidget(elemno_label, 0, 3);

	data_label = new QLabel ( general_group );
	data_label->setText(i18n("%1").arg(Data.number));
	general_grid->addWidget(data_label, 0, 4);

	// Symbol
	label = new QLabel (general_group);
	label->setText(i18n("Symbol:"));
	label->setFont(bold_font);
	general_grid->addWidget(label, 1,0);

	label = new QLabel (general_group);
	if (Data.Symbol == "0")
		label->setText(i18n("None"));
	else
		label->setText(i18n("%1").arg(Data.Symbol));
	general_grid->addWidget(label, 1,1);

	// Atomic weight
	label = new QLabel (general_group);
	label->setText(i18n("Atomic weight:"));
	label->setFont(bold_font);
	general_grid->addWidget(label ,1,3);

	label = new QLabel (general_group);
	if (Data.Weight == "0")
		label->setText(i18n("unknown"));
	else
		label->setText(i18n("%1 u").arg(Data.Weight));
	general_grid->addWidget(label, 1,4);


	// Block
	label = new QLabel (general_group);
	label->setText(i18n("Block:"));
	label->setFont(bold_font);
	general_grid->addWidget(label, 2,0);

	label = new QLabel (general_group);
	label->setText(i18n("%1").arg(Data.Block));
	general_grid->addWidget(label, 2,1);

	// Discovered
	disc_label = new QLabel (general_group);
	disc_label->setText(i18n("Discovered:"));
	disc_label->setFont(bold_font);
	general_grid->addWidget(disc_label, 2,3);

	discval_label = new QLabel (general_group);
	if (Data.date == "0")
		discval_label->setText(i18n("was known to ancient cultures"));
	if (Data.date == "3333")
		discval_label->setText(i18n("not been discovered yet"));
	else
		discval_label->setText(i18n("%1").arg(Data.date));
	general_grid->addWidget(discval_label, 2,4);


	//Density
	label = new QLabel (general_group);
	label->setText(i18n("Density:")); 
	label->setFont(bold_font);
	general_grid->addWidget(label, 3,0);

	label = new QLabel (general_group);
	if (Data.Density == -1)
		label->setText(i18n("Unknown"));
	else
		label->setText(i18n("%1").arg(Data.Density));
	general_grid->addWidget(label, 3,1);

	//Atomicradius
	label = new QLabel (general_group);
	label->setText(i18n("Atomic radius:")); 
	label->setFont(bold_font);
	general_grid->addWidget(label, 3,3);

	label = new QLabel (general_group);
	if (Data.AR == -1)
		label->setText(i18n("Unknown"));
	else
		label->setText(i18n("%1").arg(Data.AR));
	general_grid->addWidget(label,3,4);

	// *** States ***
	label = new QLabel ( show_data );
	label->setText(i18n("States"));
	label->setFont(topic_font);
	grid->addMultiCellWidget(label,2,2,0,2,Qt::AlignHCenter);

	QWidget *states_group = new QWidget(show_data);
	QGridLayout *states_grid = new QGridLayout(states_group, 1, 5, 8);
	grid->addMultiCellWidget(states_group, 3,3,0,2);

	// Melting point
	label = new QLabel (states_group);
	label->setText(i18n("Melting point:"));
	label->setFont(bold_font);
	states_grid->addWidget(label, 0,0);

	label = new QLabel (states_group);
	if (Data.MP == -1)
		label->setText(i18n("Unknown"));
	else
		label->setText(i18n("%1 C").arg(-273.15+Data.MP));
	states_grid->addWidget (label, 0,1);

	//Boiling point
	label = new QLabel (states_group);
	label->setText(i18n("Boiling point:"));
	label->setFont(bold_font);
	states_grid->addWidget(label, 0,3);

	label = new QLabel (states_group);
	if (Data.BP == -1)
		label->setText(i18n("Unknown"));
	else
		label->setText(i18n("%1 C").arg(-273.15+Data.BP));
	states_grid->addWidget (label, 0,4);

	/// *** Energies ***
	label = new QLabel ( show_data );
	label->setText(i18n("Energies"));
	label->setFont(topic_font);
	grid->addMultiCellWidget(label,4,4,0,2,Qt::AlignHCenter);

	QWidget *energy_group = new QWidget(show_data);
	QGridLayout *energy_grid = new QGridLayout(energy_group, 1, 5, 8);
	grid->addMultiCellWidget(energy_group, 5,5,0,2);

	//Ionization energy
	ion_label = new QLabel (energy_group);
	ion_label->setText(i18n("Ionization energy:"));
	ion_label->setFont(bold_font);
	energy_grid->addWidget(ion_label, 0,0);
	QWhatsThis::add(ion_label, i18n("This is the ionisationenergy"));

	ionval_label = new QLabel (energy_group);
	if (Data.IE == -1)
		ionval_label->setText(i18n("Unknown"));
	else
		ionval_label->setText(i18n("%1").arg(Data.IE));
	energy_grid->addWidget(ionval_label, 0,1);

	//Electronegativity
	en_label = new QLabel (energy_group);
	en_label->setText(i18n("Electronegativity:"));
	en_label->setFont(bold_font);
	energy_grid->addWidget(en_label, 0,3);
	QWhatsThis::add(en_label, i18n("This is the electronegativity of Pauling"));

	enval_label = new QLabel (energy_group);
	if (Data.EN == -1)
		enval_label->setText(i18n("Unkown"));
	else
		enval_label->setText(i18n("%1").arg(Data.EN));
	energy_grid->addWidget(enval_label, 0,4);


	QTable *neighbourTable = new QTable( 3 , 3 , show_data , "neighbourTable" );
	neighbourTable->setReadOnly( true );
	grid->addMultiCellWidget(neighbourTable, 6, 6, 0 , 2);

	neighbourTable->horizontalHeader()->hide();
	neighbourTable->verticalHeader()->hide();
	neighbourTable->setTopMargin( 0 );
	neighbourTable->setLeftMargin( 0 );

	getNeighbours( ElemNo );
	for( int zeile=0 ; zeile < 3 ; zeile++ )
    {
        for( int spalte=0 ; spalte < 3 ; spalte++ )
        {
            neighbourTable->setText( zeile, spalte, neighbourArray[zeile][spalte] );
            if (neighbourArray[zeile][spalte] == "leer") 
                neighbourTable->setText(zeile,spalte,"");
        }

    }

    if (Data.Name != "Unnamed")
        QWhatsThis::add(show_data, i18n("In this dialog Kalzium shows you information about %1").arg(i18n(Data.Name.utf8().data())));
    else
        QWhatsThis::add(show_data, i18n("This dialog displays information about this unnamed element"));



    // click on this button to load webpage for element
    KPushButton *web = new KPushButton(i18n("Web &Lookup"),show_data);
	QObject::connect(web, SIGNAL(clicked()), this , SLOT(lookup()));
	QWhatsThis::add(web, i18n("Click on this button to open a webpage with more information about %1").arg(i18n(Data.Name.utf8())));
	grid->addWidget( web, 7, 1);

	//click on this button to close the Dialog
	KPushButton *exit = new KPushButton( i18n("&Close"), show_data );
	QObject::connect (exit, SIGNAL(clicked()), show_data, SLOT(hide()));
	QWhatsThis::add(exit, i18n("Click on this button to close this dialog"));
	grid->addWidget( exit, 8, 1);

    grid->setColStretch(3, 1);
	grid->setRowStretch(9, 1);

    show_data->show();
}
