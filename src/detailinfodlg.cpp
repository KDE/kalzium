/***************************************************************************

        detailinfodlg.cpp  -  description
                             -------------------
    begin                : Tue Apr 8 2003
    copyright            : (C) 2003, 2004 by Carsten Niehaus
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

#include <kdialogbase.h>
#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kiconloader.h>

#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qframe.h>
#include <qlayout.h>
#include <qstring.h>

#include "detailinfodlg.h"
#include "orbitswidget.h"
#include "element.h"
#include "detail_chemical.h"
#include "detail_energy.h"
#include "prefs.h"

DetailedInfoDlg::DetailedInfoDlg( Element *el , QWidget *parent, const char *name)
    : KDialogBase(KDialogBase::IconList, i18n( "" ),  Close, Close, parent,name)
{
	e = el;

	QString num = QString::number( e->number() );
	QString elname = e->elname().utf8();
	QString cap = i18n( "%1 (%2)" ).arg( elname ).arg( num );
	setCaption( cap );
    
	m_pOverviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon( "overview" ));
	QVBoxLayout *overviewLayout = new QVBoxLayout( m_pOverviewTab );
	dTab = new DetailedTab( e, m_pOverviewTab );
	overviewLayout->addWidget( dTab );


	/////////////////////////////////
    m_pMiscTab = addPage(i18n("Miscellaneous"), i18n("Miscellaneous"), BarIcon( "misc" ));
	QVBoxLayout *miscLayout = new QVBoxLayout( m_pMiscTab );
	QLabel *discovered_label = new QLabel( i18n("Discovered: %1").arg( Element::adjustUnits(  e->date() , 5 ) ) , m_pMiscTab );
	QLabel *meanweight_label = new QLabel( i18n("Mean weight: %1 u").arg(e->meanweight() ) , m_pMiscTab );
	QWhatsThis::add( meanweight_label , i18n( "The mean weight is the atomic weight divided by the number of protons" ) );
	miscLayout->addWidget( discovered_label );
	miscLayout->addWidget( meanweight_label );
	miscLayout->insertStretch(-1,1);

	////////////////////////////////////7
	m_pPictureTab = addPage(i18n("Picture"), i18n("What does %1 look like?").arg( i18n( e->elname().utf8() ) ), BarIcon( "elempic" ));
	QVBoxLayout *mainLayout = new QVBoxLayout( m_pPictureTab );
	QLabel *piclabel = new QLabel( m_pPictureTab );
	if ( !locate(  "data" , "kalzium/elempics/" + e->symbol() + ".jpg" ).isEmpty() )
	{
		QPixmap pic ( locate( "data" , "kalzium/elempics/" + e->symbol() + ".jpg" ) );
		QImage img = pic.convertToImage();
		img = img.smoothScale ( 400, 400, QImage::ScaleMin );
		pic.convertFromImage( img );
		piclabel->setPixmap( pic );
	}
	else 
	{
		piclabel->setText( i18n( "No picture of %1 found." ).arg( i18n( e->elname().utf8() ) ) );
	}
	mainLayout->addWidget( piclabel );

	
	////////////////////////////////////7
	
	m_pEnergyTab =   addPage( i18n("Energies"), i18n( "Energy Information" ), BarIcon( "energies" ) );
	QVBoxLayout *energyLayout = new QVBoxLayout( m_pEnergyTab );
	detail_energy *wEnergy = new detail_energy( m_pEnergyTab );

	wEnergy->mp_label->setText( Element::adjustUnits( e->melting(), 0 ) );
	wEnergy->bp_label->setText( Element::adjustUnits( e->boiling(), 0 ) );
	wEnergy->sion_label->setText( Element::adjustUnits(  e->ie2(), 1 ) );
	wEnergy->ion_label->setText( Element::adjustUnits( e->ie(), 1 ) );
	wEnergy->en_label->setText(  QString::number( e->electroneg() ) );
	energyLayout->addWidget( wEnergy );
	
	////////////////////////////////////7
	
	m_pChemicalTab = addPage( i18n("Chemical Data"), i18n( "Chemical Data" ), BarIcon( "chemical") );
	QVBoxLayout *chemicalLayout = new QVBoxLayout( m_pChemicalTab, 0, KDialog::spacingHint() );
	detail_chemical *wChemical = new detail_chemical( m_pChemicalTab );

	wChemical->orbits_label->setText( e->parsedOrbits() );
	wChemical->symbol_label->setText( e->symbol() );
	wChemical->density_label->setText( Element::adjustUnits( e->density(), 4 ) );
	wChemical->block_label->setText( e->block() );
	wChemical->radius_label->setText( Element::adjustUnits( e->radius(), 2 ) );
	wChemical->weight_label->setText( Element::adjustUnits( e->weight(), 3 ) );
	chemicalLayout->addWidget( wChemical );
    
    /////////////////////////////////
	
	m_pModelTab = addPage( i18n("Atom Model"), i18n( "Atom Model" ), BarIcon( "orbits" ));
	QVBoxLayout *modelLayout = new QVBoxLayout( m_pModelTab , 0, KDialog::spacingHint() );
	OrbitsWidget *wOrbits = new OrbitsWidget( e->number(), m_pModelTab );
	QWhatsThis::add( wOrbits,  i18n( "Here you can see the atomic hull of %1. %2 has the configuration %3." )
							.arg( i18n( e->elname().utf8() ) )
							.arg( i18n( e->elname().utf8() ) )
							.arg( e->parsedOrbits() ) );
	modelLayout->addWidget( wOrbits );

}

DetailedTab::DetailedTab( Element *el, QWidget *parent, const char *name ) : QWidget( parent, name )
{
	setBackgroundMode( NoBackground );

	e = el;
}

void DetailedTab::paintEvent( QPaintEvent* )
{
	int h = this->height();
	int w = this->width();

	QPixmap pm( w, h );

	QPainter p;
	p.begin( &pm );

	h_t = 20; //height of the texts
	
	x1 =  0;
	y1 =  0;

	x2 = w;
	y2 = h;

	p.setBrush(Qt::SolidPattern);
	p.setBrush( PSEColor( e->block() ));
	p.drawRect( x1 , y1 , x2 , y2 );

	p.setBrush( Qt::black );
	p.setBrush(Qt::NoBrush);

	/**
	 *	TODO: Fix the fonts
	 **/
	QFont f1 ( "times", 20, QFont::Bold );
	QFont f2 ( "times", 10 );
	QFont f3 ( "times", 8, QFont::Bold );

	p.setFont( f1 );
	int h_ = y2/10*3; //a third of the whole widget
	p.drawText( x2/10 * 4, h_ , e->symbol() ); //Symbol

	p.setFont( f2 );
 	p.drawText( x1+4, y2-h_t , x2/2 , h_t , Qt::AlignLeft , i18n( e->elname().utf8() )); //Name
	
	p.drawText( x1+4+h_t , y2-2*h_t , x2-x1-4-h_t , h_t , Qt::AlignRight , e->oxstage() );    //Oxidationszahlen
	p.drawText( x2/2 , y2-h_t , x2/2-4 , h_t , Qt::AlignRight , QString::number( e->weight() )); //Weight

	p.setFont( f3 );
	p.drawText( x2/10*3 , h_-h_t , x2/10, h_t , Qt::AlignRight , QString::number( e->number() ));
	drawBiologicalSymbol( &p );

	p.end();

	bitBlt( this, 0, 0, &pm );
}

void DetailedTab::drawBiologicalSymbol( QPainter *p )
{
	const int db = h_t;        //diameter of the big circle
	const int ds = db/2;      //diameter of the inner circle

	int d_ds = ( db/2 )-( ds/2 ); //the delta-x/y of the inner circle

	int pos_x = x1+4;
	int pos_y = y2 - 5 - 2*h_t;

	switch ( e->biological() )
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
	QColor c;

	if ( block == "s" )
		c = Prefs::block_s();
	else if ( block == "d" )
		c = Prefs::block_p();
	else if ( block == "p" )
		c = Prefs::block_d();
	else if ( block == "f" )
		c = Prefs::block_f();
	return  c;
}

#include "detailinfodlg.moc"
