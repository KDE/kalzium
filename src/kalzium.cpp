/*
 * Copyright (C) 2003 Carsten Niehaus <cniehaus@kde.org>
 */

#include "kalzium.h"
#include "pse.h"
#include "elementbutton.h"
#include "element.h"
#include "prefs.h"
#include "settings_colorschemes.h"
#include "settings_colors.h"

#include <qlabel.h>

#include <kmainwindow.h>
#include <kconfigdialog.h>
#include <klocale.h>
#include <kdebug.h>
#include <kaction.h>

Kalzium::Kalzium()
    : KMainWindow( 0, "Kalzium" )
{
	pd = new privatedata( this );

	if (KConfigDialog::showDialog("settings"))
		return;
	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog *dialog = new KConfigDialog(this,"settings", Prefs::self());
	dialog->addPage( new setColorScheme( 0, "colorscheme_page"), i18n("Configure Default Colorscheme"), "colorize");
	dialog->addPage( new setColors( 0, "colors_page"), i18n("Configure Colors"), "colorize");
	dialog->show();

	pd->kalziumData = new KalziumDataObject();

	m_pRegularPSEAction = new KAction(i18n("Show &Regular PSE"), 0, this, SLOT(slotSwitchtoRegularPSE()), actionCollection(), "RegularPSE");
	m_pSimplePSEAction = new KAction(i18n("Show &Simple PSE"), 0, this, SLOT(slotSwitchtoSimplePSE()), actionCollection(), "SimplePSE");
	m_pMendeljevPSEAction = new KAction(i18n("Show &Mendeljev PSE"), 0, this, SLOT(slotSwitchtoMendeljevPSE()), actionCollection(), "MendeljevPSE");

	m_pRegularPSE = new RegularPSE( data(), this, "regularPSE");
	m_pSimplePSE = new SimplifiedPSE( data(), this, "SimplifiedPSE");
	m_pMendeljevPSE = new MendeljevPSE( data(), this, "MendeljevPSE");

	m_pCurrentPSE = m_pRegularPSE;

	// set the shell's ui resource file
	setXMLFile("kalziumui.rc");
	createGUI();

	setCentralWidget( m_pCurrentPSE );
}

void Kalzium::slotSwitchtoMendeljevPSE()
{
	kdDebug() << "slotSwitchtoMendeljev()" << endl;

	m_pCurrentPSE = m_pMendeljevPSE;

	m_pRegularPSE->hide();
	m_pSimplePSE->hide();
	m_pMendeljevPSE->show();

	setCentralWidget( m_pCurrentPSE );
	this->setCaption( centralWidget()->name() );
}

void Kalzium::slotSwitchtoSimplePSE()
{
	kdDebug() << "slotSwitchtoSimplePSE()" << endl;

	m_pCurrentPSE = m_pSimplePSE;

	m_pRegularPSE->hide();
	m_pMendeljevPSE->hide();
	m_pSimplePSE->show();

	setCentralWidget( m_pCurrentPSE );
	this->setCaption( centralWidget()->name() );
}
void Kalzium::slotSwitchtoRegularPSE()
{
	kdDebug() << "slotSwitchtoRegularPSE()" << endl;
	
	m_pCurrentPSE = m_pRegularPSE;
	
	m_pSimplePSE->hide();
	m_pMendeljevPSE->hide();
	m_pRegularPSE->show();
	
	setCentralWidget( m_pCurrentPSE );
	
	this->setCaption( centralWidget()->name() );
}


PSE* Kalzium::currentPSE() const
{
	return m_pCurrentPSE;
}


KalziumDataObject* Kalzium::data() const { return pd->kalziumData; }

Kalzium::~Kalzium()
{
}

#include "kalzium.moc"

