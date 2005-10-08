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
		 * public contructor
		 * @param file The file which to read or write
		 */
		SpectrumParser(const QFile& file);
		
		/**
		 * public contructor
		 * @param filename the filename of the xml-file
		 */
		SpectrumParser(const QString& filename);

		void saveSpectrum( Spectrum *spectrum );

		Spectrum* loadSpectrum( QFile *file );

//	private:
		QFile m_file;
};
#endif // SPECTRUMPARSER_H

