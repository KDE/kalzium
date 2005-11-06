#ifndef ISOTOPEPARSER_H
#define ISOTOPEPARSER_H
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

#include <QList>

#include <qxml.h>

#include "chemicaldataobject.h"

class Isotope;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class IsotopeParser : public QXmlDefaultHandler
{
	public:
		/**
		 * Constructor
		 */
		IsotopeParser();
		bool startElement( const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs );

		bool endElement( const QString& namespaceURI, const QString &localName, const QString& qName );
		
		bool characters(const QString &ch);

		QList<Isotope*> getIsotopes();

	private:
		ChemicalDataObject *currentDataObject_;
		ChemicalDataObject::BlueObeliskUnit currentUnit_;

		QVariant currentErrorValue_;

		QString currentElementSymbol_;
		
		Isotope* currentIsotope_;
		QList<Isotope*> isotopes_;
		bool inIsotope_;
		bool inAtomicNumber_,
			 inExactMass_;
		bool inAlphaPercentage_;
		bool inAlphaDecay_;
		bool inBetaplusPercentage_;
		bool inBetaplusDecay_;
		bool inBetaminusPercentage_;
		bool inBetaminusDecay_;
		bool inECPercentage_;
		bool inECDecay_;
		bool inAbundance_;
};
#endif // ISOTOPEPARSER_H

