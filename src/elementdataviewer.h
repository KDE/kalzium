#ifndef ELEMENTDATAVIEWER_H
#define ELEMENTDATAVIEWER_H
/***************************************************************************

                           elementdataviewer.h  -  description
                             -------------------
    copyright            : (C) 2004 by Carsten Niehaus
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

#include "element.h"
#include "plotwidget.h"

typedef QValueList<double> DoubleList;

class PlotSetupWidget;

class AxisData
{
	friend class ElementDataViewer;
	
	public: 
		AxisData();
		
		void setDataList( DoubleList list ){ 
			dataList = list; 
		};
		
		/**
		 * @return the value of the selected dataset of element @p element
		 */
		double value( int element ){
			element--;
			DoubleList::iterator it = dataList.at( element );
			return (*it);
		}

		enum PAXISDATA { WEIGHT=0, MEANWEIGHT=1, DENSITY=2, IE1=3, IE2=4, EN=5, MELTINGPOINT=6, BOILINGPOINT=7, ATOMICRADIUS=8  };

		int currentDataType(){
		       	return m_currentDataType;
		}

	private:
		DoubleList dataList;

		int m_currentDataType;
};

class ElementDataViewer : public KDialogBase
{
	Q_OBJECT

	public:
		ElementDataViewer( KalziumDataObject *data, QWidget *parent=0 , const char *name =0 );

		AxisData *yData;

	public slots:
		void slotZoomIn();
		void slotZoomOut();

		/**
		 * draws the plot
		 */
		void drawPlot();

		void slotUser1();

	protected:
		virtual void paintEvent(QPaintEvent*);
		virtual void keyPressEvent(QKeyEvent *e);

	private: 
		void initData();
		PlotWidget *m_pPlotWidget;
		PlotSetupWidget *m_pPlotSetupWidget;

		KalziumDataObject *d;

		QStringList names;

		void setupAxisData();
		
		void setLimits(int, int);

};

#endif // ELEMENTDATAVIEWER_H

