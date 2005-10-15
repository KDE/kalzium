#ifndef SPECTRUMPARSER_H
#define SPECTRUMPARSER_H
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

#include <QFile>

class Spectrum;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class SpectrumParser
{
	public:
		/**
		 * Save the Spectrum @p spectrum
		 * @param spectrum the Spectrum to save
		 */
		static void saveSpectrum( Spectrum *spectrum );

		/**
		 * @return the Spectrum specified by the @p id
		 * @param file the file from which the data will be loaded
		 * @param id The id of the Spectrum
		 */
		static Spectrum* loadSpectrum( QFile *file, const QString& id );
};
#endif // SPECTRUMPARSER_H

