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

#include <QSpinBox>
#include <QTreeWidget>

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
    QList<QTreeWidgetItem *> items;

    int num = 1;
    foreach (Spectrum::peak * peak , m_spectrumWidget->spectrum()->peaklist())
    {
        QStringList l;
        l << QString::number(num);
        l << QString::number(peak->wavelength);
        l << QString::number(peak->intensity);
        items.append(new QTreeWidgetItem((QTreeWidget*)0, l));

        num++;
    }

    peakList->insertTopLevelItems(0, items);
}


void SpectrumViewImpl::updateUI(int l, int r)
{
    minimumValue->setValue(l);
    maximumValue->setValue(r);
    minimumValue->setRange(l,r-1);
    maximumValue->setRange(l+1,r);
}

void SpectrumViewImpl::updatePeakInformation(Spectrum::peak * peak )
{
    intensity_label->setText(i18n("%1 of 1000", peak->intensity));
    wavelength_label->setText(i18n("%1 nm", peak->wavelength));
}


#include "spectrumviewimpl.moc"
