/***************************************************************************
copyright            : (C) 2005-2008 by Carsten Niehaus
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

#include "isotopeparser.h"

#include "chemicaldataobject.h"
#include "isotope.h"

#include <QDebug>

#include <KUnitConversion/Converter>

class IsotopeParser::Private
{
public:
    Private()
    :    currentUnit(KUnitConversion::NoUnit),
    currentErrorValue(QVariant()),
    currentElementSymbol(QString()),
    currentIsotope(nullptr),
    inIsotope(false),
    inElement(false),
    inAtomicNumber(false),
    inExactMass(false),
    inSpin(false),
    inMagMoment(false),
    inHalfLife(false),
    inAlphaDecayLikeliness(false),
    inAlphaDecay(false),
    inAlphaBetaminusDecayLikeliness(false),
    inAlphaBetaminusDecay(false),
    inBetaplusDecayLikeliness(false),
    inBetaplusDecay(false),
    inBetaplusProtonDecayLikeliness(false),
    inBetaplusProtonDecay(false),
    inBetaplusAlphaDecayLikeliness(false),
    inBetaplusAlphaDecay(false),
    inBetaminusDecayLikeliness(false),
    inBetaminusDecay(false),
    inBetaminusFissionDecayLikeliness(false),
    inBetaminusFissionDecay(false),
    inBetaminusAlphaDecayLikeliness(false),
    inBetaminusAlphaDecay(false),
    inBetaminusNeutronDecayLikeliness(false),
    inBetaminusNeutronDecay(false),
    inECDecayLikeliness(false),
    inECDecay(false),
    inProtonDecayLikeliness(false),
    inProtonDecay(false),
    inProtonAlphaDecayLikeliness(false),
    inProtonAlphaDecay(false),
    inNeutronDecayLikeliness(false),
    inNeutronDecay(false),
    inAbundance(false)
    {
    }

  ~Private()
  {
    delete currentIsotope;
    //qDeleteAll(isotopes);
  }

    ChemicalDataObject currentDataObject;
    int currentUnit;
    QVariant currentErrorValue;
    QString currentElementSymbol;
    Isotope* currentIsotope;

    QList<Isotope*> isotopes;

    bool inIsotope;
    bool inElement;
    bool inAtomicNumber;
    bool inExactMass;
    bool inSpin;
    bool inMagMoment;
    bool inHalfLife;
    bool inAlphaDecayLikeliness;
    bool inAlphaDecay;
    bool inAlphaBetaminusDecayLikeliness;
    bool inAlphaBetaminusDecay;
    bool inBetaplusDecayLikeliness;
    bool inBetaplusDecay;
    bool inBetaplusProtonDecayLikeliness;
    bool inBetaplusProtonDecay;
    bool inBetaplusAlphaDecayLikeliness;
    bool inBetaplusAlphaDecay;
    bool inBetaminusDecayLikeliness;
    bool inBetaminusDecay;
    bool inBetaminusNeutronDecayLikeliness;
    bool inBetaminusNeutronDecay;
    bool inBetaminusFissionDecayLikeliness;
    bool inBetaminusFissionDecay;
    bool inBetaminusAlphaDecayLikeliness;
    bool inBetaminusAlphaDecay;
    bool inECDecayLikeliness;
    bool inECDecay;
    bool inProtonDecayLikeliness;
    bool inProtonDecay;
    bool inProtonAlphaDecayLikeliness;
    bool inProtonAlphaDecay;
    bool inNeutronDecayLikeliness;
    bool inNeutronDecay;    
    bool inAbundance;
};

IsotopeParser::IsotopeParser()
    : QXmlDefaultHandler(), d(new Private)
{
}

IsotopeParser::~IsotopeParser()
{
    delete d;
}

bool IsotopeParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
    if (localName == QLatin1String("isotopeList")) {
        d->inElement = true;

        //now save the symbol of the current element
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == QLatin1String("id")) {
                d->currentElementSymbol = attrs.value(i);
            }
        }
    } else if (d->inElement && localName == QLatin1String("isotope")) {
        d->currentIsotope = new Isotope();
        d->currentIsotope->addData(ChemicalDataObject(QVariant(d->currentElementSymbol), ChemicalDataObject::symbol));
        d->inIsotope = true;
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == QLatin1String("number")) {
                d->currentIsotope->setNucleons(attrs.value(i).toInt());
            }
        }
    } else if (d->inIsotope && localName == QLatin1String("scalar")) {
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == QLatin1String("errorValue")) {
                d->currentErrorValue = QVariant(attrs.value(i));
                continue;
            }

            if (attrs.value(i) == QLatin1String("bo:atomicNumber")) {
                d->inAtomicNumber = true;
            } else if (attrs.value(i) == QLatin1String("bo:exactMass")) {
                d->inExactMass = true;
            } else if (attrs.value(i) == QLatin1String("bo:halfLife")) {
                for (int i = 0; i < attrs.length(); ++i) {
                    if (attrs.localName(i) == QLatin1String("units")) {
                        if (attrs.value(i) == QLatin1String("siUnits:s")) {
                            d->currentUnit = KUnitConversion::Second;
                        } else if (attrs.value(i) == QLatin1String("units:y")) {
                            d->currentUnit = KUnitConversion::Year;
                        } else {
                            d->currentUnit = KUnitConversion::NoUnit;
                        }
                    }
                }

                d->currentDataObject.setUnit(d->currentUnit);
                d->inHalfLife = true;
            } else if (attrs.value(i) == QLatin1String("bo:alphaDecay")) {
                d->inAlphaDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:alphaDecayLikeliness")) {
                d->inAlphaDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:alphabetaminusDecay")) {
                d->inAlphaBetaminusDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:alphabetaminusDecayLikeliness")) {
                d->inAlphaBetaminusDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:ecDecay")) {
                d->inECDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:ecDecayLikeliness")) {
                d->inECDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:neutronDecay")) {
                d->inNeutronDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:neutronDecayLikeliness")) {
                d->inNeutronDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:protonDecay")) {
                d->inProtonDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:protonDecayLikeliness")) {
                d->inProtonDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:protonalphaDecay")) {
                d->inProtonAlphaDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:protonalphaDecayLikeliness")) {
                d->inProtonAlphaDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusDecay")) {
                d->inBetaminusDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusDecayLikeliness")) {
                d->inBetaminusDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusneutronDecay")) {
                d->inBetaminusNeutronDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusneutronDecayLikeliness")) {
                d->inBetaminusNeutronDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusfissionDecay")) {
                d->inBetaminusFissionDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusfissionDecayLikeliness")) {
                d->inBetaminusFissionDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusfissionDecay")) {
                d->inBetaminusFissionDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaminusfissionDecayLikeliness")) {
                d->inBetaminusFissionDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaplusDecay")) {
                d->inBetaplusDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaplusDecayLikeliness")) {
                d->inBetaplusDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaplusprotonDecay")) {
                d->inBetaplusDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaplusprotonDecayLikeliness")) {
                d->inBetaplusDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaplusalphaDecay")) {
                d->inBetaplusDecay = true;
            } else if (attrs.value(i) == QLatin1String("bo:betaplusalphaDecayLikeliness")) {
                d->inBetaplusDecayLikeliness = true;
            } else if (attrs.value(i) == QLatin1String("bo:spin")) {
                d->inSpin = true;
            } else if (attrs.value(i) == QLatin1String("bo:magneticMoment")) {
                d->inMagMoment = true;
            } else if (attrs.value(i) == QLatin1String("bo:relativeAbundance")) {
                d->inAbundance = true;
            }
        }
    }
    return true;
}

bool IsotopeParser::endElement(const QString&, const QString& localName, const QString&)
{
    if (localName == QLatin1String("isotope")) {
        d->isotopes.append(d->currentIsotope);

        d->currentIsotope = nullptr;
        d->inIsotope = false;
    } else if (localName == QLatin1String("isotopeList")) { //a new list of isotopes start...
        d->inElement = false;
    }

    return true;
}

bool IsotopeParser::characters(const QString &ch)
{
    ChemicalDataObject::BlueObelisk type;
    QVariant value;

    if (d->inExactMass) {
        value = ch.toDouble();
        type = ChemicalDataObject::exactMass;
        d->inExactMass = false;
    } else if (d->inAtomicNumber) {
        value = ch.toInt();
        type = ChemicalDataObject::atomicNumber;
        d->inAtomicNumber = false;
    } else if (d->inSpin) {
        value = ch;
        type = ChemicalDataObject::spin;
        d->inSpin = false;
    } else if (d->inMagMoment) {
        value = ch;
        type = ChemicalDataObject::magneticMoment;
        d->inMagMoment = false;
    } else if (d->inHalfLife) {
        value = ch.toDouble();
        type = ChemicalDataObject::halfLife;
        d->inHalfLife = false;
    } else if (d->inAlphaDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::alphaDecay;
        d->inAlphaDecay = false;
    } else if (d->inAlphaDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::alphaDecayLikeliness;
        d->inAlphaDecayLikeliness = false;
    } else if (d->inAlphaBetaminusDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::alphabetaminusDecay;
        d->inAlphaBetaminusDecay = false;
    } else if (d->inAlphaBetaminusDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::alphabetaminusDecayLikeliness;
        d->inAlphaBetaminusDecayLikeliness = false;
    } else if (d->inBetaplusDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusDecay;
        d->inBetaplusDecay = false;
    } else if (d->inBetaplusDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusDecayLikeliness;
        d->inBetaplusDecayLikeliness = false;
    } else if (d->inBetaplusProtonDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusprotonDecay;
        d->inBetaplusProtonDecay = false;
    } else if (d->inBetaplusProtonDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusprotonDecayLikeliness;
        d->inBetaplusProtonDecayLikeliness = false;
    } else if (d->inBetaplusAlphaDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusalphaDecay;
        d->inBetaplusAlphaDecay = false;
    } else if (d->inBetaplusAlphaDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusalphaDecayLikeliness;
        d->inBetaplusAlphaDecayLikeliness = false;
    } else if (d->inBetaminusDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusDecay;
        d->inBetaminusDecay = false;
    } else if (d->inBetaminusDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusDecayLikeliness;
        d->inBetaminusDecayLikeliness = false;
    } else if (d->inBetaminusNeutronDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusneutronDecay;
        d->inBetaminusNeutronDecay = false;
    } else if (d->inBetaminusNeutronDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusneutronDecayLikeliness;
        d->inBetaminusNeutronDecayLikeliness = false;
    } else if (d->inBetaminusFissionDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusfissionDecay;
        d->inBetaminusFissionDecay = false;
    } else if (d->inBetaminusFissionDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusfissionDecayLikeliness;
        d->inBetaminusFissionDecayLikeliness = false;
    } else if (d->inBetaminusAlphaDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusalphaDecay;
        d->inBetaminusAlphaDecay = false;
    } else if (d->inBetaminusAlphaDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusalphaDecayLikeliness;
        d->inBetaminusAlphaDecayLikeliness = false;
    } else if (d->inECDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::ecDecayLikeliness;
        d->inECDecayLikeliness = false;
    } else if (d->inECDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::ecDecay;
        d->inECDecay = false;
    } else if (d->inNeutronDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::neutronDecayLikeliness;
        d->inNeutronDecayLikeliness = false;
    } else if (d->inNeutronDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::neutronDecay;
        d->inNeutronDecay = false;
    } else if (d->inProtonDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::protonDecayLikeliness;
        d->inProtonDecayLikeliness = false;
    } else if (d->inProtonDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::protonDecay;
        d->inProtonDecay = false;
    } else if (d->inProtonAlphaDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::protonalphaDecayLikeliness;
        d->inProtonAlphaDecayLikeliness = false;
    } else if (d->inProtonAlphaDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::protonalphaDecay;
        d->inProtonAlphaDecay = false;
    } else if (d->inAbundance) {
        value = ch;
        type = ChemicalDataObject::relativeAbundance;
        d->inAbundance = false;
    } else { //it is a non known value. Do not create a wrong object but return
        return true;
    }

    if (type == ChemicalDataObject::exactMass) {
        d->currentDataObject.setErrorValue(d->currentErrorValue);
    }

    d->currentDataObject.setData(value);
    d->currentDataObject.setType(type);

    if (d->currentIsotope) {
        d->currentIsotope->addData(d->currentDataObject);
    }

    return true;
}

QList<Isotope*> IsotopeParser::getIsotopes()
{
    return d->isotopes;
}
