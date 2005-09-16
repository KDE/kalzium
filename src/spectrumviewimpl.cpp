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
#include "spectrumviewimpl.h"

#include <qspinbox.h>

#include <kfiledialog.h>
#include <kguiitem.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>

#include "exporter.h"

SpectrumViewImpl::SpectrumViewImpl( QWidget *parent, const char* name )
	: SpectrumView( parent, name )
{
	resize( minimumSizeHint() );
	// simulating an update
	m_spectrumWidget->setRightBorder( maximumValue->value() );
	exportButton->setGuiItem( KGuiItem( i18n( "&Export Spectrum as Image" ), "fileexport" ) );
}

void SpectrumViewImpl::slotExportAsImage()
{
	Exporter* exporter = new Exporter();
	QString fileName = KFileDialog::getSaveFileName(
	                        QString::null, exporter->supportedImageFormats(),
	                        this, i18n( "Save Spectrum" ) );
	if( !fileName.isEmpty() )
	{
		if ( !exporter->saveAsImage( &m_spectrumWidget->pixmap(), fileName ) )
			KMessageBox::error( this, i18n( "The spectrum could not be saved" ), i18n( "Image Could Not Be Saved") );
	}	
	delete exporter;
}
#include "spectrumviewimpl.moc"
