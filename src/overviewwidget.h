#ifndef OVERVIEWWIDGET_H
#define OVERVIEWWIDGET_H
/***************************************************************************
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

#include <qwidget.h>
#include <overviewbase.h>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;


class OverviewWidget : public OverViewBase
{
	Q_OBJECT

	public:
		OverviewWidget( QWidget* parent = 0, const char* name = 0 );

		void setElement( const int );
};


#endif // OVERVIEWWIDGET_H

