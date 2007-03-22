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
#include "isotope.h"
#include "kalziumutils.h"
#include "kalziumdataobject.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>
#include <kcombobox.h>
#include <kapplication.h>

#include <qfile.h>
#include <qlabel.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>

#include "element.h"
#include "orbitswidget.h"
#include "detailedgraphicaloverview.h"
#include "spectrum.h"
#include "spectrumviewimpl.h"

//TODO add bondxx-radius (H-H-distance for example)

DetailedInfoDlg::DetailedInfoDlg( Element *el , QWidget *parent, const char *name)
    : KDialogBase( IconList, name, Help|User1|User2|Close, Close, parent, name, 
			false, //non modal
			false, 
			KGuiItem(i18n("Next element", "Next"), "1rightarrow"), 
			KGuiItem(i18n("Previous element", "Previous"), "1leftarrow"))
{
	m_element = el;

	m_baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	m_baseHtml.append("kalzium/data/htmlview/");
	m_baseHtml.append("style.css");

	m_picsdir = KGlobal::dirs()->findResourceDir( "appdata", "elempics/" ) + "elempics/";

	( actionButton( KDialogBase::Close ) )->setFocus();
	
	// creating the tabs but not the contents, as that will be done when
	// setting the element
	createContent();

	m_actionCollection = new KActionCollection(this);	
	KStdAction::quit(this, SLOT(slotClose()), m_actionCollection);

	setButtonTip( User2, i18n( "Goes to the previous element" ) );
	setButtonTip( User1, i18n( "Goes to the next element" ) );

	setElement( el );
}

void DetailedInfoDlg::setElement(Element *element)
{
	if ( !element ) return;

	m_element = element;
	m_elementNumber = element->number();
	
	reloadContent();

	enableButton( User1, true );
	enableButton( User2, true );
	if ( m_elementNumber == 1 )
		enableButton( User2, false );
	else if ( m_elementNumber == KalziumDataObject::instance()->numberOfElements() )
		enableButton( User1, false );
}

KHTMLPart* DetailedInfoDlg::addHTMLTab( const QString& title, const QString& icontext, const QString& iconname )
{
	QFrame *frame = addPage(title, icontext, BarIcon(iconname));
	QVBoxLayout *layout = new QVBoxLayout( frame );
	layout->setMargin( 0 );
	KHTMLPart *w = new KHTMLPart( frame, "html-part", frame );
	layout->addWidget( w->view() );

	return w;
}

void DetailedInfoDlg::fillHTMLTab( KHTMLPart* htmlpart, const QString& htmlcode )
{
	if ( !htmlpart ) return;

	htmlpart->begin();
	htmlpart->write( htmlcode );
	htmlpart->end();
}

QString DetailedInfoDlg::getHtml(DATATYPE type)
{
	QString html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>Chemical data</title><link rel=\"stylesheet\" type=\"text/css\" href=\"";
	html += m_baseHtml + "\" /><base href=\"" + m_baseHtml + "\"/></head><body><div class=\"chemdata\">";

	//get the list of ionisation-energies
	QValueList<double> ionlist = m_element->ionisationList();
	
	html.append( "<div><table summary=\"header\"><tr><td>" );
	html.append( m_element->symbol() );
	html.append( "<td><td>" );
	html.append( i18n( "Block: %1" ).arg( m_element->block() ) );
	html.append( "</td></tr></table></div>" );
	html.append( "<table summary=\"characteristics\" class=\"characterstics\">");
	switch ( type )
	{
		case CHEMICAL:
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( "<b>" + i18n( "Electronic configuration: %1" ).arg( m_element->parsedOrbits() ) + "</b>" );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"density.png\" alt=\"icon\"/></td><td>" );
			html.append( "<b>" + i18n( "Density: %1" ).arg( m_element->adjustUnits( Element::DENSITY ) ) + "</b>" );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
			html.append( "<b>" + i18n( "Covalent Radius: %1" ).arg( m_element->adjustRadius( Element::COVALENT ) ) + "</b>" );
			html.append( "</td></tr>" );
			if ( m_element->radius(Element::IONIC) > 0.0 )
			{
				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n( "<b>Ionic Radius (Charge): %1</b> (%2)" ).arg( m_element->adjustRadius(Element::IONIC) ).arg( m_element->ioncharge() ) );
				html.append( "</td></tr>" );
			}
			if ( m_element->radius(Element::VDW) > 0.0 )
			{
				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td>" );
				html.append( "<b>" + i18n( "van der Waals Radius: %1" ).arg( m_element->adjustRadius(Element::VDW) ) + "</b>" );
				html.append( "</td></tr>" );
			}
		
			if ( m_element->radius(Element::ATOMIC) > 0.0 )
			{
				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
				html.append( "<b>" + i18n( "Atomic Radius: %1" ).arg( m_element->adjustRadius(Element::ATOMIC) ) + "</b>" );
				html.append( "</td></tr>" );
			}
			
			html.append( "<tr><td stype=\"text-align:center\"><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
			html.append( "<b>" + i18n( "Mass: %1" ).arg( m_element->adjustUnits( Element::MASS ) ) + "</b>" );
			html.append( "</td></tr>" );
			if ( !m_element->isotopes().isEmpty() )
			{
				html.append( "<tr><td stype=\"text-align:center\"><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
				html.append( isotopeTable() );
				html.append( "</td></tr>" );
			}
			break;
		case MISC:
			html.append( "<tr><td><img src=\"discovery.png\" alt=\"icon\"/></td><td>" );
			html.append( m_element->adjustUnits( Element::DATE ) );
			if ( !m_element->scientist( ).isEmpty() )
				html += "<br />" + i18n("It was discovered by %1").arg(m_element->scientist() );
			html.append( "</td></tr>" );
			
			if ( m_element->abundance() > 0 ){
			html.append( "<tr><td><img src=\"abundance.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Abundance in crustal rocks: %1 ppm" ).arg( KGlobal::locale()->formatLong( m_element->abundance() ) ) );
			html.append( "</td></tr>" );
			}
			
			html.append( "<tr><td><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Mean mass: %1 u" ).arg( KalziumUtils::localizedValue( m_element->meanmass(), 6 ) ) );
			html.append( "</td></tr>" );
			if ( !m_element->nameOrigin().isEmpty() )
			{
				html.append( "<tr><td><img src=\"book.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n( "Origin of the name: %1" ).arg( m_element->nameOrigin() ) );
				html.append( "</td></tr>" );
			}
			if ( m_element->artificial() || m_element->radioactive() )
			{
				html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
				if ( !m_element->radioactive() )
					html.append( i18n( "This element is artificial" ));
				else if ( !m_element->artificial() )
					html.append( i18n( "This element is radioactive" ));
				else
					html.append( i18n( "This element is radioactive and artificial" ));
				html.append( "</td></tr>" );
			}
			break;
		case ENERGY:
			html.append( "<tr><td><img src=\"meltingpoint.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Melting Point: %1" ).arg( m_element->adjustUnits( Element::MELTINGPOINT ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"boilingpoint.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Boiling Point: %1" ).arg( m_element->adjustUnits( Element::BOILINGPOINT ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Electronegativity: %1" ).arg( m_element->adjustUnits( Element::EN ) ) );
			html.append( "</td></tr>" );
			html.append( "<tr><td><img src=\"electronaffinity.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Electron affinity: %1 " ).arg( m_element->adjustUnits(Element::EA) ) );
			html.append( "</td></tr>" );
						

			uint i = 0;
			for ( ; i < ionlist.count() ; ++i )
			{
				html.append( "<tr><td><img src=\"ionisation.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n("the first variable is a number. The result is for example '1.' or '5.', the second is the value of the ionisation energy",
				             "%1. Ionization energy: %2" ).arg( QString::number( i+1 ), m_element->adjustUnits( Element::IE, ionlist[i] ) ) );
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
	QValueList<Isotope*> list = m_element->isotopes();

	QString html;
	
	html = "<table class=\"isotopes\" cellspacing=\"0\"><tr><td colspan=\"7\">";
	html += i18n( "Isotope-Table" );
	html += "</tr></td><tr><td><b>";
	html += i18n( "Mass" );
	html += "</b></td><td><b>";
	html += i18n( "Neutrons" );
	html += "</b></td><td><b>";
	html += i18n( "Percentage" );
	html += "</b></td><td><b>";
	html += i18n( "Half-life period" );
	html += "</b></td><td><b>";
	html += i18n( "Energy and Mode of Decay" );
	html += "</b></td><td><b>";
	html += i18n( "Spin and Parity" );
	html += "</b></td><td><b>";
	html += i18n( "Magnetic Moment" );
	html += "</b></td></tr>";

	QValueList<Isotope*>::const_iterator it = list.begin();
	const QValueList<Isotope*>::const_iterator itEnd = list.end();

	for ( ; it != itEnd; ++it )
	{
		html.append( "<tr><td align=\"right\">" );
		if ( ( *it )->weight() > 0.0 )
			html.append( i18n( "%1 u" ).arg( KalziumUtils::localizedValue( ( *it )->weight(), 6 ) ) );
		//	html.append( i18n( "%1 u" ).arg( QString::number( ( *it )->weight() ) ));
		html.append( "</td><td>" );
		html.append( QString::number( ( *it )->neutrons() ) );
		html.append( "</td><td>" );
		if ( ( *it )->percentage() > 0.0 )
			html.append( i18n( "this can for example be '24%'", "%1%" ).arg( KalziumUtils::localizedValue( ( *it )->percentage(), 6 ) ) );
		html.append( "</td><td>" );
		if ( ( *it )->halflife() > 0.0 ) {
			html.append( ( *it )->halflifeAsString()  );
		}
		html.append( "</td><td>" );
		if ( ( *it )->alphapercentage() > 0.0 ){
			if ( ( *it )->alphadecay() > 0.0 )
			html.append( i18n( "%1 MeV" ).arg( KalziumUtils::localizedValue( ( *it )->alphadecay(), 6 ) ) );
			html.append( i18n( " %1" ).arg( QChar( 945 ) ) );
			if ( ( *it )->alphapercentage() < 100.0)
				html.append( i18n( "(%1%)" ).arg( KalziumUtils::localizedValue( (*it )->alphapercentage(), 6 ) ) );
			if ( ( *it )->betaminuspercentage() > 0.0 || ( *it )->betapluspercentage() > 0.0 || ( *it )->ecpercentage() > 0.0)
			html.append( i18n( ", " ) );
			}
		if ( ( *it )->betaminuspercentage() > 0.0 ){
			if ( ( *it )->betaminusdecay() > 0.0 )
				html.append( i18n( "%1 MeV" ).arg( KalziumUtils::localizedValue( ( *it )->betaminusdecay(), 6 ) ) );
			html.append( i18n( " %1<sup>-</sup>" ).arg( QChar( 946 ) ) );
			if ( ( *it )->betaminuspercentage() < 100.0)
				html.append( i18n( "(%1%)" ).arg( KalziumUtils::localizedValue( ( *it )->betaminuspercentage(), 6 ) ));
			if ( ( *it )->betapluspercentage() > 0.0 || ( *it )->ecpercentage() > 0.0 )
			html.append( i18n( ", " ) );
			}
		if ( ( *it )->betapluspercentage() > 0.0 ){
			if ( ( *it )->betaplusdecay() > 0.0 )
				html.append( i18n( "%1 MeV" ).arg( KalziumUtils::localizedValue( ( *it )->betaplusdecay(), 6 ) ) );
			html.append( i18n( " %1<sup>+</sup>" ).arg(QChar( 946 ) ) );
			if ( ( *it )->betapluspercentage() == ( *it )->ecpercentage() ) {
				if ( ( *it )->ecdecay() > 0.0 ) {
					html.append( i18n( "%1 MeV" ).arg( KalziumUtils::localizedValue( ( *it )->ecdecay(), 6 ) ) ); 
				}
				html.append( i18n( "Acronym of Electron Capture"," EC" ) ); 
			}
			if ( ( *it )->betapluspercentage() < 100.0)	
				html.append( i18n( "(%1%)" ).arg( KalziumUtils::localizedValue( ( *it )->betapluspercentage(), 6 ) ) ); 
			html += " ";
			}	
		if ( ( *it )->ecpercentage() > 0.0 && ( *it )->ecpercentage()!=( *it )->betapluspercentage()){
			if ( ( *it )->ecdecay() > 0.0 )
				html.append( i18n( "%1 MeV" ).arg( KalziumUtils::localizedValue( ( *it )->ecdecay(), 6 ) ) );
			html.append( i18n( "Acronym of Electron Capture"," EC" ) );
			if ( ( *it )->ecpercentage() < 100.0 )
				html.append( i18n( "(%1%)" ).arg( KalziumUtils::localizedValue( ( *it )->ecpercentage(), 6 ) ) );
			}
		html.append( "</td><td>" );
		html.append( ( *it )->spin() );
		html.append( "</td><td>" );
		if ( !( *it )->magmoment().isEmpty() ) {
			QString v = KGlobal::locale()->formatNumber( ( *it )->magmoment(), false, 6 );
			html.append( i18n( "%1 %2<sub>n</sub>" ).arg( v ).arg( QChar( 956 ) ) );
		}	
		html.append( "</td></tr>" );
	
	}
	
	html += "</table>";

	return html;
}

void DetailedInfoDlg::createContent( )
{
	// overview tab
	QFrame *m_pOverviewTab = addPage( i18n( "Overview" ), i18n( "Overview" ), BarIcon( "overview" ) );
	QVBoxLayout *overviewLayout = new QVBoxLayout( m_pOverviewTab );
	overviewLayout->setMargin( 0 );
	dTab = new DetailedGraphicalOverview( m_pOverviewTab, "DetailedGraphicalOverview" );
	overviewLayout->addWidget( dTab );

	// picture tab
	QFrame *m_pPictureTab = addPage( i18n( "Picture" ), i18n( "What does this element look like?" ), BarIcon( "elempic" ) );
	QVBoxLayout *mainLayout = new QVBoxLayout( m_pPictureTab );
	mainLayout->setMargin( 0 );
	piclabel = new QLabel( m_pPictureTab );
	piclabel->setMinimumSize( 400, 350 );
	mainLayout->addWidget( piclabel );

	// atomic model tab
	QFrame *m_pModelTab = addPage( i18n( "Atom Model" ), i18n( "Atom Model" ), BarIcon( "orbits" ) );
	QVBoxLayout *modelLayout = new QVBoxLayout( m_pModelTab );
	modelLayout->setMargin( 0 );
	wOrbits = new OrbitsWidget( m_pModelTab );
	modelLayout->addWidget( wOrbits );

	// html tabs
	m_htmlpages["chemical"] = addHTMLTab( i18n( "Chemical Data" ), i18n( "Chemical Data" ), "chemical" );
	m_htmlpages["energies"] = addHTMLTab( i18n( "Energies" ), i18n( "Energy Information" ), "energies" );
	m_htmlpages["misc"] = addHTMLTab( i18n( "Miscellaneous" ), i18n( "Miscellaneous" ), "misc" );

	// spectrum widget tab
	QFrame *m_pSpectrumTab = addPage( i18n("Spectrum"), i18n( "Spectrum" ), BarIcon( "spectrum" ));
	QVBoxLayout *spectrumLayout = new QVBoxLayout( m_pSpectrumTab , 0, KDialog::spacingHint() );
	spectrumLayout->setMargin( 0 );
	m_spectrumStack = new QWidgetStack( m_pSpectrumTab );
	spectrumLayout->addWidget( m_spectrumStack );
	m_spectrumview = new SpectrumViewImpl( m_spectrumStack, "spectrumwidget" );
	m_spectrumStack->addWidget( m_spectrumview );
	m_spectrumLabel = new QLabel( m_spectrumStack );
	m_spectrumStack->addWidget( m_spectrumLabel );
}

void DetailedInfoDlg::reloadContent()
{
	// reading the most common data
	const QString element_name = m_element->elname();
	const QString element_symbol = m_element->symbol();

	// updating caption
	setCaption( i18n( "For example Carbon (6)" , "%1 (%2)" ).arg( element_name ).arg( m_elementNumber ) );

	// updating overview tab
	dTab->setElement( m_element );

	// updating picture tab
	QString picpath = m_picsdir + element_symbol + ".jpg";
	if ( QFile::exists( picpath ) )
	{
		QImage img( picpath, "JPEG" );
		img = img.smoothScale ( 400, 400, QImage::ScaleMin );
		QPixmap pic;
		pic.convertFromImage( img );
		piclabel->setPixmap( pic );
	}
	else 
		piclabel->setText( i18n( "No picture of %1 found." ).arg( element_name ) );

	// updating atomic model tab
	wOrbits->setElementNumber( m_elementNumber );
	QWhatsThis::add( wOrbits,
	    i18n( "Here you can see the atomic hull of %1. %2 has the configuration %3." )
	    .arg( m_element->elname() )
	    .arg( m_element->elname() )
	    .arg( m_element->parsedOrbits() ) );

	// updating html tabs
	fillHTMLTab( m_htmlpages["chemical"], getHtml( CHEMICAL ) );
	fillHTMLTab( m_htmlpages["energies"], getHtml( ENERGY ) );
	fillHTMLTab( m_htmlpages["misc"], getHtml( MISC ) );

	// updating spectrum widget
	if ( m_element->hasSpectrum() )
	{
		delete m_spectrumview;
		
		m_spectrumview = new SpectrumViewImpl( m_spectrumStack, "spectrumwidget" );
		m_spectrumview->setSpectrum( m_element->spectrum() );
		m_spectrumStack->addWidget( m_spectrumview );
		m_spectrumStack->raiseWidget( m_spectrumview );
	}
	else
	{
		m_spectrumLabel->setText( i18n( "No spectrum of %1 found." ).arg( element_name ) );
		m_spectrumStack->raiseWidget( m_spectrumLabel );
	}
}

void DetailedInfoDlg::slotHelp()
{
	emit helpClicked();
	
	QString chapter = "infodialog_overview";
	switch ( activePageIndex() ){
		case 0: 
			chapter = "infodialog_overview";
			break;
		case 1:
			 chapter = "infodialog_picture";
			break;
		case 2:
			 chapter = "infodialog_orbits";
			break;
		case 3:
			 chapter = "infodialog_chemical";
			break;
		case 4:
			 chapter = "infodialog_energies";
			break;
		case 5:
			 chapter = "infodialog_misc";
			break;
		case 6:
			 chapter = "infodialog_spectrum";
			break;
	}

	if ( kapp )
		kapp->invokeHelp ( chapter, "kalzium" );
}

void DetailedInfoDlg::wheelEvent( QWheelEvent *ev )
{
	if ( ev->delta() < 0 )
		// setting the previous element
		slotUser2();
	else
		// setting the next element
		slotUser1();
}

void DetailedInfoDlg::slotUser1()
{
// setting the next element
	if ( m_elementNumber < KalziumDataObject::instance()->numberOfElements() )
	{
		setElement( KalziumDataObject::instance()->element( m_elementNumber + 1 ) );
		// no need to increment m_elementNumber here too,
		// as it's already done by setElement
		emit elementChanged( m_elementNumber );
	}
}

void DetailedInfoDlg::slotUser2()
{
// setting the previous element
	if ( m_elementNumber > 1 )
	{
		setElement( KalziumDataObject::instance()->element( m_elementNumber - 1 ) );
		// no need to decrement m_elementNumber here too,
		// as it's already done by setElement
		emit elementChanged( m_elementNumber );
	}
}

#include "detailinfodlg.moc"
