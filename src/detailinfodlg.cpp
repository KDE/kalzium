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
#include "kalziumdataobject.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>
#include <kcombobox.h>
#include <kstdaction.h>
#include <ktoolinvocation.h>

#include <QFile>
#include <QLabel>
#include <QImage>
#include <QLayout>
#include <QPushButton>

#include "element.h"
#include "orbitswidget.h"
#include "detailedgraphicaloverview.h"
#include "spectrum.h"
#include "spectrumviewimpl.h"
#include "kalziumutils.h"

//TODO add bondxx-radius (H-H-distance for example)

DetailedInfoDlg::DetailedInfoDlg( Element *el , QWidget *parent )
    : KDialogBase( IconList, "detailinfodlg", Help|User1|User2|Close, Close, parent, "detailinfodlg",
			false, //non modal
			false, 
			KGuiItem(i18n("Next element", "Next"), "1rightarrow"), 
			KGuiItem(i18n("Previous element", "Previous"), "1leftarrow"))
{
	m_data    = KalziumDataObject::instance();

	m_baseHtml = KGlobal::dirs()->findResourceDir( "data", "kalzium/data/" ) += "kalzium/data/htmlview/";

	m_picsdir = KGlobal::dirs()->findResourceDir( "data", "kalzium/elempics/" ) + "kalzium/elempics/";

	( actionButton( KDialogBase::Close ) )->setFocus();

	// creating the tabs but not the contents, as that will be done when
	// setting the element
	createContent();

	m_actionCollection = new KActionCollection(this);	
	KStdAction::quit(this, SLOT(slotClose()), m_actionCollection);

	setButtonTip( User2, i18n( "Goes to the previous element" ) );
	setButtonTip( User1, i18n( "Goes to the next element" ) );

	// setting the element and updating the whole dialog
	setElement( el );
}

void DetailedInfoDlg::setElement(Element *element)
{
	if ( !element ) return;

	m_element = element;
	
	reloadContent();

	enableButton( User1, true );
	enableButton( User2, true );
	if ( m_element->data( ChemicalDataObject::atomicNumber ) == 1 )
		enableButton( User2, false );
	else if ( m_element->data( ChemicalDataObject::atomicNumber ) == m_data->numberOfElements() )
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
	QString html =
	    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">"
	    "<html><head><title>Chemical data</title>"
	    "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + m_baseHtml + "style.css\" />"
	    "<base href=\"" + m_baseHtml + "\"/></head><body>"
	    "<div class=\"chemdata\"><div><table summary=\"header\">"
	    "<tr><td>" + m_element->dataAsString( ChemicalDataObject::symbol ) + "<td><td>"
	    + i18n( "Block: %1" ).arg( m_element->dataAsString( ChemicalDataObject::periodTableBlock ) ) +
	    "</td></tr></table></div>"
	    "<table summary=\"characteristics\" class=\"characterstics\">";

	switch ( type )
	{
		case CHEMICAL:
		{
//X 			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
//X 			html.append( "<b>" + i18n( "Electronic configuration: %1" ).arg( m_element->parsedOrbits() ) + "</b>" );
//X 			html.append( "</td></tr>" );
//X 			html.append( "<tr><td><img src=\"density.png\" alt=\"icon\"/></td><td>" );
//X 			html.append( "<b>" + i18n( "Density: %1" ).arg( m_element->adjustUnits( Element::DENSITY ) ) + "</b>" );
//X 			html.append( "</td></tr>" );
			// covalent radius
			html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
			html.append( "<b>" + i18n( "Covalent Radius: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::radiusCovalent ) ) + "</b>" );
			html.append( "</td></tr>" );
//X 			if ( m_element->radius(Element::IONIC) > 0.0 )
//X 			{
//X 				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td>" );
//X 				html.append( i18n( "<b>Ionic Radius (Charge): %1</b> (%2)" ).arg( m_element->adjustRadius(Element::IONIC) ).arg( m_element->ioncharge() ) );
//X 				html.append( "</td></tr>" );
//X 			}
			// van der Waals radius
			html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td>" );
			html.append( "<b>" + i18n( "van der Waals Radius: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::radiusVDW ) ) + "</b>" );
			html.append( "</td></tr>" );
//X 		
//X 			if ( m_element->radius(Element::ATOMIC) > 0.0 )
//X 			{
//X 				html.append( "<tr><td><img src=\"radius.png\" alt=\"icon\"/></td><td><b>" );
//X 				html.append( "<b>" + i18n( "Atomic Radius: %1" ).arg( m_element->adjustRadius(Element::ATOMIC) ) + "</b>" );
//X 				html.append( "</td></tr>" );
//X 			}
//X 			
			html.append( "<tr><td stype=\"text-align:center\"><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
			html.append( "<b>" + i18n( "Mass: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::mass ) ) + "</b>" );
			html.append( "</td></tr>" );
//X 			if ( !m_element->isotopes().isEmpty() )
//X 			{
//X 				html.append( "<tr><td stype=\"text-align:center\"><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
//X 				html.append( isotopeTable() );
//X 				html.append( "</td></tr>" );
//X 			}
			break;
		}
		case MISC:
		{
			// discovery date and discoverers
			html.append( "<tr><td><img src=\"discovery.png\" alt=\"icon\"/></td><td>" );
			html += KalziumUtils::prettyUnit( m_element, ChemicalDataObject::date );
			QString discoverers = m_element->dataAsString( ChemicalDataObject::discoverers );
			if ( !discoverers.isEmpty() )
			{
				discoverers = discoverers.replace( ";", ", " );
				html += "<br />" + i18n( "It was discovered by %1." ).arg( discoverers );
			}
			html.append( "</td></tr>" );
			// abundance
			int abundance = m_element->dataAsVariant( ChemicalDataObject::relativeAbundance ).toInt();
			if ( abundance > 0 )
			{
				html.append( "<tr><td><img src=\"abundance.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n( "Abundance in crustal rocks: %1 ppm" ).arg( abundance ) );
				html.append( "</td></tr>" );
			}
			
//X 			html.append( "<tr><td><img src=\"mass.png\" alt=\"icon\"/></td><td>" );
//X 			html.append( i18n( "Mean mass: %1 u" ).arg( QString::number( m_element->meanmass() ) ) );
//X 			html.append( "</td></tr>" );
			// origin of the name
			QString nameorigin = m_element->dataAsString( ChemicalDataObject::nameOrigin );
			if ( !nameorigin.isEmpty() )
			{
				html.append( "<tr><td><img src=\"book.png\" alt=\"icon\"/></td><td>" );
				html.append( i18n( "Origin of the name: %1" ).arg( nameorigin ) );
				html.append( "</td></tr>" );
			}
//X 			if ( m_element->artificial() || m_element->radioactive() )
//X 			{
//X 				html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
//X 				if ( !m_element->radioactive() )
//X 					html.append( i18n( "This element is artificial" ));
//X 				else if ( !m_element->artificial() )
//X 					html.append( i18n( "This element is radioactive" ));
//X 				else
//X 					html.append( i18n( "This element is radioactive and artificial" ));
//X 				html.append( "</td></tr>" );
//X 			}
			break;
		}
		case ENERGY:
		{
			// melting point
			html.append( "<tr><td><img src=\"meltingpoint.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Melting Point: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::meltingpoint ) ) );
			html.append( "</td></tr>" );
			// boiling point
			html.append( "<tr><td><img src=\"boilingpoint.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Boiling Point: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::boilingpoint ) ) );
 			html.append( "</td></tr>" );
			// electro negativity
			html.append( "<tr><td><img src=\"structure.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Electronegativity: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::electronegativityPauling ) ) );
			html.append( "</td></tr>" );
			// electro affinity
			html.append( "<tr><td><img src=\"electronaffinity.png\" alt=\"icon\"/></td><td>" );
			html.append( i18n( "Electron Affinity: %1" ).arg( KalziumUtils::prettyUnit( m_element, ChemicalDataObject::electronAffinity ) ) );
			html.append( "</td></tr>" );
//X 
//X 			//get the list of ionisation-energies
//X 			QList<double> ionlist = m_element->ionisationList();
//X 			for ( int i = 0; i < ionlist.count() ; ++i )
//X 			{
//X 				html.append( "<tr><td><img src=\"ionisation.png\" alt=\"icon\"/></td><td>" );
//X 				html.append( i18n("the first variable is a number. The result is for example '1.' or '5.', the second is the value of the ionisation energy",
//X 				             "%1. Ionization energy: %2" ).arg( QString::number( i+1 ), m_element->adjustUnits( Element::IE, ionlist[i] ) ) );
//X 			html.append( "</td></tr>" );
//X 			}
			break;
		}
	}

	html += "</table></div></body></html>";
	
	return html;
}

QString DetailedInfoDlg::isotopeTable()
{
//X 	QList<Isotope*> list = m_element->isotopes();
//X 
//X 	QString html;
//X 	
//X 	html = "<table class=\"isotopes\" cellspacing=\"0\"><tr><td colspan=\"7\">";
//X 	html += i18n( "Isotope-Table" );
//X 	html += "</tr></td><tr><td><b>";
//X 	html += i18n( "Mass" );
//X 	html += "</b></td><td><b>";
//X 	html += i18n( "Neutrons" );
//X 	html += "</b></td><td><b>";
//X 	html += i18n( "Percentage" );
//X 	html += "</b></td><td><b>";
//X 	html += i18n( "Half-life period" );
//X 	html += "</b></td><td><b>";
//X 	html += i18n( "Energy and Mode of Decay" );
//X 	html += "</b></td><td><b>";
//X 	html += i18n( "Spin and Parity" );
//X 	html += "</b></td><td><b>";
//X 	html += i18n( "Magnetic Moment" );
//X 	html += "</b></td></tr>";
//X 
//X 	QList<Isotope*>::const_iterator it = list.begin();
//X 	const QList<Isotope*>::const_iterator itEnd = list.end();
//X 
//X 	for ( ; it != itEnd; ++it )
//X 	{
//X 		html.append( "<tr><td align=\"right\">" );
//X 		if ( ( *it )->weight() > 0.0 )
//X 			html.append( i18n( "%1 u" ).arg( ( *it )->weight() ) );
//X 		//	html.append( i18n( "%1 u" ).arg( QString::number( ( *it )->weight() ) ));
//X 		html.append( "</td><td>" );
//X 		html.append( QString::number( ( *it )->neutrons() ) );
//X 		html.append( "</td><td>" );
//X 		if ( ( *it )->percentage() > 0.0 )
//X 			html.append( i18n( "this can for example be '24%'", "%1%" ).arg( ( *it )->percentage() ) );
//X 		html.append( "</td><td>" );
//X 		if ( ( *it )->halflife() > 0.0 )
//X 			html.append( ( *it )->halflifeAsString() );
//X 		html.append( "</td><td>" );
//X 		if ( ( *it )->alphapercentage() > 0.0 ){
//X 			if ( ( *it )->alphadecay() > 0.0 )
//X 			html.append( i18n( "%1 MeV" ).arg(( *it )->alphadecay() ) );
//X 			html.append( i18n( " %1" ).arg( QChar( 945 ) ) );
//X 			if ( ( *it )->alphapercentage() < 100.0)
//X 				html.append( i18n( "(%1%)" ).arg(( *it )->alphapercentage() ));
//X 			if ( ( *it )->betaminuspercentage() > 0.0 || ( *it )->betapluspercentage() > 0.0 || ( *it )->ecpercentage() > 0.0)
//X 			html.append( i18n( ", " ) );
//X 			}
//X 		if ( ( *it )->betaminuspercentage() > 0.0 ){
//X 			if ( ( *it )->betaminusdecay() > 0.0 )
//X 				html.append( i18n( "%1 MeV" ).arg(( *it )->betaminusdecay() ) );
//X 			html.append( i18n( " %1<sup>-</sup>" ).arg( QChar( 946 ) ) );
//X 			if ( ( *it )->betaminuspercentage() < 100.0)
//X 				html.append( i18n( "(%1%)" ).arg(( *it )->betaminuspercentage() ));
//X 			if ( ( *it )->betapluspercentage() > 0.0 || ( *it )->ecpercentage() > 0.0 )
//X 			html.append( i18n( ", " ) );
//X 			}
//X 		if ( ( *it )->betapluspercentage() > 0.0 ){
//X 			if ( ( *it )->betaplusdecay() > 0.0 )
//X 				html.append( i18n( "%1 MeV" ).arg(( *it )->betaplusdecay() ) );
//X 			html.append( i18n( " %1<sup>+</sup>" ).arg(QChar( 946 ) ) );
//X 			if ( ( *it )->betapluspercentage() == ( *it )->ecpercentage() ) {
//X 				if ( ( *it )->ecdecay() > 0.0 ) {
//X 				html.append( i18n( "%1 MeV" ).arg(( *it )->ecdecay() ) ); }
//X 				html.append( i18n( "Acronym of Electron Capture"," EC" ) ); 
//X 			}
//X 			if ( ( *it )->betapluspercentage() < 100.0)	
//X 				html.append( i18n( "(%1%)" ).arg(( *it )->betapluspercentage() )); 
//X 			html += " ";
//X 			}	
//X 		if ( ( *it )->ecpercentage() > 0.0 && ( *it )->ecpercentage()!=( *it )->betapluspercentage()){
//X 			if ( ( *it )->ecdecay() > 0.0 )
//X 				html.append( i18n( "%1 MeV" ).arg(( *it )->ecdecay() ) );
//X 			html.append( i18n( "Acronym of Electron Capture"," EC" ) );
//X 			if ( ( *it )->ecpercentage() < 100.0 )
//X 				html.append( i18n( "(%1%)" ).arg(( *it )->ecpercentage() ));
//X 			}
//X 		html.append( "</td><td>" );
//X 		html.append( ( *it )->spin() );
//X 		html.append( "</td><td>" );
//X 		if ( !( *it )->magmoment().isEmpty() )
//X 			html.append( i18n( "%1 %2<sub>n</sub>" ).arg( ( *it )->magmoment() ).arg( QChar( 956 ) ) );
//X 		html.append( "</td></tr>" );
//X 	
//X 	}
//X 	
//X 	html += "</table>";
//X 
//X 	return html;
	return QString();
}

void DetailedInfoDlg::createContent( )
{
	// overview tab
	QFrame *m_pOverviewTab = addPage( i18n( "Overview" ), i18n( "Overview" ), BarIcon( "overview" ) );
	QVBoxLayout *overviewLayout = new QVBoxLayout( m_pOverviewTab );
	overviewLayout->setMargin( 0 );
	dTab = new DetailedGraphicalOverview( m_pOverviewTab );
	dTab->setObjectName( "DetailedGraphicalOverview" );
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
/*
	wOrbits->setWhatsThis(
	    i18n( "Here you can see the atomic hull of %1. %2 has the configuration %3." )
	    .arg( m_element->dataAsString( ChemicalDataObject::name ) )
	    .arg( m_element->dataAsString( ChemicalDataObject::name ) )
	    .arg( "" ));//m_element->parsedOrbits() ) );
*/
	modelLayout->addWidget( wOrbits );

	// html tabs
	m_htmlpages["chemical"] = addHTMLTab( i18n( "Chemical Data" ), i18n( "Chemical Data" ), "chemical" );
	m_htmlpages["energies"] = addHTMLTab( i18n( "Energies" ), i18n( "Energy Information" ), "energies" );
	m_htmlpages["misc"] = addHTMLTab( i18n( "Miscellaneous" ), i18n( "Miscellaneous" ), "misc" );

//X 	m_pSpectrumTab = addPage( i18n("Spectrum"), i18n( "Spectrum" ), BarIcon( "spectrum" ));
//X 	QVBoxLayout *spectrumLayout = new QVBoxLayout( m_pSpectrumTab , 0, KDialog::spacingHint() );
//X 	m_pages.append( m_pSpectrumTab );
//X 	
//X 	//now add the spectrum-widget if needed
//X 	if ( m_element->hasSpectrum() )
//X 	{
//X 		m_spectrumview = new SpectrumViewImpl( m_pSpectrumTab, "spectrumwidget" );
//X 		m_spectrumview->setSpectrum( m_element->spectrum() );
//X 		spectrumLayout->addWidget( m_spectrumview );
//X 	}
//X //X 	else
//X //X 		spectrumLayout->addWidget( new QLabel( i18n( "No spectrum of %1 found." ).arg( m_element->elementName() ), m_pSpectrumTab ) );
}

void DetailedInfoDlg::reloadContent()
{
	// reading the most common data
	const int element_number = m_element->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();
	const QString element_name = m_element->dataAsString( ChemicalDataObject::name );
	const QString element_symbol = m_element->dataAsString( ChemicalDataObject::symbol );

	// updating caption
	setCaption( i18n( "For example Carbon (6)" , "%1 (%2)" ).arg( element_name ).arg( element_number ) );

	// updating overview tab
	dTab->setElement( m_element );

	// updating picture tab
	QString picpath = m_picsdir + element_symbol + ".jpg";
	if ( QFile::exists( picpath ) )
	{
		QImage img( picpath, "JPEG" );
		img = img.scaled( 400, 400, Qt::KeepAspectRatio );
		piclabel->setPixmap( QPixmap::fromImage( img ) );
	}
	else 
		piclabel->setText( i18n( "No picture of %1 found." ).arg( element_name ) );

	// updating atomic model tab
	wOrbits->setElementNumber( element_number );

	// updating html tabs
	fillHTMLTab( m_htmlpages["chemical"], getHtml( CHEMICAL ) );
	fillHTMLTab( m_htmlpages["energies"], getHtml( ENERGY ) );
	fillHTMLTab( m_htmlpages["misc"], getHtml( MISC ) );

//X 	m_pSpectrumTab = addPage( i18n("Spectrum"), i18n( "Spectrum" ), BarIcon( "spectrum" ));
//X 	QVBoxLayout *spectrumLayout = new QVBoxLayout( m_pSpectrumTab , 0, KDialog::spacingHint() );
//X 	m_pages.append( m_pSpectrumTab );
//X 	
//X 	//now add the spectrum-widget if needed
//X 	if ( m_element->hasSpectrum() )
//X 	{
//X 		m_spectrumview = new SpectrumViewImpl( m_pSpectrumTab, "spectrumwidget" );
//X 		m_spectrumview->setSpectrum( m_element->spectrum() );
//X 		spectrumLayout->addWidget( m_spectrumview );
//X 	}
//X //X 	else
//X //X 		spectrumLayout->addWidget( new QLabel( i18n( "No spectrum of %1 found." ).arg( m_element->elementName() ), m_pSpectrumTab ) );
}

void DetailedInfoDlg::slotHelp()
{
	QString chapter = "infodialog_overview";
	switch ( activePageIndex() )
	{
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
/*
		case 6:
			 chapter = "infodialog_spectrum";
			break;
*/
	}

	KToolInvocation::invokeHelp( chapter, QLatin1String( "kalzium" ) );
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
	int number = m_element->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();

	if ( number < m_data->numberOfElements() )
	{
		setElement( m_data->element( number + 1 ) );
#if 0
		emit elementChanged( number + 1 );
#endif
	}
}

void DetailedInfoDlg::slotUser2()
{
// setting the previous element
	int number = m_element->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();

	if ( number > 1 )
	{
		setElement( m_data->element( number - 1 ) );
#if 0
		emit elementChanged( number - 1 );
#endif
	}
}

#include "detailinfodlg.moc"
