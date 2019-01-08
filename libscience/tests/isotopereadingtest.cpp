/***************************************************************************
 *   Copyright (C) 2005, 2006, 2008 by Carsten Niehaus <cniehaus@kde.org>  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "isotopeparser.h"
#include "isotope.h"
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2) {
        std::cout << "Usage: isotopes <XML_FILE>\n";
        return 1;
    }

    IsotopeParser * parser = new IsotopeParser();
    QFile xmlFile(argv[1]);

    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    QList<Isotope*> v = parser->getIsotopes();

    qDebug() << "Found " << v.count() << " isotopes.";;

    qDebug() << "As a test I am now issuing all isotopes with 50 nuclueons: ";

    foreach (Isotope* i, v) {
        if (i) {
//X             if (i->nucleons() == 50 ) {
//X                 qDebug() << "   Isotope of " << i->parentElementSymbol() << " with a mass of " << i->mass();
//X                 qDebug() << "       Halflife: " << i->halflife() << i->halflifeUnit( );
//X             }
            if (i->parentElementSymbol() == QLatin1String("Ti")) {
                qDebug() << "   Isotope of " << i->parentElementSymbol() << " with a mass of " << i->mass();
                qDebug() << "       Halflife: " << i->halflife() << i->halflifeUnit( );
            }
        }
    }

    delete parser;
    qDeleteAll(v);

    return 0;
}
