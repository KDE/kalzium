/***************************************************************************
    copyright            : (C) 2005, 2006 by Carsten Niehaus
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
#ifndef SPECTRUMVIEWIMPL_H
#define SPECTRUMVIEWIMPL_H

#include "spectrum.h"

#include "ui_spectrumview.h"

/**
 * @author Carsten Niehaus
 */
class SpectrumViewImpl : public QWidget, Ui_SpectrumView
{
	Q_OBJECT

	public: 
		/**
		 * @param parent the parent widget
		 */
		SpectrumViewImpl( QWidget* parent );

		/**
		 * sets the spectrum to @p spec
		 * @param spec the spectrum to display
		 */
		void setSpectrum( Spectrum* spec ){
			m_spectrumWidget->setSpectrum( spec );
			m_spectrumWidget->update();
		}

	private slots:
		/**
		 * export the spectrum as an image
		 */
		void slotExportAsImage();
};

#endif // SPECTRUMVIEWIMPL_H
