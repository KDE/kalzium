/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/
#include "isotope.h"

#include <kdebug.h>
#include <klocale.h>

Isotope::Isotope( int neutrons, double percentage, double weight, double halflife, const QString& format, bool alphadecay, bool betaplusdecay, bool betaminusdecay, double alphapercentage, double betaminuspercentage, double betapluspercentage, bool ecdecay, double decayenergy, const QString& spin, const QString& magmoment)
{
	m_neutrons = neutrons;
	m_percentage = percentage;
	m_halflife = halflife;
	m_format = format;
	m_weight = weight;
	m_alphadecay = alphadecay;
	m_betaplusdecay = betaplusdecay;
	m_betaminusdecay = betaminusdecay;

	m_ecdecay = ecdecay;
	m_decayenergy = decayenergy;
	m_spin = spin;
	m_magmoment = magmoment;

	m_alphapercentage = alphapercentage;
	m_betaminusdecay = betaminuspercentage;
	m_betaplusdecay = betapluspercentage;
}

QString Isotope::halflifeAsString()
{
	QString halflife;

	if ( !seconds() )//years
	{
		if ( m_halflife > 1000000 )
			halflife = i18n("%1 million years").arg( m_halflife/1000000.0 );
		if ( m_halflife > 1000000000 )
			halflife = i18n("%1 billion years").arg( m_halflife/1000000000.0 );
		else
			halflife = i18n("%1 years").arg( m_halflife );
	}
	else
	{
		if ( m_halflife < 120 )
			halflife = i18n("%1 seconds").arg( m_halflife );
		else if ( m_halflife > 1000 )
			halflife = i18n("%1 minutes").arg( m_halflife/60.0 );
		else if ( m_halflife > 3600 )
			halflife = i18n("%1 hours").arg( m_halflife/( 60*60 ) );
		if ( m_halflife > 86400 )   //one day
			halflife = i18n("%1 days").arg( m_halflife/( 60*60*24 ) );
		if ( m_halflife > ( 31536000 * 2 ) ) //two year
			halflife = i18n("%1 years").arg( m_halflife/( 3600.0*365 ));
	}

	return halflife;
}
