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
#include "spectrumviewimpl.h"

#include <QTreeWidget>
#include <QTableWidget>

#include <kunitconversion/converter.h>
#include <kdebug.h>
#include "kalziumdataobject.h"

SpectrumViewImpl::SpectrumViewImpl( QWidget *parent )
        : QWidget( parent )
{
    setupUi( this );

    connect( minimumValue, SIGNAL( valueChanged( int ) ),
             m_spectrumWidget, SLOT( setLeftBorder( int ) ) );
    connect( maximumValue, SIGNAL( valueChanged( int ) ),
             m_spectrumWidget, SLOT( setRightBorder( int ) ) );
    connect( m_spectrumWidget, SIGNAL( bordersChanged(int,int) ),
             this, SLOT( updateUI(int,int) ) );
    connect(m_spectrumWidget, SIGNAL(peakSelected(Spectrum::peak*)),
            this, SLOT(updatePeakInformation(Spectrum::peak*)));

    QList<int> length;
    length << KUnitConversion::Nanometer << KUnitConversion::Angstrom;
    m_lengthUnit->setUnitList(length);

    resize( minimumSizeHint() );
}

void SpectrumViewImpl::fillPeakList()
{
    peakListTable->setRowCount(m_spectrumWidget->spectrum()->peaklist().count());

    for (int i = 0; i <  m_spectrumWidget->spectrum()->peaklist().count(); i++ )
    {
        Spectrum::peak * peak = m_spectrumWidget->spectrum()->peaklist().at(i);

        peakListTable->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(i + 1)));

        double peakWavelength = KUnitConversion::Value( peak->wavelength, KUnitConversion::Angstrom )
                                .convertTo( KUnitConversion::Nanometer ).number();

        QTableWidgetItem *waveItem = new QTableWidgetItem(QString::number(peakWavelength));
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(peak->intensity));
        waveItem->setFlags(Qt::ItemFlags(Qt::ItemIsEnabled));
        valueItem->setFlags(Qt::ItemFlags(Qt::ItemIsEnabled));
        peakListTable->setItem ( i, 0, waveItem );
        peakListTable->setItem ( i, 1, valueItem );
    }
}

void SpectrumViewImpl::updateUI(int l, int r)
{
    minimumValue->setValue(l);
    minimumValue->setSuffix( KalziumDataObject::instance()->unitAsString( KUnitConversion::Nanometer ) );
    maximumValue->setValue(r);
    maximumValue->setSuffix( KalziumDataObject::instance()->unitAsString( KUnitConversion::Nanometer ) );
}

void SpectrumViewImpl::updatePeakInformation(Spectrum::peak * peak )
{
    QTableWidgetItem *item;

    double peakWavelength = KUnitConversion::Value( peak->wavelength, KUnitConversion::Angstrom )
                            .convertTo( KUnitConversion::Nanometer ).number();

    for (int i = 0; i <  peakListTable->rowCount(); i++ ) {
        item = peakListTable->item(i, 0);

        kDebug()<<item->text() <<  peakWavelength;

        if ( item->text().operator==( QString::number(peakWavelength) ) ) {
            item->setBackgroundColor( Qt::red );
            break;
        }

    }

//     intensity_label->setText(i18n("%1 of 1000", peak->intensity));
//
//     double peakWavelength = KUnitConversion::Value( peak->wavelength, KUnitConversion::Angstrom )
//                             .convertTo( KUnitConversion::Nanometer ).number();
//
//     QString label = QString::number( peakWavelength );
//     label.append(" ");
//     label.append(KalziumDataObject::instance()->unitAsString( KUnitConversion::Nanometer ) );// TODO form settings
//     wavelength_label->setText( label );
}


#include "spectrumviewimpl.moc"
