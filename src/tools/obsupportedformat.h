/***************************************************************************
 *   Copyright (C) 2006-2007 by Jerome Pansanel                            *
 *   j.pansanel@pansanel.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#ifndef _OB_SUPPORTEDFORMAT_H_
#define _OB_SUPPORTEDFORMAT_H_

#include <QStringList>

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

        QStringList getInputFormatExtensions() const {
            return m_InputFormatExtensions;
        };

        void setFormatExtensions();

    private:
        QStringList m_InputFormat;
        QStringList m_OutputFormat;

        QStringList m_InputFormatExtensions;
};

#endif // OBSUPPORTEDFORMAT_H

