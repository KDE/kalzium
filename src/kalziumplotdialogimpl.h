#ifndef KALZIUMPLOTIMPL_H
#define KALZIUMPLOTIMPL_H
/***************************************************************************
                    csvdialogimpl.h  -  description
                             -------------------
    begin                : June 2003
	copyright            : (C) 2003 by Carsten Niehaus                     
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
#include <qvaluelist.h>
#include <qptrlist.h>

class KListView;
class KComboBox;
class QSpinBox;
class KalziumPlotWidget;
		
class ChemicalElement
{
	public:
		ChemicalElement();

		QString name, symbol, block, orbit, ox;
		double weight, en, mp, bp, density, ie, ar;
		int acidbeh, az, date, group, biological, number;
};
	
typedef QValueList<double> doubleList;
typedef QPtrList<doubleList> dlist;
typedef QPtrList<ChemicalElement> chemElements;

class KalziumPlotDialogImpl : public KDialogBase
{
	Q_OBJECT

	public:
		KalziumPlotDialogImpl( QWidget *parent = 0 , const char *name = 0 );

	private:
		KListView *elementsKLV;
		QSpinBox *fromSpin, *toSpin;
		KComboBox *whatKCB;

		KalziumPlotWidget *pw;

		void startPlotting();
		void updateListview();

		void getPositions( int , double& , ChemicalElement* );

		doubleList getDoubleList( const int , const int );
		const double getMax( const int, const int);
		const double getMin( const int, const int);

		void loadData();

		dlist dl;
		chemElements elementsPtrList;
		
		void setPlotAxis( const int );


	protected:
		virtual void paintEvent(  QPaintEvent* );
		virtual void keyPressEvent( QKeyEvent *e );

	public slots:
		void slotZoomIn();
		void slotZoomOut();

	private slots:
		void slotUser1();

};

	

#endif // KALZIUMPLOTIMPL_H

