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

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kglobalsettings.h>

#include <qlabel.h>
#include <qpainter.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include "detailinfodlg.h"
#include "orbitswidget.h"
#include "detail_chemical.h"
#include "detail_energy.h"
#include "prefs.h"
#include "isotopewidget.h"
#include "detailedgraphicaloverview.h"

DetailedInfoDlg::DetailedInfoDlg( KalziumDataObject *data, Element *el , QWidget *parent, const char *name)
    : KDialogBase(KDialogBase::IconList, "",  Close, Close, parent,name)
{
	m_data = data;
	e = el;

	( actionButton( KDialogBase::Close ) )->setFocus();

	
	
	m_pOverviewTab = addPage(i18n("Overview"), i18n("Overview"), BarIcon( "overview" ));
    m_pMiscTab = addPage(i18n("Miscellaneous"), i18n("Miscellaneous"), BarIcon( "misc" ));
	m_pPictureTab = addPage(i18n("Picture"), i18n("What does this element look like?"), BarIcon( "elempic" ));
	m_pEnergyTab =   addPage( i18n("Energies"), i18n( "Energy Information" ), BarIcon( "energies" ) );
	m_pChemicalTab = addPage( i18n("Chemical Data"), i18n( "Chemical Data" ), BarIcon( "chemical") );
	m_pModelTab = addPage( i18n("Atom Model"), i18n( "Atom Model" ), BarIcon( "orbits" ));
	
	miscLayout = new QVBoxLayout( m_pMiscTab );
	mainLayout = new QVBoxLayout( m_pPictureTab );
	overviewLayout = new QVBoxLayout( m_pOverviewTab );
	energyLayout = new QVBoxLayout( m_pEnergyTab );
	chemicalLayout = new QVBoxLayout( m_pChemicalTab, 0, KDialog::spacingHint() );
	QVBoxLayout *modelLayout = new QVBoxLayout( m_pModelTab , 0, KDialog::spacingHint() );
	
	
	
	dTab = new DetailedGraphicalOverview( m_pOverviewTab, "DetailedGraphicalOverview" );
	dTab->setElement( e );
 	overviewLayout->addWidget( dTab );
	wChemical = new detail_chemical( m_pChemicalTab );
	wEnergy = new detail_energy( m_pEnergyTab );
	wOrbits = new OrbitsWidget( m_pModelTab );
	piclabel = new QLabel( m_pPictureTab );
	discovered_label = new QLabel( m_pMiscTab );
	meanweight_label = new QLabel( m_pMiscTab );
	QWhatsThis::add( meanweight_label , i18n( "The mean weight is the atomic weight divided by the number of protons" ) );
	QWhatsThis::add( discovered_label, i18n( "Here you can see when the element was discovered." ) );
	
	miscLayout->addWidget( discovered_label );
	miscLayout->addWidget( meanweight_label );
	miscLayout->insertStretch(-1,1);
	mainLayout->addWidget( piclabel );
	chemicalLayout->addWidget( wChemical );
	energyLayout->addWidget( wEnergy );
	modelLayout->addWidget( wOrbits );
	
	createContent( e );
}

void DetailedInfoDlg::createContent( Element *el )
{
	
	QString num = QString::number( el->number() );
	QString elname = i18n( el->elname().utf8() );
	QString cap = i18n("For example Carbon (6)" , "%1 (%2)" ).arg( elname ).arg( num );
	setCaption( cap );

	dTab->setElement( el );

	/////////////////////////////////
	discovered_label->setText( i18n("Discovered: %1").arg( el->adjustUnits( Element::DATE ) ) );
	meanweight_label->setText( i18n("Mean weight: %1 u").arg(el->meanweight() ) );

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

	
	////////////////////////////////////7
	

	wEnergy->mp_label->setText( el->adjustUnits( Element::MELTINGPOINT ) );
	wEnergy->bp_label->setText( el->adjustUnits( Element::BOILINGPOINT ) );
	wEnergy->sion_label->setText( el->adjustUnits( Element::IE2 ) );
	wEnergy->ion_label->setText( el->adjustUnits( Element::IE ) );
	wEnergy->en_label->setText(  el->adjustUnits( Element::EN ) );
	
	////////////////////////////////////7
	

	wChemical->orbits_label->setText( el->parsedOrbits() );
	wChemical->symbol_label->setText( el->symbol() );
	wChemical->density_label->setText( el->adjustUnits( Element::DENSITY ) );
	wChemical->block_label->setText( el->block() );
	wChemical->radius_label->setText( el->adjustUnits( Element::RADIUS ) );
	wChemical->weight_label->setText( el->adjustUnits( Element::WEIGHT  ) );

	if ( el->Isotopes() != "0"  )
		wChemical->isotopeWidget->setIsotopes( el->Isotopes() );
	else
		wChemical->isotopeLabel->hide();

	wChemical->isotopeWidget->repaint();
    
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

	createContent( e );
}


#include "detailinfodlg.moc"
