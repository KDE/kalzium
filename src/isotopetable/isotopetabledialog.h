/***************************************************************************
 *   Copyright (C) 2007, 2008 by Carsten Niehaus <cniehaus@kde.org>        *
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

#ifndef ISOTOPETABLEDIALOG_H
#define ISOTOPETABLEDIALOG_H

#include "ui_isotopedialog.h"

#include <QDialog>

class IsotopeItem;

/**
 * This class is the drawing widget for the whole table
 *
 * @author Pino Toscano
 * @author Carsten Niehaus
 */
class IsotopeTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IsotopeTableDialog(QWidget* parent = nullptr);

private:
    Ui::isotopeWidget ui;

private slots:
    void updateDockWidget(IsotopeItem *);
    void zoom(int);
    void slotZoomLevelChanged(double);

};

#endif // ISOTOPETABLEDIALOG_H
