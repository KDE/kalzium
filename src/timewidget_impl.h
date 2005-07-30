#ifndef TIMEWIDGET_IMPL_H
#define TIMEWIDGET_IMPL_H

/***************************************************************************
    copyright            : (C) 2005 by Carsten Niehaus
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

#include "timewidget.h"

class TimeWidgetIMPL : public TimeWidget
{
	Q_OBJECT

	public:
		/**
		 * @param parent The parent of this widget
		 * @param name The name of this widget
		 */
		TimeWidgetIMPL( QWidget *parent = 0, const char* name = 0 );
};
#endif // TIMEWIDGET_IMPL_H

