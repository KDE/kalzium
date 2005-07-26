#include "spectrumviewimpl.h"

SpectrumViewImpl::SpectrumViewImpl( QWidget *parent, const char* name )
	: SpectrumView( parent, name )
{
}

void SpectrumViewImpl::slotExportAsImage()
{
//X 	Exporter* exporter = new Exporter();
//X 	QString fileName = KFileDialog::getSaveFileName(
//X 	                        QString::null, exporter->supportedImageFormats(),
//X 	                        this, i18n( "Save Spectrum" ) );
//X 	if( !fileName.isEmpty() )
//X 	{
//X 		if ( !exporter->saveAsImage( &m_spectrumWidget->pixmap(), fileName ) )
//X 			KMessageBox::error( this, i18n( "The spectrum could not be saved" ), i18n( "Image could not be saved") );
//X 	}	
//X 	delete exporter;
}

void SpectrumViewImpl::slotBordersChanged( int l, int r )
{}

#include "spectrumviewimpl.moc"
