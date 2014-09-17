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
#include <kunitconversion/converter.h>
#include <QDebug>


class IsotopeParser::Private
{
public:
    Private()
    :    currentUnit(KUnitConversion::NoUnit),
    currentErrorValue(QVariant()),
    currentElementSymbol(QString()),
    currentIsotope(0),
    inIsotope(false),
    inElement(false),
    inAtomicNumber(false),
    inExactMass(false),
    inSpin(false),
    inMagMoment(false),
    inHalfLife(false),
    inAlphaDecayLikeliness(false),
    inAlphaDecay(false),
    inBetaplusDecayLikeliness(false),
    inBetaplusDecay(false),
    inBetaminusDecayLikeliness(false),
    inBetaminusDecay(false),
    inECDecayLikeliness(false),
    inECDecay(false),
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
    bool inBetaplusDecayLikeliness;
    bool inBetaplusDecay;
    bool inBetaminusDecayLikeliness;
    bool inBetaminusDecay;
    bool inECDecayLikeliness;
    bool inECDecay;
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
    if (localName == "isotopeList") {
        d->inElement = true;

        //now save the symbol of the current element
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == "id") {
                d->currentElementSymbol = attrs.value(i);
            }
        }
    } else if (d->inElement && localName == "isotope") {
        d->currentIsotope = new Isotope();
        d->currentIsotope->addData(ChemicalDataObject(QVariant(d->currentElementSymbol), ChemicalDataObject::symbol));
        d->inIsotope = true;
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == "number") {
                d->currentIsotope->setNucleons(attrs.value(i).toInt());
            }
        }
    } else if (d->inIsotope && localName == "scalar") {
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == "errorValue") {
                d->currentErrorValue = QVariant(attrs.value(i));
                continue;
            }

            if (attrs.value(i) == "bo:atomicNumber") {
                d->inAtomicNumber = true;
            } else if (attrs.value(i) == "bo:exactMass") {
                d->inExactMass = true;
            } else if (attrs.value(i) == "bo:halfLife") {
                for (int i = 0; i < attrs.length(); ++i) {
                    if (attrs.localName(i) == "units") {
                        if (attrs.value(i) == "siUnits:s") {
                            d->currentUnit = KUnitConversion::Second;
                        } else if (attrs.value(i) == "units:y") {
                            d->currentUnit = KUnitConversion::Year;
                        } else {
                            d->currentUnit = KUnitConversion::NoUnit;
                        }
                    }
                }

                d->currentDataObject.setUnit(d->currentUnit);
                d->inHalfLife = true;
            } else if (attrs.value(i) == "bo:alphaDecay") {
                d->inAlphaDecay = true;
            } else if (attrs.value(i) == "bo:alphaDecayLikeliness") {
                d->inAlphaDecayLikeliness = true;
            } else if (attrs.value(i) == "bo:ecDecay") {
                d->inECDecay = true;
            } else if (attrs.value(i) == "bo:ecDecayLikeliness") {
                d->inECDecayLikeliness = true;
            } else if (attrs.value(i) == "bo:betaminusDecay") {
                d->inBetaminusDecay = true;
            } else if (attrs.value(i) == "bo:betaminusDecayLikeliness") {
                d->inBetaminusDecayLikeliness = true;
            } else if (attrs.value(i) == "bo:betaplusDecay") {
                d->inBetaplusDecay = true;
            } else if (attrs.value(i) == "bo:betaplusDecayLikeliness") {
                d->inBetaplusDecayLikeliness = true;
            } else if (attrs.value(i) == "bo:spin") {
                d->inSpin = true;
            } else if (attrs.value(i) == "bo:magneticMoment") {
                d->inMagMoment = true;
            } else if (attrs.value(i) == "bo:relativeAbundance") {
                d->inAbundance = true;
            }
        }
    }
    return true;
}

bool IsotopeParser::endElement(const QString&, const QString& localName, const QString&)
{
    if (localName == "isotope") {
        d->isotopes.append(d->currentIsotope);

        d->currentIsotope = 0;
        d->inIsotope = false;
    } else if (localName == "isotopeList") { //a new list of isotopes start...
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
    } else if (d->inBetaplusDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusDecay;
        d->inBetaplusDecay = false;
    } else if (d->inBetaplusDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaplusDecayLikeliness;
        d->inBetaplusDecayLikeliness = false;
    } else if (d->inBetaminusDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusDecay;
        d->inBetaminusDecay = false;
    } else if (d->inBetaminusDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::betaminusDecayLikeliness;
        d->inBetaminusDecayLikeliness = false;
    } else if (d->inECDecayLikeliness) {
        value = ch.toDouble();
        type = ChemicalDataObject::ecDecayLikeliness;
        d->inECDecayLikeliness = false;
    } else if (d->inECDecay) {
        value = ch.toDouble();
        type = ChemicalDataObject::ecDecay;
        d->inECDecay = false;
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
