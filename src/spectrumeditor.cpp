/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
***************************************************************************/
#include "spectrumeditor.h"
#include "spectrum.h"
#include "spectrumwidget.h"
#include "exporter.h"
#include "bandeditor.h"

#include <qlayout.h>
#include <qspinbox.h>
#include <qlabel.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <kglobal.h>
#include <kguiitem.h>
#include <kpushbutton.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>
#include <knuminput.h>
#include <klineedit.h>

SpectrumEditor::SpectrumEditor( QWidget *parent, const char* name )
  : KDialogBase(parent, name, true, i18n( "Spectrum Editor" ),
                Help|User1|Close, NoDefault, true,
                KGuiItem( i18n( "&Export Spectrum as Image" ), "fileexport" ) )
{
	m_bandEditor = new bandEditor( this, "m_bandEditor" );
	Spectrum *spectrum = new Spectrum();
	m_bandEditor->m_spectrumWidget->setSpectrum( spectrum );
	m_bandEditor->m_spectrumWidget->setBorders( 380, 780 );

	const int widthOfZero = m_bandEditor->spinMin->fontMetrics().width( '0' );
	m_bandEditor->spinMin->setMinimumWidth( widthOfZero * 10 );
	m_bandEditor->spinMin->setMaximumWidth( widthOfZero * 12 );
	m_bandEditor->spinMax->setMinimumWidth( widthOfZero * 10 );
	m_bandEditor->spinMax->setMaximumWidth( widthOfZero * 12 );

	setMainWidget( m_bandEditor );
	
	setMinimumSize( 500, 450 );
	
	connect( m_bandEditor->spinMin, SIGNAL( valueChanged( int ) ), m_bandEditor->m_spectrumWidget, SLOT( setRightBorder( int ) ) );
	connect( m_bandEditor->spinMax, SIGNAL( valueChanged( int ) ), m_bandEditor->m_spectrumWidget, SLOT( setLeftBorder( int ) ) );
	connect( m_bandEditor->addButton, SIGNAL( clicked() ), this, SLOT( slotAddBand( ) ) );
}

void SpectrumEditor::slotAddBand()
{
	double value = m_bandEditor->wl->value();

	if ( !( value > 0.0 ) ) return; //invalid wavelength

	double e1 = m_bandEditor->e1->value();
	double e2 = m_bandEditor->e2->value();
	double aki = m_bandEditor->aki->value();
	
	QString conf1 = m_bandEditor->conf1->text();
	QString conf2 = m_bandEditor->conf2->text();

	QString J1 = m_bandEditor->j1->text();
	QString J2 = m_bandEditor->j2->text();
	QString t1 = m_bandEditor->t1->text();
	QString t2 = m_bandEditor->t2->text();
	int intensity = m_bandEditor->intensity->value();
	
	Spectrum::band band;

	band.wavelength = value;
	band.aki = aki;
	band.energy2= e2;
	band.energy1= e1;
	band.intensity = intensity;
	band.term2 = t2;
	band.term1 = t1;
	band.J1 = J1;
	band.J2 = J2;
	band.intensity = intensity;
	band.electronconfig1 = conf1;
	band.electronconfig2 = conf2;
	
	m_bandEditor->m_spectrumWidget->spectrum()->addBand( band );
	m_bandEditor->m_spectrumWidget->update();
}

void SpectrumEditor::slotUser1()
{
	Exporter* exporter = new Exporter();
	QString fileName = KFileDialog::getSaveFileName(
	                        QString::null, exporter->supportedImageFormats(),
	                        this, i18n( "Save Spectrum" ) );
	if( !fileName.isEmpty() )
	{
		if ( !exporter->saveAsImage( &m_bandEditor->m_spectrumWidget->pixmap(), fileName ) )
			KMessageBox::error( this, i18n( "The spectrum could not be saved" ), i18n( "Image could not be saved") );
	}	
	delete exporter;
}

#include "spectrumeditor.moc"
