/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus <cniehaus@kde.org>        *
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

#include "elementparser.h"
#include "element.h"
#include "chemicaldataobject.h"
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2) {
        std::cout << "Usage: elements <XML_FILE>\n";
        return 1;
    }

    ElementSaxParser * parser = new ElementSaxParser();
    QFile xmlFile(argv[1]);

    QXmlInputSource source(&xmlFile);
    QXmlSimpleReader reader;

    reader.setContentHandler(parser);
    reader.parse(source);

    QList<Element*> v = parser->getElements();

    std::cout << "Found " << v.count() << " elements." << std::endl;

    foreach (Element *e, v) {
        if (e) {
            QList<ChemicalDataObject> list = e->data();

            //Test: give me all data available
            foreach (const ChemicalDataObject &o, list) {
                QString unit = o.unitAsString();
                if (unit == QLatin1String("bo:noUnit")) {
                    unit = QLatin1String("");
                }
                qDebug() << "Name: " << o.type() << " " << o.valueAsString() << " " << unit;
            }
        }
    }

    delete parser;
    qDeleteAll(v);

    return 0;
}

// QString dictRef( int type ) const
// {
//     QString botype;
//     switch (type) {
//     case atomicNumber:
//         botype = "atomicNumber";
//         break;
//     case symbol:
//         botype = "symbol";
//         break;
//     case name:
//         botype = "name";
//         break;
//     case mass:
//         botype = "mass";
//         break;
//     case exactMass:
//         botype = "exactMass";
//         break;
//     case spin:
//         botype = "spin";
//         break;
//     case magneticMoment:
//         botype = "magneticMoment";
//         break;
//     case halfLife:
//         botype = "halfLife";
//         break;
//     case alphaDecay:
//         botype = "alphaDecay";
//         break;
//     case alphaDecayLikeliness:
//         botype = "alphaDecayLikeliness";
//         break;
//     case betaminusDecayLikeliness:
//         botype = "betaminusDecayLikeliness";
//         break;
//     case betaminusDecay:
//         botype = "betaminusDecay";
//         break;
//     case betaplusDecayLikeliness:
//         botype = "betaplusDecayLikeliness";
//         break;
//     case betaplusDecay:
//         botype = "betaplusDecay";
//         break;
//     case ecDecayLikeliness:
//         botype = "ecDecayLikeliness";
//         break;
//     case ecDecay:
//         botype = "ecDecay";
//         break;
//     case ionization:
//         botype = "ionization";
//         break;
//     case electronAffinity:
//         botype = "electronAffinity";
//         break;
//     case electronegativityPauling:
//         botype = "electronegativityPauling";
//         break;
//     case radiusCovalent:
//         botype = "radiusCovalent";
//         break;
//     case radiusVDW:
//         botype = "radiusVDW";
//         break;
//     case meltingpoint:
//         botype = "meltingpoint";
//         break;
//     case boilingpoint:
//         botype = "boilingpoint";
//         break;
//     case periodTableBlock:
//         botype = "periodTableBlock";
//         break;
//     case nameOrigin:
//         botype = "nameOrigin";
//         break;
//     case orbit:
//         botype = "orbit";
//         break;
//     case date:
//         botype = "date";
//         break;
//     case discoverers:
//         botype = "discoverers";
//         break;
//     case period:
//         botype = "period";
//         break;
//     case relativeAbundance:
//         botype = "relativeAbundance";
//         break;
//     case family:
//         botype ="family";
//         break;
//     case group:
//         botype ="group";
//         break;
//     case acidicbehaviour:
//         botype ="acidicbehaviour";
//         break;
//     case electronicConfiguration:
//         botype ="electronicConfiguration";
//         break;
//     case crystalstructure:
//         botype ="crystalstructure";
//         break;
//     case dangerSymbol:
//         botype ="dangerSymbol";
//         break;
//     case RPhrase:
//         botype ="RPhrase";
//         break;
//     case SPhrase:
//         botype ="SPhrase";
//         break;
//     case discoveryCountry:
//         botype ="discoveryCountry";
//         break;
//     }
//
//     botype = botype.prepend( QLatin1String("bo:") );
//
//     return botype;
// }
