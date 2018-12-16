/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus <cniehaus@kde.org>              *
 *   Copyright (C) 2006 by Jerome Pansanel <j.pansanel@pansanel.net>       *
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

#ifndef OBCONVERTER_H
#define OBCONVERTER_H

#include "ui_obconverterwidget.h"

#include <QDialog>

// OpenBabel includes
#include <openbabel/obconversion.h>

class QDialog;

/**
 * @author Carsten Niehaus
 * @author Jerome Pansanel
 */
class KOpenBabel : public QDialog
{
    Q_OBJECT

public:
    /**
     * public constructor
     *
     * @param parent the parent widget
     */
    KOpenBabel(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~KOpenBabel();


    /**
     * Add file to the list
     */
    void addFile(const QString &filename);

private:
    Ui::OBConverterWidget ui;

    OpenBabel::OBConversion *OBConvObject;

    QString File;

    /**
     * Setup the interface for the window
     */
    void setupWindow();

private slots:
    /**
     * Add file to the list
     */
    void slotAddFile();

    /**
     * Select every file in the list
     */
    void slotSelectAll();

    /**
     * Delete file from the list
     */
    void slotDeleteFile();

    /**
     * Try to guess the input file type from the selection
     */
    void slotGuessInput();

    /**
     * Convert the file in the selected type
     */
    void slotConvert();

    /**
     * Open help page
     */
    void slotHelpRequested();

};

#endif // OBCONVERTER_H
