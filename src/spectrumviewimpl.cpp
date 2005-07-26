#include "spectrumviewimpl.h"

#include <qspinbox.h>

#include <kfiledialog.h>
#include <klocale.h>
#include <kmessagebox.h>

#include "exporter.h"

SpectrumViewImpl::SpectrumViewImpl( QWidget *parent, const char* name )
	: SpectrumView( parent, name )
{
	resize( minimumSizeHint() );
	// simulating an update
	m_spectrumWidget->setRightBorder( maximumValue->value() );
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
			KMessageBox::error( this, i18n( "The spectrum could not be saved" ), i18n( "Image could not be saved") );
	}	
	delete exporter;
}

void SpectrumViewImpl::slotBordersChanged( int l, int r )
{
	(void)l;
	(void)r;
}

#include "spectrumviewimpl.moc"
