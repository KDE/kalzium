/***************************************************************************
    copyright            : (C) 2005, 2006 by Carsten Niehaus
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

#ifndef SPECTRUMVIEWIMPL_H
#define SPECTRUMVIEWIMPL_H

#include "spectrum.h"

#include "ui_spectrumview.h"

/**
 * @author Carsten Niehaus
 */
class SpectrumViewImpl : public QWidget, Ui_SpectrumView
{
    Q_OBJECT

public:
    /**
     * @param parent the parent widget
     */
    SpectrumViewImpl(QWidget* parent);

    /**
     * sets the spectrum to @p spec
     * @param spec the spectrum to display
     */
    void setSpectrum(Spectrum* spec)
    {
        m_spectrumWidget->setSpectrum(spec);

        fillPeakList();

        m_spectrumWidget->update();
    }

Q_SIGNALS:
    void settingsChanged();

private slots:
    /**
     * set the correct ranges and min/max values of the
     * GUI elements
     * @param left The left border of the spectrum
     * @param right The right border of the spectrum
     */
    void updateUI(int left, int right);

    void updatePeakInformation(Spectrum::peak * peak);

    void setUnit();

    void updateMin(int left);
    void updateMax(int right);

private:
    /**
     * filling the list of peaks
     */
    void fillPeakList();
};

#endif // SPECTRUMVIEWIMPL_H
