#ifndef SPECTRUMVIEWIMPL_H
#define SPECTRUMVIEWIMPL_H

#include "spectrumview.h"
#include "spectrum.h"
#include "spectrumwidget.h"

/**
 * @author Carsten Niehaus
 * 
 */
class SpectrumViewImpl : public SpectrumView
{
	Q_OBJECT

	public: 
		SpectrumViewImpl( QWidget* parent, const char* name );

		void setSpectrum( Spectrum* spec ){
			m_spectrumWidget->setSpectrum( spec );
		}

	public slots:
		void slotBordersChanged( int, int );
		void slotExportAsImage();
};

#endif // SPECTRUMVIEWIMPL_H
