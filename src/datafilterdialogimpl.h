/***************************************************************************

        datafilterdialog.h  -  description
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
#ifndef DATAFILTERDIALOGIMPL_H
#define DATAFILTERDIALOGIMPL_H

#include "datafilterdialog.h"

class Datafilterdialogimpl : public Datafilterdialog
{
	Q_OBJECT

	public:
		Datafilterdialogimpl( QWidget* parent = 0, const char* name = 0 );

};
#endif // DATAFILTERDIALOGIMPL_H

