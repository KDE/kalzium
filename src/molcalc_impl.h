#ifndef MOLCALC_IMPL_H
#define MOLCALC_IMPL_H
/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "molcalcbase.h"
#include "element.h"

/**
 * @author Carsten Niehaus
 */
class MolcalcImpl : public MolcalcDialog
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 * @param el is the element which data will be used
		 */
		MolcalcImpl( QWidget *parent = 0, const char *name = 0);
};
		

#endif // MOLCALC_IMPL_H
