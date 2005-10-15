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

#include <QString>
#include <QList>
#include <QStringList>
#include <qdom.h>

class Element;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class ElementParser
{
	public:
		/**
		 * @return the Element with the symbol symbol
		 */
		static Element* loadElement( const QString& symbol, const QDomDocument& dataDocument );
		
		static Element* loadElement( const QDomElement& element );

		/**
		 * @return a QStringList with the symbols of all known elements
		 */
		static QStringList loadElementSymbols(const QDomDocument& dataDocument);

		/**
		 * @return all chemical elements in the xml-file
		 */
		static QList<Element*> loadAllElements(const QDomDocument& dataDocument);
};

