#ifndef SPECTRUMVIEWIMPL_H
#define SPECTRUMVIEWIMPL_H

#include "spectrumview.h"
#include "spectrum.h"
#include "spectrumwidget.h"

/**
 * @author Carsten Niehaus
 */
class SpectrumViewImpl : public SpectrumView
{
	Q_OBJECT

	public: 
		/**
		 * @param parent the parent widget
		 * @param name the name used internally
		 */
		SpectrumViewImpl( QWidget* parent, const char* name );

		/**
		 * sets the spectrum to @p spec
		 * @param spec the spectrum to display
		 */
		void setSpectrum( Spectrum* spec ){
			m_spectrumWidget->setSpectrum( spec );
		}

	private slots:
		/**
		 * export the spectrum as an image
		 */
		void slotExportAsImage();
};

#endif // SPECTRUMVIEWIMPL_H
