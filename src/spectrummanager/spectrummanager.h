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
#ifndef SPECTRUMMANAGER_H
#define SPECTRUMMANAGER_H

#include <kmainwindow.h>

#include "../spectrum.h"

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class SpectrumManager : public KMainWindow
{
	Q_OBJECT

	public:
		/**
		 * public contructor
		 */
		SpectrumManager();

	private slots:
		void slotPrint();
		void slotSave();
		void slotSaveas();
		void addBandToSpectrum( Spectrum::band );

	private:
		QWidget *bandEditor;
		Spectrum *m_spectrum;

		QString bandData( Spectrum::band band );
};
#endif // SPECTRUMMANAGER_H

