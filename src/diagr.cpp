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

//local includes
#include "diagr.h"

#ifdef __osf__
#include <nan.h>
#define isnan(x) IsNAN(x)
#define isinf(x) IsINF(X)
#endif

CDiagr::CDiagr()
{
	RahmenFarbe = qRgb( 0, 0, 0 );
	AchsenFarbe = qRgb( 0, 0, 0 );
	GitterFarbe = qRgb( 192, 192, 192 );
	RahmenDicke = 2;
	AchsenDicke = 2;
	GitterDicke = 1;
	TeilstrichDicke = 1;
	TeilstrichLaenge = 10;
	RahmenOffset = 4;
	mode = 0;
	g_mode = 1;
	ex = ey = 1.;
}


CDiagr::~CDiagr()
{
}

void CDiagr::Create( QPoint Ref, 			// Bezugspunkt links unten
                     int lx, int ly, 				// Achsenlängen
                     double xmin, double xmax,    // x-Wertebereich
                     double ymin, double ymax, 	// y-Wertebereich
                     char mode, 					// Modus (Achsen, Pfeile, Rahmen)
                     char prt )                    // prt=1 => Druckerausgabe
{	int x, y, h, w;

	CDiagr::xmin = xmin;
	CDiagr::xmax = xmax;	// globale Variablen setzen
	CDiagr::ymin = ymin;
	CDiagr::ymax = ymax;
	CDiagr::mode = mode;
	CDiagr::prt = prt;
	xmd = xmax + 1e-6;
	ymd = ymax + 1e-6;
	tsx = ceil( xmin / ex ) * ex;
	tsy = ceil( ymin / ey ) * ey;
	skx = lx / ( xmax - xmin );			// Skalierungsfaktoren berechnen
	sky = ly / ( ymax - ymin );
	ox = Ref.x() - skx * xmin + 0.5;	// Ursprungskoordinaten berechnen
	oy = Ref.y() + sky * ymax + 0.5;
	PlotArea.setRect( x = Ref.x(), y = Ref.y(), w = lx, h = ly );
	if ( mode & EXTRAHMEN )
	{	//x-=(int)(RahmenOffset*0.1*skx); y-=(int)(RahmenOffset*0.1*sky);
		//w+=2*(int)(RahmenOffset*0.1*skx); h+=2*(int)(RahmenOffset*0.1*sky);
		if ( prt == 1 )
		{
			x -= 40;
			y -= 40;
			w += 80;
			h += 80;
		}
		else
		{
			x -= 10;
			y -= 10;
			w += 20;
			h += 20;
		}
		if ( mode & BESCHRIFTUNG )
		{	//if(xmin>=0.) {x-=(int)(0.5*skx); w+=(int)(0.5*skx);}
			//if(ymin>=0.) h+=(int)(0.6*sky);
			if ( prt == 1 )
			{
				if ( xmin >= 0. )
				{
					x -= 40;
					w += 40;
				}
				if ( ymin >= 0. )
					h += 40;
			}
			else
			{
				if ( xmin >= 0. )
				{
					x -= 20;
					w += 20;
				}
				if ( ymin >= 0. )
					h += 15;
			}
		}
	}

	Rahmen.setRect( x, y, w, h );
}

void CDiagr::Skal( double ex, double ey, char g_mode )
{
	CDiagr::ex = ex;
	CDiagr::ey = ey;
	CDiagr::g_mode = g_mode;
	tsx = ceil( xmin / ex ) * ex;
	tsy = ceil( ymin / ey ) * ey;
}

void CDiagr::Plot( QPainter* pDC )
{
	QPen pen( RahmenFarbe, RahmenDicke );

	if ( g_mode )
		Raster( pDC );         		// Raster zeichnen
	Achsen( pDC );							// Achsen zeichnen
	if ( mode & BESCHRIFTUNG )
		Beschriftung( pDC ); // Achsen beschriften
	if ( mode & ( RAHMEN | EXTRAHMEN ) ) 			// Rahmen zeichnen
	{	pDC->setPen( pen );
		pDC->drawRect( Rahmen );
	}
}

int CDiagr::Transx(double x)		// reale x-Koordinate
{   int xi;                			// transformierte x-Koordinate
    static double lastx;            // vorherige x-Koordinate

    if(isnan(x))
    {   xclipflg=1;
        if(lastx<1. && lastx>-1.) xi=(int)(ox-skx*lastx);
        else xi=(lastx<0)? PlotArea.left(): PlotArea.right();
    }
    else if(isinf(x)==-1){xclipflg=0; xi=PlotArea.left();}
    else if(isinf(x)==1) {xclipflg=0; xi=PlotArea.right();}
    else if(x<xmin) {xclipflg=1; xi=PlotArea.left();}
    else if(x>xmax) {xclipflg=1; xi=PlotArea.right();}
	else {xclipflg=0; xi=(int)(ox+skx*x);}

    lastx=x;
    return xi;
}

int CDiagr::Transy(double y)		// reale y-Koordinate
{   int yi;                     	// transformierte y-Koordinate
    static double lasty;            // vorherige y-Koordinate

    if(isnan(y))
    {   yclipflg=1;
        if(lasty<1. && lasty>-1.) yi=(int)(oy-sky*lasty);
        else yi=(lasty<0)? PlotArea.bottom(): PlotArea.top();
    }
    else if(isinf(y)==-1){yclipflg=0; yi=PlotArea.bottom();}
    else if(isinf(y)==1) {yclipflg=0; yi=PlotArea.top();}
    else if(y<ymin) {yclipflg=1; yi=PlotArea.bottom();}
    else if(y>ymax) {yclipflg=1; yi=PlotArea.top();}
    else {yclipflg=0; yi=(int)(oy-sky*y);}

    lasty=y;
    return yi;
}

double CDiagr::Transx( int x ) 		// Bildschirmkoordinate
{ return ( x -ox ) / skx;     			// reale x-Koordinate
}

double CDiagr::Transy( int y )         // Bildschirmkoordinate
{ return ( oy -y ) / sky;     			// reale y-Koordinate
}

void CDiagr::Achsen( QPainter* pDC ) 	// Achsen zeichnen
{	int a, b, tl;
	double d, da, db;

	if ( mode & ACHSEN )
	{
		int dx, dy;

		pDC->setPen( QPen( AchsenFarbe, AchsenDicke ) );
		pDC->Lineh( PlotArea.left(), b = Transy( 0. ), a = PlotArea.right() );	// x-Achse
		if ( mode & PFEILE && !( mode & RAHMEN ) && xmax > 0. ) 					// Pfeile
		{	dx = ( int ) ( 0.4 * skx );
			dy = ( int ) ( 0.15 * sky );
			pDC->Line( a, b, a - dx, b + dy );
			pDC->Line( a, b, a - dx, b - dy );
		}
		pDC->Linev( a = Transx( 0. ), PlotArea.bottom(), b = PlotArea.top() ); 	// y-Achse
		if ( mode & PFEILE && !( mode & RAHMEN ) && ymax > 0. ) 					// Pfeile
		{	dx = ( int ) ( 0.15 * skx );
			dy = ( int ) ( 0.4 * sky );
			pDC->Line( a, b, a - dx, b + dy );
			pDC->Line( a, b, a + dx, b + dy );
		}
	}

	pDC->setPen( QPen( AchsenFarbe, TeilstrichDicke ) );
	if ( mode & ACHSEN )
	{
		da = oy - TeilstrichLaenge;
		db = oy + TeilstrichLaenge;
		tl = ( mode & RAHMEN ) ? 0 : TeilstrichLaenge;
		d = tsx;
		if ( da > ( double ) PlotArea.bottom() )
		{
			a = PlotArea.bottom() - tl;
			b = PlotArea.bottom() + TeilstrichLaenge;
		}
		else if ( db < ( double ) PlotArea.top() )
		{
			b = PlotArea.top();
			a = PlotArea.top() - TeilstrichLaenge;
		}
		else
		{
			a = ( int ) da;
			b = ( int ) db;
		}
		while ( d < xmd - ex / 2. )
		{
			pDC->Linev( Transx( d ), a, b );
			d += ex;
		}

		da = ox - TeilstrichLaenge;
		db = ox + TeilstrichLaenge;
		d = tsy;
		if ( da < ( double ) PlotArea.left() )
		{
			a = PlotArea.left() - tl;
			b = PlotArea.left() + TeilstrichLaenge;
		}
		else if ( db > ( double ) PlotArea.right() )
		{
			b = PlotArea.right();
			a = PlotArea.right() - TeilstrichLaenge;
		}
		else
		{
			a = ( int ) da;
			b = ( int ) db;
		}
		while ( d < ymd - ey / 2. )
		{
			pDC->Lineh( a, Transy( d ), b );
			d += ey;
		}
	}
	else if ( mode & RAHMEN )
	{
		a = PlotArea.bottom() + TeilstrichLaenge;
		b = PlotArea.top() - TeilstrichLaenge;
		d = tsx;
		while ( d < xmd )
		{
			pDC->Linev( Transx( d ), PlotArea.bottom(), a );
			pDC->Linev( Transx( d ), PlotArea.top(), b );
			d += ex;
		}

		a = PlotArea.left() + TeilstrichLaenge;
		b = PlotArea.right() - TeilstrichLaenge;
		d = tsy;
		while ( d < ymd )
		{
			pDC->Lineh( PlotArea.left(), Transy( d ), a );
			pDC->Lineh( PlotArea.right(), Transy( d ), b );
			d += ey;
		}
	}
}

void CDiagr::Raster( QPainter* pDC )
{
	int a, b;
	double d, x, y;
	QPen pen( GitterFarbe, GitterDicke );

	pDC->setPen( pen );
	if ( g_mode == 1 )                   		// Linienraster
	{	d = tsx;
		while ( d < xmd )
		{
			pDC->Linev( Transx( d ), PlotArea.bottom(), PlotArea.top() );
			d += ex;
		}
		d = tsy;
		while ( d < ymd )
		{
			pDC->Lineh( PlotArea.left(), Transy( d ), PlotArea.right() );
			d += ey;
		}
	}
	else if ( g_mode == 2 )             		// Kreuzraster
	{	int dx, dy;

		dx = ( int ) ( 0.05 * skx );
		dy = ( int ) ( 0.05 * sky );
		for ( x = tsx; x < xmd; x += ex )
		{
			a = Transx( x );
			for ( y = tsy; y < ymd; y += ey )
			{
				b = Transy( y );
				pDC->Lineh( a - dx, b, a + dx );
				pDC->Linev( a, b - dy, b + dy );
			}
		}
	}
	else if ( g_mode == 3 ) 			// Kreisraster
	{	int x1, y1, x2, y2, dr, d2r, xm;
		double c, w;
		QRect rc = PlotArea;

		pDC->setClipRect( pDC->xForm( rc ) );
		c = hypot( xmd * skx, ymd * sky );
		xm = ( int ) ( c + ox );
		dr = ( int ) ( skx * ex );
		d2r = ( int ) ( 2. * skx * ex );
		x1 = ( int ) ox - dr;
		y1 = ( int ) oy - dr;
		x2 = y2 = d2r;

		do
		{
			pDC->drawEllipse( x1, y1, x2, y2 );
			x1 -= dr;
			y1 -= dr;
			x2 += d2r;
			y2 += d2r;
		}
		while ( x2 <= xm );

		x1 = ( int ) ox;
		y1 = ( int ) oy;
		for ( w = 0.; w < 2.*M_PI; w += M_PI / 12. )
		{
			x2 = ( int ) ( ox + c * cos( w ) );
			y2 = ( int ) ( oy + c * sin( w ) );
			pDC->Line( x1, y1, x2, y2 );
		}
		pDC->setClipping( FALSE );
	}
}

void CDiagr::Beschriftung( QPainter* pDC )
{
	int dx, dy, h, n, x, y;
	double d;
	QString s;
	QFont font;

	if ( prt == 1 )
	{
		dx = 20;
		dy = 40;
		h = 30;
	}
	else
	{
		dx = 5;
		dy = 10;
		h = 11;
	}
	font = QFont( "Helvetica", h );
	pDC->setFont( font );
	x = Transx( 0. );
	y = Transy( 0. );
	pDC->drawText( x - dx, y + dy, 0, 0,
	               Qt::AlignRight | Qt::AlignVCenter | Qt::DontClip, "0" );

	for ( d = tsx, n = ( int ) ceil( xmin / ex ); d < xmd; d += ex, ++n )
	{
		if ( n == 0 || fabs( d - xmd ) <= 1.5 * ex )
			continue;

		if ( n < 0 )
			s = "-";
		else
			s = "+";
		if ( fabs( ex - M_PI / 2. ) < 1e-3 )
		{
			if ( n == -1 || n == 1 )
				s += QChar( 960 ) + QString( "/2" );
			else if ( n % 2 == 0 )
			{
				if ( n == -2 || n == 2 )
					s += QChar( 960 );
				else
				{
					s = QString().sprintf( "%+d", n / 2 );
					s += QChar( 960 );
				}
			}
			else
				continue;
			pDC->drawText( Transx( d ), y + dy, 0, 0,
			               Qt::AlignCenter | Qt::DontClip, s );
		}

		else if ( fabs( ex - M_PI / 3. ) < 1e-3 )
		{
			if ( n == -1 || n == 1 )
				s += QChar( 960 ) + QString( "/3" );
			else if ( n % 3 == 0 )
			{
				if ( n == -3 || n == 3 )
					s += QChar( 960 );
				else
				{
					s = QString().sprintf( "%+d", n / 3 );
					s += QChar( 960 );
				}
			}
			else
				continue;
			pDC->drawText( Transx( d ), y + dy, 0, 0,
			               Qt::AlignCenter | Qt::DontClip, s );
		}

		else if ( fabs( ex - M_PI / 4. ) < 1e-3 )
		{
			if ( n == -1 || n == 1 )
				s += QChar( 960 ) + QString( "/4" );
			else if ( n % 4 == 0 )
			{
				if ( n == -4 || n == 4 )
					s += QChar( 960 );
				else
				{
					s = QString().sprintf( "%+d", n / 4 );
					s += QChar( 960 );
				}
			}
			else
				continue;
			pDC->drawText( Transx( d ), y + dy, 0, 0,
			               Qt::AlignCenter | Qt::DontClip, s );
		}

		else if ( ( n % 5 == 0 || n == 1 || n == -1 ) )
		{
			s = QString().sprintf( "%+0.3g", n * ex );
			pDC->drawText( Transx( d ), y + dy, 0, 0,
			               Qt::AlignCenter | Qt::DontClip, s );
		}
	}
	if ( xmax > 0 )
		pDC->drawText( Transx( xmax ) - dx, y + dy, 0, 0,
		               Qt::AlignCenter | Qt::DontClip, "x" );

	for ( d = tsy, n = ( int ) ceil( ymin / ey ); d < ymd; d += ey, ++n )
	{
		if ( n == 0 || fabs( d - ymd ) <= 1.5 * ey )
			continue;

		if ( n < 0 )
			s = "-";
		else
			s = "+";
		if ( fabs( ey - M_PI / 2. ) < 1e-3 )
		{
			if ( n == -1 || n == 1 )
				s += QChar( 960 ) + QString( "/2" );
			else if ( n % 2 == 0 )
			{
				if ( n == -2 || n == 2 )
					s += QChar( 960 );
				else
				{
					s = QString().sprintf( "%+d", n / 2 );
					s += QChar( 960 );
				}
			}
			else
				continue;
			pDC->drawText( x - dx, Transy( d ), 0, 0,
			               Qt::AlignVCenter | Qt::AlignRight | Qt::DontClip, s );
		}

		else if ( fabs( ey - M_PI / 3. ) < 1e-3 )
		{
			if ( n == -1 || n == 1 )
				s += QChar( 960 ) + QString( "/3" );
			else if ( n % 3 == 0 )
			{
				if ( n == -3 || n == 3 )
					s += QChar( 960 );
				else
				{
					s = QString().sprintf( "%+d", n / 3 );
					s += QChar( 960 );
				}
			}
			else
				continue;
			pDC->drawText( x - dx, Transy( d ), 0, 0,
			               Qt::AlignVCenter | Qt::AlignRight | Qt::DontClip, s );
		}

		else if ( fabs( ey - M_PI / 4. ) < 1e-3 )
		{
			if ( n == -1 || n == 1 )
				s += QChar( 960 ) + QString( "/4" );
			else if ( n % 4 == 0 )
			{
				if ( n == -4 || n == 4 )
					s += QChar( 960 );
				else
				{
					s = QString().sprintf( "%+d", n / 4 );
					s += QChar( 960 );
				}
			}
			else
				continue;
			pDC->drawText( x - dx, Transy( d ), 0, 0,
			               Qt::AlignVCenter | Qt::AlignRight | Qt::DontClip, s );
		}

		else if ( ( n % 5 == 0 || n == 1 || n == -1 ) )
		{
			s = QString().sprintf( "%+0.3g", n * ey );
			pDC->drawText( x - dx, Transy( d ), 0, 0,
			               Qt::AlignVCenter | Qt::AlignRight | Qt::DontClip, s );
		}
	}
	if ( ymax > 0 )
		pDC->drawText( x - dx, Transy( ymax ) + dy, 0, 0,
		               Qt::AlignVCenter | Qt::AlignRight | Qt::DontClip, "y" );
}
