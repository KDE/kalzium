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

#include <qwidget.h>

class QSlider;
class KLineEdit;
class KComboBox;
class QLabel;

/**This class converts values.
 *@author Carsten Niehaus
 */

class KConvert : public QWidget  {
	Q_OBJECT

	public: 
		enum messures {exa,peta,tera,giga,mega,kilo,hecto,deci,centi,milli,micro,nno,pico,femto,atto};
		KConvert(QWidget *parent, const char *name=0 );
		QSlider *FromSlider, *ToSlider;

		KLineEdit *ValueKLE, *ResultKLE, *ValueDim, *ResultDim;
		KComboBox *ToCK, *FromCK, *categorieKC;

		QLabel *difflabel, *q, *w, *e, *r;

		QString convertIntToMes( int );

	private:
		double toSI( QString );
		QString toTarget( double );

		void showDifference();

		private slots:
		void slotCalculate();
		void slotSetResultDim(const QString&);
		void slotSetToDim(const QString&);
		void slotAdjustKomboContent(int);
		void slotDifference();
};

#endif // KALZIUMCONVERTER_H
