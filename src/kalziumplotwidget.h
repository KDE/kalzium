#ifndef KALZIUMPLOTWIDGET_H
#define KALZIUMPLOTWIDGET_H
/***************************************************************************

                           KalziumGraph.h  -  description
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

#include <qwidget.h>

#include "../../libkdeedu/kdeeduplot/kplotwidget.h"

#define BIGTICKSIZE 10
#define SMALLTICKSIZE 4
#define XPADDING 40
#define YPADDING 40

class QColor;
class QPixmap;

class KalziumPlotWidget : public KPlotWidget
{
	Q_OBJECT

	public:
		KalziumPlotWidget( double x1=0.0, double x2=1.0, double y1=0.0, double y2=1.0, QWidget *parent=0, const char* name=0 );

		/**Destructor (empty)
		 */
		~KalziumPlotWidget() {}

		/**@enum AXIS_TYPE
		  *Specifies the data type of the axis.
		  */
		enum AXIS_TYPE { DOUBLE=0, TIME=1, ANGLE=2, UNKNOWN_TYPE };

		/**@short Determine the placement of major and minor tickmarks, based on the current Limit settings
		  */
		void updateTickmarks();
		void setLimits(  double xb1, double xb2, double yb1, double yb2 );
		void setSecondaryLimits(  double xb1, double xb2, double yb1, double yb2 );
		void checkLimits();

		double xb() const { return DataRect2.x(); }
		double xb2() const { return DataRect2.x2(); }
		double yb() const { return DataRect2.y(); }
		double yb2() const { return DataRect2.y2(); }
		double dataWidth2() const { return DataRect2.width(); }
		double dataHeight2() const { return DataRect2.height(); }

		double xScale() const { return XScaleFactor; }
		void setXScale(  double s ) { XScaleFactor = s; }
		double yScale() const { return YScaleFactor; }
		void setYScale(  double s ) { YScaleFactor = s; }

		/**@return the data type of the x-axis (DOUBLE, TIME or ANGLE)
		  ·   ·   */
		AXIS_TYPE xAxisType() const { return XAxisType; }
		AXIS_TYPE xAxisType0() const { return XAxisType_0; }
		/**@return the data type of the y-axis (DOUBLE, TIME or ANGLE)
		 */
		AXIS_TYPE yAxisType() const { return YAxisType; }
		AXIS_TYPE yAxisType0() const { return YAxisType_0; }

		/**@short set the data type of the x-axis
		  ·   ·   *@param xtype the new data type ( DOUBLE, TIME, or ANGLE )
		  ·   ·   */
		void setXAxisType(  AXIS_TYPE xtype ) { XAxisType = xtype; }
		void setXAxisType0(  AXIS_TYPE xtype ) { XAxisType_0 = xtype; }
		/**@short set the data type of the y-axis
		  ·   ·   *@param xtype the new data type ( DOUBLE, TIME, or ANGLE )
		  ·   ·   */
		void setYAxisType(  AXIS_TYPE ytype ) { YAxisType = ytype; }
		void setYAxisType0(  AXIS_TYPE ytype ) { YAxisType_0 = ytype; }

	protected:
		void drawBox(  QPainter *p );

		double dXtick2, dYtick2;
		int nmajX2, nminX2, nmajY2, nminY2;
		AXIS_TYPE XAxisType, YAxisType, XAxisType_0, YAxisType_0;
		double XScaleFactor, YScaleFactor;
		DRect DataRect2;
};

#endif // KALZIUMPLOTWIDGET_H

