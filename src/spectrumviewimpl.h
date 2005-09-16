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
