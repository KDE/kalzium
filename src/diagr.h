/*
* KmPlot - a math. function plotter for the KDE-Desktop
*
* Copyright (C) 1998, 1999  Klaus-Dieter Möller
*               2000, 2002 kd.moeller@t-online.de
*               
* This file is part of the KDE Project.
* KmPlot is part of the KDE-EDU Project.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

/** @file diagr.h
 * @brief contains the CDiagr class; View has a member of this class
 */

#ifndef diagr_included
#define diagr_included

// C/C++ includes
#include <math.h>
#include <stdio.h>

// Qt includes
#include <qnamespace.h>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qcolor.h>
#include <qpen.h>
#include <qpoint.h>
#include <qrect.h>

/** @name Easy Typing Substitutions. */
//@{
#define	Line				drawLine
#define	Lineh(x1, y, x2)	drawLine(x1, y, x2, y)
#define Linev(x, y1, y2)	drawLine(x, y1, x, y2) 
//@}
//
/** @name Axes Mode Flag Constants */
//@{
#define	ACHSEN			1		/**< visible axes */
#define PFEILE			2		/**< axes with arrows */
#define RAHMEN			4		/**< visible frame */
#define	EXTRAHMEN		8		/**< bigger frame */
#define BESCHRIFTUNG	16		/**< visible labels */ 
//@}

/** @name Grid Mode Constants */
//@{
#define	LINIENRASTER	1	/**< line grid */
#define	KREUZRASTER     2	/**< crosses grid */
#define	KREISRASTER     3	/**< polar grid */ 
//@}

/** @brief This class handles the drawing of the system of coordinates.
 * 
 * This is no GUI widget; a CDiagr instance is member of the View widget. 
 * The View class controls the paint device for CDiagr.
 *
 * @see View, View::dgr
 */
class CDiagr
{
public:
	/** The constructor sets the variables of the class to their initial values. */
	CDiagr();

	/** The destructor is empty... */
	~CDiagr();

	/**
	 * This function prepares system of coordinates. 
	 * Especially the axes and grid mode and the scaling is set.
	 * 
	 * @param Ref Coordinates of the bottom left edge.
	 * @param lx Length of the x-axis.
	 * @param ly Length of the y-axis.
	 * @param prt 1: output to priter, else drawing on screen.
	 * 
	 * The other parameters @p xmin, @p xmax, @p ymin, @p ymax, and @p mode have 
	 * the same meaning as the global variables declared in misc.h.
	 *
	 * @see misc::xmin, misc::xmax, misc::ymin, misc::ymax, misc::mode
	 */
	void	Create( QPoint Ref,
	             int lx, int ly,
	             double xmin, double xmax,
	             double ymin, double ymax,
	             char mode,
	             char prt );

	/** Prepares the scaling parameters */
	void Skal( double ex, double ey, char g_mode );	
	
	/** This draws the system of coordinates to the painter @p pDC. */
	void	Plot( QPainter* pDC );

	/**
	 * Looks for the plotting rectangle.
	 * @result PlotArea
	 * @see PlotArea
	 */
	QRect	GetPlotArea()
	{
		return PlotArea;
	}

	/**
	 * Looks for the rectangle for the frame.
	 * @result Rahmen
	 * @see Rahmen
	 */
	QRect	GetRahmen()
	{
		return Rahmen;
	}
	/** @defgroup group_trans Coordinate Transformation */
	//@{
	/** Calculating the device coordinates (Pixel) for a given "real" coordinate. */
	int	Transx( double ),
	/** Calculating the device coordinates (Pixel) for a given "real" coordinate. */
	Transy( double );

	/** Calculating the "real" coordinate from a device depending coordinate. */
	double	Transx( int ),
	/** Calculating the "real" coordinate from a device depending coordinate. */
	Transy( int );
	//@}

	/** Color of the frame. */
	QRgb RahmenFarbe,
	/** Color of the axes.
	 * @see misc::AchsenFarbe */
	AchsenFarbe,
	/** Color of the grid.
	 * @see misc::GitterFarbe */
	GitterFarbe;

	/** Same as in misc.h. 
	 * @see misc::Rahmendick */
	uint RahmenDicke,
	/** Same as in misc.h. 
	 * @see misc::AchsenDicke */
	AchsenDicke,
	/** Same as in misc.h. 
	 * @see misc::GitterDicke */
	GitterDicke,
	/** Same as in misc.h. 
	 * @see misc::TeilstrichDicke */
	TeilstrichDicke,
	/** Same as in misc.h. 
	 * @see misc::TeilstrichLaenge */
	TeilstrichLaenge,
	/** Offset of the frame. */
	RahmenOffset,
	/** This Flag is set to 1 if the coordinate doesnt fit into the plotting area.
	 * @see */
	xclipflg,            	// clipflg wird auf 1 gesetzt, wenn die
	yclipflg;			// Koordinaten außerhalb des Diagramms liegen

private:

	/** Draw the axes. */
	void Achsen( QPainter* );
	/** Draw the grid. */
	void Raster( QPainter* );
	/** Draw the axis labels. */
	void Beschriftung( QPainter* );
	/** contains Some settings flags.
	 * @see ACHSEN, PFEILE, RAHMEN, BESCHRIFTUNG, misc::mode
	 */
	char mode,
	/** Grid type:
	 * @li 0: no grid
	 * @li 1: line grid
	 * @li 2: crosses grid
	 * @li 3: polar grid
	 * @see LINIENRASTER, KREUZRASTER, KREISRASTER, misc::g_mode
	 */
	g_mode,

	/** If the this flag is set to 1 the output device is a printer.
	 * Since some Qt changes it is neccesary to distinguish between drawing 
	 * to printer or screen to preserve the high precision of the output.
	 */
	prt;

	double xmin, xmax,      // x-Wertebereich
	ymin, ymax,      // y-Wertebereich
	xmd, ymd,     	// x/y Begrenzung
	ex, ey,     	// x/y-Achsenteilung
	tsx, tsy,     	// Positionen der ersten Teilstriche
	ox, oy,        // Bildschirm-Koordinaten des Ursprungs
	skx, sky;  // Skalierungsfaktoren
	QRect	PlotArea,     	// Plotbereich
	Rahmen;     // Rahmen
};

#endif // diagr_included
