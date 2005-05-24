/***************************************************************************
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

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kglobalsettings.h>
#include <khtml_part.h>
#include <khtmlview.h>

#include <qlabel.h>
#include <qpainter.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qvaluelist.h>

#include "detailinfodlg.h"
#include "orbitswidget.h"
#include "prefs.h"
#include "isotopewidget.h"
#include "detailedgraphicaloverview.h"

DetailedInfoDlg::DetailedInfoDlg( KalziumDataObject *data, Element *el , QWidget *parent, const char *name)
    : KDialogBase(KDialogBase::IconList, "",  Close, Close, parent,name)
{
	m_data = data;
	e = el;

	m_baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	m_baseHtml.append("kalzium/data/");
	m_baseHtml.append("bg.jpg");

	( actionButton( KDialogBase::Close ) )->setFocus();
	
	m_pOverviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon( "overview" ));
	m_pPictureTab = addPage(i18n("Picture"), i18n("What does this element look like?"), BarIcon( "elempic" ));
	m_pModelTab = addPage( i18n("Atom Model"), i18n( "Atom Model" ), BarIcon( "orbits" ));
	
	mainLayout = new QVBoxLayout( m_pPictureTab );
	overviewLayout = new QVBoxLayout( m_pOverviewTab );
	QVBoxLayout *modelLayout = new QVBoxLayout( m_pModelTab , 0, KDialog::spacingHint() );

	dTab = new DetailedGraphicalOverview( m_pOverviewTab, "DetailedGraphicalOverview" );
	dTab->setElement( e );
 	overviewLayout->addWidget( dTab );
	wOrbits = new OrbitsWidget( m_pModelTab );
	piclabel = new QLabel( m_pPictureTab );
	
	mainLayout->addWidget( piclabel );
	modelLayout->addWidget( wOrbits );
	
	createContent( e );
}

void DetailedInfoDlg::addTab( const QString& htmlcode, const QString& title, const QString icontext, const QString iconname )
{
	QFrame *frame = addPage(title, icontext, BarIcon(iconname));
	QVBoxLayout *layout = new QVBoxLayout( frame );
	KHTMLPart *w = new KHTMLPart( frame, "html-part", frame );
	layout->addWidget( w->view() );
	
	w->begin();
	w->write( htmlcode );
	w->end();

	//add this page to the list of pages
	m_pages.append( frame );
}

QString DetailedInfoDlg::getHtml(DATATYPE type)
{
	QString html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	html.append( m_baseHtml );
	html.append("\">");

	//get the list of ionisation-energies
	QValueList<double> ionlist = e->ionisationList();

	switch ( type )
	{
		case CHEMICAL:
			html.append( i18n( "Orbital structure: " ) );
			html.append( e->parsedOrbits() );
			html.append( "<p />" );
			html.append( i18n("Symbol: ") );
			html.append( e->symbol() );
			html.append( "<p />" );
			html.append( i18n( "Density: " ) );
			html.append( e->adjustUnits( Element::DENSITY ) );
			html.append( "<p />" );
			html.append( i18n( "Block: " ) );
			html.append( e->block() );
			html.append( "<p />" );
			html.append( i18n( "Radius: " ) );
			html.append( e->adjustUnits( Element::RADIUS ) );
			html.append( "<p />" );
			html.append( i18n( "Weight: " ) );
			html.append( e->adjustUnits( Element::WEIGHT  ) );
			break;
		case MISC:
			html.append( "<p />" );
			html.append( i18n( "Discovered: " ) );
			html.append( e->adjustUnits( Element::DATE ) );
			html.append( "<p />" );
			html.append( i18n( "Mean weight: " ) );
			html.append( QString::number( e->meanweight() ));
			html.append( "u" );
			break;
		case ENERGY:
			html.append( "<p>" );
			html.append( i18n( "Melting Point: " ) );
			html.append( e->adjustUnits( Element::MELTINGPOINT ) );
			html.append( "<p>" );
			html.append( i18n( "Boiling Point: " ) );
			html.append( e->adjustUnits( Element::BOILINGPOINT ) );
			html.append( "<p>" );
			
			int i = 0;
			for ( ; i < ionlist.count() ; ++i )
			{
				html.append( i18n("the variable is a number. The result is for example '1.' or '5.'", "%1. Ionizationenergy: " ).arg( QString::number( i+1 ) ) );
				html.append( e->adjustUnits( Element::IE, ionlist[i] ) );
				html.append( "<p>" );
			}
			html.append( i18n( "Electronegativity: " ) );
			html.append( e->adjustUnits( Element::EN ) );
			break;
	}
			
	html.append( "</p></body></html>" );
	
	return html;
}

void DetailedInfoDlg::createContent( Element *el )
{
	addTab( getHtml(CHEMICAL), i18n( "Chemical Data" ), i18n( "Chemical Data" ), "chemical" );
	addTab( getHtml(ENERGY), i18n( "Energies" ), i18n( "Energy Information" ), "energies" );
	addTab( getHtml(MISC), i18n( "Miscellaneous" ), i18n( "Miscellaneous" ), "misc" );
	
	QString num = QString::number( el->number() );
	QString elname = i18n( el->elname().utf8() );
	QString cap = i18n("For example Carbon (6)" , "%1 (%2)" ).arg( elname ).arg( num );
	setCaption( cap );

	dTab->setElement( el );

	////////////////////////////////////7
	if ( !locate(  "data" , "kalzium/elempics/" + el->symbol() + ".jpg" ).isEmpty() )
	{
		QPixmap pic ( locate( "data" , "kalzium/elempics/" + el->symbol() + ".jpg" ) );
		QImage img = pic.convertToImage();
		img = img.smoothScale ( 400, 400, QImage::ScaleMin );
		pic.convertFromImage( img );
		piclabel->setPixmap( pic );
	}
	else 
	{
		piclabel->setText( i18n( "No picture of %1 found." ).arg( elname ) );
	}

    /////////////////////////////////
	
	wOrbits->setElementNumber( el->number() );
	wOrbits->repaint();
	QWhatsThis::add( wOrbits,  i18n( "Here you can see the atomic hull of %1. %2 has the configuration %3." )
							.arg( elname )
							.arg( elname )
							.arg( el->parsedOrbits() ) );
}

void DetailedInfoDlg::wheelEvent( QWheelEvent *ev )
{
	int number = e->number();

	Element *element;
	if ( ev->delta() < 0 )
	{
		if ( number > 1 )
			element = m_data->element( number-1 );
		else
			return;
	}
	else if ( number < 111 )
	{
		element = m_data->element( number+1 );
	}
	else
		return;

	e = element;
	
	QValueList<QFrame*>::iterator it = m_pages.begin();
	QValueList<QFrame*>::iterator itEnd = m_pages.end();
	for ( ; it != itEnd ; ++it ){
		delete *it;
		*it = NULL;
	}
	m_pages.clear();

	createContent( e );
}

#include "detailinfodlg.moc"
