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
#include <klocalizedstring.h>

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

    peakListTable->setColumnCount(2);

    QStringList headers = QStringList() << i18n("Wavelength") << i18n("Intensity");
    peakListTable->setHeaderLabels(headers);

    peakListTable->setRootIsDecorated(false);
    peakListTable->setSortingEnabled(true);

    QList<int> length;
    length << KUnitConversion::Nanometer << KUnitConversion::Angstrom;
    m_lengthUnit->setUnitList(length);

    resize( minimumSizeHint() );
}

void SpectrumViewImpl::fillPeakList()
{
    peakListTable->clear();

    QList<QTreeWidgetItem *> items;

    for (int i = 0; i <  m_spectrumWidget->spectrum()->peaklist().count(); i++ )
    {
        Spectrum::peak * peak = m_spectrumWidget->spectrum()->peaklist().at(i);

        double peakWavelength = peak->wavelengthToUnit( KUnitConversion::Nanometer );

        QStringList row = QStringList() << QString::number(peakWavelength)
                          << QString::number(peak->intensity);

        items.append(new QTreeWidgetItem((QTreeWidget*)0, row));

    }
    peakListTable->insertTopLevelItems(0, items);
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
    double peakWavelength = peak->wavelengthToUnit( KUnitConversion::Nanometer );

    QList<QTreeWidgetItem *> foundItems = peakListTable->findItems( QString::number(peakWavelength) , Qt::MatchExactly );

    if ( !foundItems.isEmpty() ) {
        foundItems.first()->setBackground( 0, QBrush(Qt::red) );
        foundItems.first()->setBackground( 1, QBrush(Qt::red) );
    }
}


#include "spectrumviewimpl.moc"
