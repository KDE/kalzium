/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
***************************************************************************/
#include "spectrumeditor.h"
#include "../spectrumwidget.h"
#include "../exporter.h"

#include <spectrum.h>

#include <qlayout.h>
#include <qspinbox.h>
#include <qlabel.h>

#include <kdebug.h>

SpectrumEditor::SpectrumEditor( QWidget *parent )
  : QWidget( parent )
{
	ui.setupUi( this );

	m_spectrumWidget = new SpectrumWidget( this );
	m_spectrumWidget->hide();

	connect( ui.addButton, SIGNAL( clicked() ), this, SLOT( slotAddBand() ) );
}

void SpectrumEditor::slotAddBand()
{
	kdDebug() << "SpectrumEditor::slotAddBand()" << endl;
	
	double value = ui.wl->value();

	if ( !( value > 0.0 ) ) return; //invalid wavelength

	double e1 = ui.e1->value();
	double e2 = ui.e2->value();
	double aki = ui.aki->text().toDouble();
	
	QString conf1 = ui.conf1->text();
	QString conf2 = ui.conf2->text();

	QString J1 = ui.j1->text();
	QString J2 = ui.j2->text();
	QString t1 = ui.t1->text();
	QString t2 = ui.t2->text();
	int intensity = ui.intensity->value();
	
	Spectrum::band band;

	band.wavelength = value;
	band.aki = aki;
	band.energy2= e2;
	band.energy1= e1;
	band.intensity = intensity;
	band.term2 = t2;
	band.term1 = t1;
	band.J1 = J1;
	band.J2 = J2;
	band.intensity = intensity;
	band.electronconfig1 = conf1;
	band.electronconfig2 = conf2;

	emit bandAdded( band );
}


#include "spectrumeditor.moc"
