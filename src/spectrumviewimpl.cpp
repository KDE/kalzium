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

#include <kdebug.h>

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

	resize( minimumSizeHint() );
}

void SpectrumViewImpl::fillPeakList()
{
    int row;
    peakListTable->setRowCount(m_spectrumWidget->spectrum()->peaklist().count());

    for(int i = 0; i <  m_spectrumWidget->spectrum()->peaklist().count(); i++ )
    {
        Spectrum::peak * peak = m_spectrumWidget->spectrum()->peaklist().at(i);
	row = i + 1;
        peakListTable->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(row)));
        peakListTable->setItem ( i, 0, new QTableWidgetItem(QString::number(peak->wavelength)) );
	peakListTable->setItem ( i, 1, new QTableWidgetItem(QString::number(peak->intensity)) );
    }
}


void SpectrumViewImpl::updateUI(int l, int r)
{
    minimumValue->setValue(l);
    maximumValue->setValue(r);
}

void SpectrumViewImpl::updatePeakInformation(Spectrum::peak * peak )
{
    intensity_label->setText(i18n("%1 of 1000", peak->intensity));
    wavelength_label->setText(i18n("%1 Å", peak->wavelength));
}


#include "spectrumviewimpl.moc"
