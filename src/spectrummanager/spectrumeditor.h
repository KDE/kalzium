#ifndef SPECTRUMEDITOR_H
#define SPECTRUMEDITOR_H
/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
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
#include <QWidget>

#include "bandeditor.h"
#include "../spectrumwidget.h"

class Spectrum;
class SpectrumWidget;
class bandEditor;

class QSpinBox;
class KPushButton;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class SpectrumEditor : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * public constructor
		 * @param parent the widgets parent
		 */
		SpectrumEditor( QWidget *parent );

	private:
		bandEditor *m_bandEditor;

		Ui::bandEditor ui;

		SpectrumWidget *m_spectrumWidget;
	
	private slots:
		void slotAddBand();

	signals:
		/**
		 * use if a new band is added to the spectrum
		 */
		void bandAdded( Spectrum::band );
};

#endif // SPECTRUMEDITOR_H

