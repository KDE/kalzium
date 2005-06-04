/***************************************************************************
    begin                : Tue Apr 8 2003
    copyright            : (C) 2003, 2004, 2005 by Carsten Niehaus
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

#include "detailinfodlg.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kstandarddirs.h>

#include <qlabel.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "element.h"
#include "orbitswidget.h"
#include "detailedgraphicaloverview.h"

DetailedInfoDlg::DetailedInfoDlg( KalziumDataObject *data, Element *el , QWidget *parent, const char *name)
    : KDialogBase(KDialogBase::IconList, "",  Close, Close, parent,name)
{
	m_data = data;
	e = el;

	m_baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	m_baseHtml.append("kalzium/data/htmlview/");
	m_baseHtml.append("style.css");

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
	QString html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>Chemical data</title><link rel=\"stylesheet\" type=\"text/css\" href=\"";
	html += m_baseHtml + "\" /><base href=\"" + m_baseHtml + "\"/></head><body><div class=\"chemdata\">";

	//get the list of ionisation-energies
	QValueList<double> ionlist = e->ionisationList();
	
	html.append( "<div><table summary=\"header\"><tr><td>" );
	html.append( e->symbol() );
	html.append( "<td><td>" );
	html.append( i18n( "Block: %1" ).arg( e->block() ) );
	html.append( "</td></tr></table></div>" );
	html.append( "<table summary=\"characteristics\" class=\"characterstics\">");
	switch ( type )
	{
		case CHEMICAL:
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "<b>Orbital structure: %1</b>" ).arg( e->parsedOrbits() ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"density.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "<b>Density: %1</b>" ).arg( e->adjustUnits( Element::DENSITY ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
			html.append( i18n( "<b>Covalent Radius: %1</b>" ).arg( e->adjustRadius( Element::COVALENT ) ) );
			html.append( "</td></tr>" );
			if ( e->getRadius(Element::IONIC) != 0.0 )
			{
				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
				html.append( i18n( "<b>Ionic Radius (Charge): %1 </b>(%2)" ).arg( e->adjustRadius(Element::IONIC) ).arg( e->ioncharge() ) );
				html.append( "</td></tr>" );
			}
			if ( e->getRadius(Element::VDW) != 0.0 )
			{
				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
				html.append( i18n( "<b>van der Waals Radius: %1 </b>" ).arg( e->adjustRadius(Element::VDW) ) );
				html.append( "</td></tr>" );
			}
		
			if ( e->getRadius(Element::ATOMIC) != 0.0 )
			{
				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
				html.append( i18n( "<b>Atomic Radius: %1 </b>" ).arg( e->adjustRadius(Element::ATOMIC) ) );
				html.append( "</td></tr>" );
			}
			
			html.append( "<tr><td stype=\"text-align:center\"><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "<b>Mass: %1</b>" ).arg( e->adjustUnits( Element::MASS ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td stype=\"text-align:center\"><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
			html.append( isotopeTable() );
			html.append( "</td></tr>" );
			break;
		case MISC:
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( e->adjustUnits( Element::DATE ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Mean mass: %1 u" ).arg( QString::number( e->meanmass() ) ) );
			html.append( "</td></tr>" );
			if ( !e->nameOrigin().isEmpty() )
			{
				html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n( "Origin of the name: %1" ).arg( e->nameOrigin() ) );
				html.append( "</td></tr>" );
			}
			break;
		case ENERGY:
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Orbital structure: %1" ).arg( e->parsedOrbits() ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Melting Point: %1" ).arg( e->adjustUnits( Element::MELTINGPOINT ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Boiling Point: %1" ).arg( e->adjustUnits( Element::BOILINGPOINT ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Electronegativity: %1" ).arg( e->adjustUnits( Element::EN ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Electron affinity: %1 " ).arg( e->adjustUnits(Element::EA) ) );
			html.append( "</td></tr>" );
						

			uint i = 0;
			for ( ; i < ionlist.count() ; ++i )
			{
				html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n("the first variable is a number. The result is for example '1.' or '5.', the second is the value of the ionisation energy",
				             "%1. Ionization energy: %2" ).arg( QString::number( i+1 ), e->adjustUnits( Element::IE, ionlist[i] ) ) );
			html.append( "</td></tr>" );
			}
			break;
	}
			
	html += ( "</table>" );
	html += "</div></body></html>";
	
	return html;
}

QString DetailedInfoDlg::isotopeTable()
{
	const QString isotopes_string = e->Isotopes();
	QString isotopes = isotopes_string;
	QString html;

	int pos;
	int l;

	html = "<table class=\"isotopes\" cellspacing=\"0\"><tr><td colspan=\"3\">";
	html += i18n( "Isotope-Table" );
	html += "</tr></td><tr><td><b>";
	html += i18n( "Weight" );
	html += "</b></td><td><b>";
	html += i18n( "Neutrons" );
	html += "</b></td><td><b>";
	html += i18n( "Percentage" );
	html += "</b></td></tr>";
	
	for (  int num = 0; num < isotopes_string.contains(  ";" ) ; ++num )
	{
		pos = isotopes.find(  ";" );
		l = isotopes.length();

		QString str = isotopes.left(  pos );
		QString new_str = isotopes.right(  l-pos-1 );

		//now append the html-code... 
		html.append( isotopeRow( str ) );
		isotopes = new_str;
	}
	
	html += ( "</table>" );

	return html;
}

QString DetailedInfoDlg::isotopeRow( const QString& str )
{
	QString text;

	//number of neutrons of the isotope
	int pos = str.find( ":" );
	QString neutrons_str = str.left( pos );
	QString tmp = str.right( str.length()-pos-1 );

	//Weight of the isotope
	pos = tmp.find( ":" );
	QString weight_str = tmp.left( pos );

	//Percentage of the isotope
	tmp = tmp.right( tmp.length()-pos-1 );

 	QString neutrons;
 	QString weight;
 	QString percentage;

	weight_str.append( i18n( " u" ) );
	weight.append( weight_str );
	neutrons.append( neutrons_str );

	percentage.append( tmp );

	text = "<tr><td align=\"right\">";
	text.append( weight );
	text.append( "</td><td>" );
	text.append( neutrons );
	text.append( "</td><td>" );
	text.append( percentage );
	text.append( "</td></tr>" );

	return text;
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
