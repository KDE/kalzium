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
		
typedef QValueList<double> doubleList;
typedef QPtrList<doubleList> dlist;

class KalziumPlotDialogImpl : public KDialogBase
{
	Q_OBJECT

	public:
		KalziumPlotDialogImpl( QWidget *parent = 0 , const char *name = 0 );
	
	private:
		KListView *elementsKLV;
		QSpinBox *fromSpin, *toSpin;
		KComboBox *whatKCB;
		
		void initPlotObjects();
		KalziumPlotWidget *pw;

		void startPlotting();

		void getPositions( int , double& , doubleList* );

		void loadData();

		dlist dl;
		

	protected:
		virtual void paintEvent(  QPaintEvent* );
		
	private slots:
		void slotUser1();
		
};

	

#endif // KALZIUMPLOTIMPL_H

