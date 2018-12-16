/***************************************************************************
 *   Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>              *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#ifndef EQCHEMVIEW_H
#define EQCHEMVIEW_H

#include <QWidget>

#include "ui_equationview.h"

/**
 * @author Carsten Niehaus
 */
class EQChemDialog : public QWidget
{
    Q_OBJECT

public:
    /**
     * public constructor
     *
     * @param parent the parent widget
     */
    explicit EQChemDialog(QWidget *parent);

private:
    const QString getHelpText();

    Ui::EquationView ui;

public slots:
    /**
      start the computation (balancing) of the equation
      */
    void compute();

    void copyAnswer();

protected:
    QSize sizeHint() const override;
};

#endif // EQCHEMVIEW_H
