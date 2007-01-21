#ifndef OBCONVERTER_H
#define OBCONVERTER_H
/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus <cniehaus@kde.org>
 *   Copyright (C) 2006 by Jerome Pansanel                                 *
 *   j.pansanel@pansanel.net
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


#include <kdialog.h>

#include "ui_obconverterwidget.h"

// Foward Declarations
class QWidget;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class KListBox;
class QComboBox;
class QListView;

/**
 * @author Carsten Niehaus
 */
class OBConverter : public KDialog
{
    Q_OBJECT

    public: 
        /**
         * public constructor
         *
         * @param parent the parent widget
         */
        OBConverter( QWidget *parent );
        
        /**
         * Add file to the list
         */
        void addFile( const QString &filename );

    private:
        Ui::OBConverterWidget ui;
        
        QString File;

        /**
         * Setup the interface for the window
         */
        void setupWindow();

    private slots:
        /**
         * Clear the window
         */
        void slotNew();

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

};

/**
 * Full description
 * @short Get supported format (input and output) from openbabel
 * @author Jerome Pansanel
 */
class OBSupportedFormat
{
    public:
        /**
         * Constructor
         */
        OBSupportedFormat();

        /**
         * Destructor
         */
        ~OBSupportedFormat();

        /**
         * @return a list containing the supported input format
         */
        QStringList getInputFormat() const;

        /**
         * @return a list containing the supported output format
         */
        QStringList getOutputFormat() const;

    private:
        QStringList m_InputFormat;
        QStringList m_OutputFormat;
};

#endif // OBCONVERTER_H
