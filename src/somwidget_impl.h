#ifndef SOMWIDGET_IMPL_H
#define SOMWIDGET_IMPL_H
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

#include "somwidget.h"

class Element;

class SOMWidgetIMPL : public SOMWidget
{
	Q_OBJECT

	public:
		SOMWidgetIMPL( QValueList<Element*>, QWidget *parent = 0, const char* name = 0 );

	private:
		QValueList<Element*> m_list;
		QString m_htmlBegin;
		QString m_htmlEnd;

	private slots:
		void slotTemp( int temp );
};
#endif // SOMWIDGET_IMPL_H
