/***************************************************************************
copyright            : (C) 2005, 2008 by Carsten Niehaus
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
#include "spectrumparser.h"

#include "spectrum.h"

#include <QDebug>
#include <QFile>
#include <QList>

class SpectrumParser::Private
{
public:
    Private()
            : currentSpectrum(nullptr),
            inMetadata_(false),
            inSpectrum_(false),
            inSpectrumList_(false),
            inPeakList_(false),
            inPeak_(false)
    {}

    ~Private()
    {
        delete currentSpectrum;
        delete currentPeak;
    }

    Spectrum * currentSpectrum;
    Spectrum::peak * currentPeak;

    bool inMetadata_;
    bool inSpectrum_;
    bool inSpectrumList_;
    bool inPeakList_;
    bool inPeak_;

    double wavelength;
    int intensity;

    QList<Spectrum*> spectra;
};

SpectrumParser::SpectrumParser()
        : QXmlDefaultHandler(), d(new Private)
{
}

SpectrumParser::~SpectrumParser()
{
    delete d;
}

bool SpectrumParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
    if (localName == QLatin1String("spectrum")) {

        d->currentSpectrum = new Spectrum();
        d->inSpectrum_ = true;

        //now save the element of the current spectrum
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == QLatin1String("id")) {
                currentElementID = attrs.value(i);
            }
        }

    } else if (d->inSpectrum_ && localName == QLatin1String("peakList")) {
        d->inPeakList_ = true;
    } else if (d->inSpectrum_ && d->inPeakList_ && localName == QLatin1String("peak")) {
        d->inPeak_ = true;
        for (int i = 0; i < attrs.length(); ++i) {
            if (attrs.localName(i) == QLatin1String("xValue")) {
                d->intensity = attrs.value(i).toInt();
            } else if (attrs.localName(i) == QLatin1String("yValue")) {
                d->wavelength = attrs.value(i).toDouble();
            }
        }
        d->currentPeak = new Spectrum::peak(d->wavelength, d->intensity);
    }
    return true;
}

bool SpectrumParser::endElement(const QString&, const QString& localName, const QString &)
{
    if (localName == QLatin1String("spectrum")) {
        int num = currentElementID.midRef(1).toInt();
        d->currentSpectrum->setParentElementNumber(num);

        d->spectra.append(d->currentSpectrum);

        d->currentSpectrum = nullptr;
        d->inSpectrum_ = false;
    } else if (localName == QLatin1String("peakList")) {
        d->inSpectrumList_ = false;
    } else if (localName == QLatin1String("peak")) {
//X             qDebug() << "in 'peak'" << " with this data: " << d->currentPeak->intensity << " (intensity)" ;
        d->currentSpectrum->addPeak(d->currentPeak);
        d->currentPeak = nullptr;
        d->inPeak_ = false;
    }
    return true;
}

bool SpectrumParser::characters(const QString &ch)
{
    Q_UNUSED(ch)
    return true;
}

QList<Spectrum*> SpectrumParser::getSpectrums()
{
    return d->spectra;
}
