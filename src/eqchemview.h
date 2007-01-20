/***************************************************************************
 *   Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.           *
 ***************************************************************************/

#ifndef _EQCHEMVIEW_H_
#define _EQCHEMVIEW_H_

#include <kdialog.h>

#include "ui_equationview.h"

/**
 * @author Carsten Niehaus
 */
class EQChemDialog : public KDialog
{
    Q_OBJECT

    public: 
        /**
         * public constructor
         *
         * @param parent the parent widget
         */
        EQChemDialog( QWidget *parent );

    private:
        Ui::EquationView ui;

        void copyAnswer();

    public slots:
        /**
          start the computation (balancing) of the equation
          */
        void compute();

        void slotUser1();
};

#endif // _EQCHEMVIEW_H_
