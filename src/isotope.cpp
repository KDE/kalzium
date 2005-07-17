#include "isotope.h"

#include <kdebug.h>
#include <klocale.h>

Isotope::Isotope(  int neutrons, double percentage, double weight, double halflife, QString format, bool alphadecay, bool betaplusdecay, bool betaminusdecay, bool ecdecay, double decayenergy, QString spin, QString magmoment )
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
}

QString Isotope::halflifeToHtml()
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
