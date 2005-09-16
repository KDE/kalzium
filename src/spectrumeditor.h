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
#include <kdialogbase.h>

class Spectrum;
class SpectrumWidget;
class bandEditor;

class QSpinBox;
class KPushButton;

class SpectrumEditor : public KDialogBase
{
	Q_OBJECT

	public:
		SpectrumEditor( QWidget *parent, const char* name );

	private:
//		SpectrumWidget *m_spectrumWidget;
//		QSpinBox *m_spinbox_right, *m_spinbox_left;

		bandEditor *m_bandEditor;
	
	protected slots:
		virtual void slotUser1();

	private slots:
		void slotAddBand();
};

#endif // SPECTRUMEDITOR_H

