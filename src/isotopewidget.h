#ifndef ISOTOPEWIDGET_H
#define ISOTOPEWIDGET_H
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

class QLabel;

class IsotopeWidget : public QWidget
{
	Q_OBJECT

	public:
		IsotopeWidget( QWidget* parent = 0, const char* name = 0 );

		void setIsotopes( const QString& isotopes_string );

	private:
		QWidget* isotopeLabel( const QString& str );
};


#endif // ISOTOPEWIDGET_H
