/***************************************************************************
                          kalziumconverter.h  -  description
                             -------------------
    begin                : Sunday Oct 20 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#ifndef KALZIUMCONVERTER_H
#define KALZIUMCONVERTER_H

#include <converter.h>

class QSlider;

/**This class converts values.
 *@author Carsten Niehaus
 */

class KConvert : public KalziumConvert  {
	Q_OBJECT

	public: 
		KConvert(QWidget *parent, const char *name=0 );
		QSlider *FromSlider, *ToSlider;

	private:
		double toSI( QString );
		QString toTarget( double );
		void showSlider();
		void hideSlider();

		private slots:
			void slotCalculate();
		void slotSetResultDim(const QString&) const;
		void slotSetToDim(const QString&) const;
		void slotAdjustKomboContent(int);
};

#endif // KALZIUMCONVERTER_H
