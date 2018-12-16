/***************************************************************************
 *   Copyright (C) 2006-2008 by Carsten Niehaus <cniehaus@kde.org>         *
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

#ifndef RSDIALOG_H
#define RSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QWidget>

#include "ui_rswidget.h"

/**
 * This class is the main class for R- and S-Phrases dialog.
 *
 * @author Carsten Niehaus
 */
class RSDialog : public QDialog
{
    Q_OBJECT

public:
    RSDialog(QWidget * parent);

    Ui::RSWidget ui;

    /**
     * Filter the R- and S-Phrases.
     */
    void filterRS(const QList<int>& r, const QList<int>& s);

    QString rphrase(int number);

    QString sphrase(int number);

public slots:
    void filter();

private slots:
    void slotHelp();

private:
    QMap<int, QString> rphrases_map;
    QMap<int, QString> sphrases_map;

    void createSPhrases();
    void createRPhrases();

    void invalidPhaseString();
};

#endif // RSDIALOG_H
