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

// Own includes
#include "obsupportedformat.h"

// Qt includes
#include <qstringlist.h>
#include <qregexp.h>

// OpenBabel includes
#include <openbabel/obconversion.h>

using namespace std;
using namespace OpenBabel;

OBSupportedFormat::OBSupportedFormat()
{
    OBConversion conv;
    Formatpos pos;
    OBFormat* pFormat;
    const char* str = NULL;
    while( OBConversion::GetNextFormat(pos, str, pFormat) )
    {
        if(( pFormat->Flags() & NOTWRITABLE) && (pFormat->Flags() & NOTREADABLE) ) {
            continue;
        }
        if ( !(pFormat->Flags() & NOTREADABLE) ) {
            m_InputFormat.append(str);
        }
        if ( !(pFormat->Flags() & NOTWRITABLE) ) {
            m_OutputFormat.append(str);
        }
    }
}

OBSupportedFormat::~OBSupportedFormat()
{
}

QStringList OBSupportedFormat::getInputFormat() const
{
    return( m_InputFormat );
}

QStringList OBSupportedFormat::getOutputFormat() const
{
    return( m_OutputFormat );
}

void OBSupportedFormat::setFormatExtensions()
{

    for(QStringList::Iterator it = m_InputFormat.begin(); it != m_InputFormat.end(); ++it) {
        QString tmp = (*it).replace(QRegExp(" --.*"),QString(""));

        m_InputFormatExtensions << tmp;
    }

}

