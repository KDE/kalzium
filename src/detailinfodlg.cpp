/***************************************************************************

        detailinfodlg.cpp  -  description
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

#include <kdebug.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <ksimpleconfig.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qimage.h>
#include <qwhatsthis.h>

#include "detailinfodlg.h"
#include "orbitswidget.h"
#include "eleminfo.h"


DetailedInfoDlg::DetailedInfoDlg( const ElementInfo Eleminfo , QWidget *parent, const char *name)
    : KDialogBase(IconList, i18n("Detailed Look on %1").arg( Eleminfo.Name.lower().utf8() ), Ok|User1 ,Ok, parent,name, true, false)
{
	Data = Eleminfo;
    const KIconLoader *kil = KGlobal::iconLoader();

	setButtonText( User1 , i18n( "Weblookup" ) );

	/////////////////////////////////
	overviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon(kil->iconPath( "overview" , KIcon::User)));
	QVBoxLayout *overviewLayout = new QVBoxLayout( overviewTab );
	QWidget *overviewWidget = new QWidget( overviewTab );
	QVBoxLayout *foo_layout = new QVBoxLayout( overviewWidget );

	dTab = new DetailedTab( overviewWidget );
	dTab->setData( Data );
	overviewLayout->addWidget( overviewWidget );
	foo_layout->addWidget( dTab );

	dTab->show();

    /////////////////////////////////

	mainTab = addPage(i18n("Picture"), i18n("What does %1 look like?").arg( Data.Name.utf8() ), BarIcon(kil->iconPath( "elempic" , KIcon::User)));
	QVBoxLayout *mainLayout = new QVBoxLayout( mainTab );
	
	QLabel *piclabel = new QLabel( mainTab );
	
	if ( !locate(  "data" , "kalzium/elempics/" + Data.Symbol + ".jpg" ).isEmpty() )
	{
		QPixmap pic ( locate( "data" , "kalzium/elempics/" + Data.Symbol + ".jpg" ) );
		QImage img = pic.convertToImage();
		img = img.smoothScale ( 400, 400, QImage::ScaleMin );
		pic.convertFromImage( img );
		piclabel->setPixmap( pic );
	}
	else 
	{
		piclabel->setText( i18n( "No picture of %1 found!" ).arg( Data.Name.utf8() ) );
	}

	mainLayout->addWidget( piclabel );

    /////////////////////////////////
    energyTab = addPage(i18n("Energies"), i18n("Energyinformation"), BarIcon(kil->iconPath( "energies" , KIcon::User)));
	QVBoxLayout *energyLayout = new QVBoxLayout( energyTab, 5 );
	QLabel *ENlabel = new QLabel( i18n( "Electronegativity: %1" ).arg( Data._EN ) , energyTab );
	QLabel *Ionlabel = new QLabel(i18n( "Ionization energy: %1" ).arg( Data._IE ) , energyTab );
	QLabel *MPlabel = new QLabel(i18n( "Melting point: %1" ).arg( Data._MP ) , energyTab );
	QLabel *BPlabel = new QLabel(i18n( "Boiling point: %1" ).arg( Data._BP ) , energyTab );
	energyLayout->addWidget( ENlabel );
	energyLayout->addWidget( Ionlabel );
	energyLayout->addWidget( MPlabel );
	energyLayout->addWidget( BPlabel );
	energyLayout->insertStretch(-1,1);

    /////////////////////////////////
    chemicalTab = addPage(i18n("Chemical Data"), i18n("Chemical data"), BarIcon(kil->iconPath( "chemical" , KIcon::User)) );
	QVBoxLayout *chemicalLayout = new QVBoxLayout( chemicalTab, 5 );
	QLabel *orbtisLabel = new QLabel( i18n( "Orbits: %1" ).arg( Data.orbits ) , chemicalTab );
	QLabel *symbolLabel = new QLabel( i18n( "Symbol: %1" ).arg( Data.Symbol ) , chemicalTab  );
	QLabel *densityLabel = new QLabel( i18n( "Density: %1").arg( Data._Density ) , chemicalTab );
	QLabel *blockLabel  = new QLabel( i18n( "Block: %1" ).arg( Data.Block ) , chemicalTab );
	QLabel *atomrad    = new QLabel( i18n( "Atomic radius: %1" ).arg( Data._AR ) , chemicalTab );
	QLabel *atomweightLabel = new QLabel( i18n( "Atomic weight: %1" ).arg( Data.Weight ) , chemicalTab );
	chemicalLayout->addWidget( orbtisLabel );
	chemicalLayout->addWidget( symbolLabel );
	chemicalLayout->addWidget( densityLabel);
	chemicalLayout->addWidget( blockLabel  );
	chemicalLayout->addWidget( atomrad );
	chemicalLayout->addWidget( atomweightLabel);
	chemicalLayout->insertStretch(-1,1);
    
	/////////////////////////////////
    miscTab = addPage(i18n("Miscellaneous"), i18n("Miscellaneous"), BarIcon(kil->iconPath( "misc" , KIcon::User)));
	QVBoxLayout *miscLayout = new QVBoxLayout( miscTab, 5 );
	QLabel *discovered_label = new QLabel( i18n("Discovered: %1").arg(Data.date ) , miscTab );
	QLabel *meanweight_label = new QLabel( i18n("Meanweight: %1").arg(Data.meanweight ) , miscTab );
	QWhatsThis::add( meanweight_label , i18n( "The meanweight is the atomic weight divided by the number of protons" ) );
	miscLayout->addWidget( discovered_label );
	miscLayout->addWidget( meanweight_label );
	miscLayout->insertStretch(-1,1);
	
	/////////////////////////////////
    orbitsTab = addPage(i18n("Atom model"), i18n("Atom Model"), BarIcon(kil->iconPath( "orbits" , KIcon::User)));
	QVBoxLayout *orbitsLayout = new QVBoxLayout( orbitsTab );
	OrbitsWidget *orbitsPic = new OrbitsWidget( Data.number , orbitsTab );
	QWhatsThis::add( orbitsPic,  i18n( "Here you can see the atomic hull of %1. %2 has the configuration %3." )
								.arg( Data.Name.utf8() )
								.arg( Data.Name.utf8() )
								.arg( Data.orbits ) );
	orbitsLayout->addWidget( orbitsPic );
}

void DetailedInfoDlg::slotUser1()
{
	KHTMLPart *html = new KHTMLPart();

	KConfig *mainc = KGlobal::config();
	mainc->setGroup( "WLU" );
	QString url = mainc->readEntry( "address" ) + Data.Symbol.lower()  + ".html";
	if ( mainc->readEntry( "address" ).contains( "pearl1" ) ) 
		url = mainc->readEntry( "address" )+QString::number( Data.number )+".html";

	const KURL site(url);
	html->openURL(site);
	html->view()->resize(html->view()->contentsWidth() + html->view()->frameWidth() ,400);
	html->show();
}


DetailedTab::DetailedTab( QWidget *parent, const char *name ) : QWidget( parent, name )
{
}

void DetailedTab::setData( ElementInfo Eleminfo )
{
	EData = Eleminfo;
}

void DetailedTab::paintEvent( QPaintEvent* )
{
	QPainter p;
	p.begin(this);

	int h = this->height();
	int w = this->width();

	h_t = 20; //height of the texts
	
	x1 =  0;
	y1 =  0;

	x2 = w;
	y2 = h;

	p.setBrush(Qt::SolidPattern);
	p.setBrush( PSEColor( EData.Block ));
	p.drawRect( x1 , y1 , x2 , y2 );
	
	p.setBrush( Qt::black );
	p.setBrush(Qt::NoBrush);

	QFont f1 ( "times", 20, QFont::Bold );
	QFont f2 ( "times", 10 );
	QFont f3 ( "times", 8, QFont::Bold );

	p.setFont( f1 );
	int h_ = y2/10*3; //a third of the whole widget
	p.drawText( x2/10 * 4, h_ , EData.Symbol ); //Symbol

	p.setFont( f2 );
 	p.drawText( x1+4, y2-h_t , x2/2 , h_t , Qt::AlignLeft , EData.Name.utf8() ); //Name
	
	p.drawText( x1+4+h_t , y2-2*h_t , x2-x1-4-h_t , h_t , Qt::AlignRight , EData.oxstage);    //Oxidationszahlen
	p.drawText( x2/2 , y2-h_t , x2/2-4 , h_t , Qt::AlignRight , EData.Weight ); //Weight

	p.setFont( f3 );
	p.drawText( x2/10*3 , h_-h_t , x2/10, h_t , Qt::AlignRight , QString::number( EData.number ));
	drawBiologicalSymbol( &p );

	p.end();
}

void DetailedTab::drawBiologicalSymbol( QPainter *p )
{
	const int db = h_t;        //diameter of the big circle
	const int ds = db/2;      //diameter of the inner circle

	int d_ds = ( db/2 )-( ds/2 ); //the delta-x/y of the inner circle

	int pos_x = x1+4;
	int pos_y = y2 - 5 - 2*h_t;

	switch ( EData.biological )
	{
		case 0:        //nothing
			break;
		case 1:        //red, red
			p->setBrush( Qt::red );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::red );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::red );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 2:        //green, red
			p->setBrush( Qt::red );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::red );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::green );
			p->setPen( Qt::green );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 3:        //green
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::green );
			p->setPen( Qt::green );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 4:        //green, blue
			p->setBrush( Qt::blue );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::blue );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::green );
			p->setPen( Qt::green );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 5:        //blue
			p->setBrush(Qt::SolidPattern);
			p->setBrush( Qt::blue );
			p->setPen( Qt::blue );
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
		case 6:        //blue, blue
			p->setBrush( Qt::blue );
			p->setBrush(Qt::NoBrush);
			p->setPen( Qt::blue );
			p->drawEllipse( pos_x,pos_y,db,db );
			p->setBrush(Qt::SolidPattern);
			p->drawEllipse( pos_x+d_ds, pos_y+d_ds, ds, ds );
			break;
	}
}

QColor DetailedTab::PSEColor( const QString &block ) const
{
	KConfig *config = KGlobal::config();
	config->setGroup( "Colors" );
    QColor c;
    if ( block == "s" )
		c = config->readColorEntry("s");
    else if ( block == "d" )
		c = config->readColorEntry("d");
    else if ( block == "p" )
		c = config->readColorEntry("p");
    else if ( block == "f" )
		c = config->readColorEntry("f");
    return  c;
}


#include "detailinfodlg.moc"
