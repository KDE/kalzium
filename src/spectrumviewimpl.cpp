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
	
	resize( minimumSizeHint() );
}

void SpectrumViewImpl::updateUI(int l, int r)
{
    minimumValue->setValue(l);
    maximumValue->setValue(r);
    minimumValue->setRange(l,r-1);
    maximumValue->setRange(l+1,r);
}

#include "spectrumviewimpl.moc"
