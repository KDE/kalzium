/***************************************************************************
copyright            : (C) 2005 by Carsten Niehaus
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
#include <spectrummanager.h>

#include "spectrumeditor.h"
#include "spectrumparser.h"
#include "bandeditor.h"
#include "smprintdialogpage.h"

#include <kapplication.h>
#include <kaction.h>
#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kprinter.h>

#include <QPainter>
#include <QFileDialog>

	SpectrumManager::SpectrumManager()
: KMainWindow( 0 )
{
	KStdAction::print(  this, SLOT(  slotPrint() ), actionCollection() );
	KStdAction::save(  this, SLOT(  slotSave() ), actionCollection() );
	KStdAction::saveAs(  this, SLOT(  slotSaveas() ), actionCollection() );
	KStdAction::quit(  kapp, SLOT ( closeAllWindows() ),actionCollection() );

	setXMLFile("spectrummanagerui.rc" );
	setupGUI();

	SpectrumEditor *editor = new SpectrumEditor( this );	
	setCentralWidget( editor );

	m_spectrum = new Spectrum();

	connect( editor, SIGNAL( bandAdded( Spectrum::band ) ), 
			this, SLOT( addBandToSpectrum( Spectrum::band ) ) );
}

void SpectrumManager::addBandToSpectrum( Spectrum::band* band ){
	m_spectrum->addBand( band );
}

void SpectrumManager::slotSave()
{}

void SpectrumManager::slotSaveas()
{
//	QString filename = KFileDialog::getSaveFileName();

QString	filename = "/home/carsten/svn/trunk/KDE/kdeedu/kalzium/src/spectrummanager/test.txt";
	
	QFile file( filename );
	if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
		return;

	QTextStream out( &file );

	QList<Spectrum::band*> bL = m_spectrum->bandlist();

	out << "Wavelength,aki,energy1,energy2,intensity,electronconfig1,electronconfig2,term1,term2,J1,J2" << "\n";

	foreach( Spectrum::band* band, bL )
	{
	( void )band;	
	}
}

QString SpectrumManager::bandData( Spectrum::band band )
{
	QString line;
	
	line+= QString::number( band.wavelength ) + ",";
	line+= QString::number( band.aki ) + ",";
	line+= QString::number( band.energy1 ) + ",";
	line+= QString::number( band.energy2 ) + ",";
	line+= QString::number( band.intensity ) + ",";
	line+= band.electronconfig1  + ",";
	line+= band.electronconfig2  + ",";
	line+= band.term1  + ",";
	line+= band.term2  + ",";
	line+= band.J1  + ",";
	line+= band.J2;
	
	return line;
}

void SpectrumManager::slotPrint()
{
#warning This doesn't yet work because KPrinter isn't ported to Qt4
	SMPrintDialogPage * p = new SMPrintDialogPage( this );
	
	KPrinter printer;
	printer.setOrientation( KPrinter::Landscape );
	printer.addDialogPage( p );
	printer.setFullPage( true );

	if ( printer.setup( this ) )
	{
		kdDebug() << "SpectrumManager::slotPrint() printing!" << endl;

		QPainter paint;

		paint.begin( &printer );

		paint.drawLine( 0,0,20,20 );
		paint.drawText( 30,40, "hallo" );

		paint.end();
	}
	else
		kdDebug() << "SpectrumManager::slotPrint() Printing Failed!" << endl;
}

#include "spectrummanager.moc"
