/***********************************************************************************
* Mass calculator: Plasmoid to calculate mass of a Molecule.
* Copyright (C) 2009, 2010 Etienne Rebetez, etienne.rebetez@oberwallis.ch
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
***********************************************************************************/


#include "Periodictable.h"
#include <psetables.h>
#include <math.h>

PeriodicGrid::PeriodicGrid ( int tableTyp, Plasma::Applet *applet )
{
    m_applet = applet;
    m_psTableType = tableTyp;

    m_actualPeriodSystem = new QGraphicsGridLayout();
    m_actualPeriodSystem->setSpacing(0);
    m_actualPeriodSystem->setContentsMargins ( 0,0,0,0 );

    setLayout(m_actualPeriodSystem);
    create();
}

PeriodicGrid::PeriodicGrid ( Plasma::Applet *applet )
{
    PeriodicGrid(0, applet);
}

PeriodicGrid::~PeriodicGrid()
{
}

void PeriodicGrid::create()
{
    int color = 100, period, group, intElement = 1;

    foreach (ElementLabel * i, m_elementItemList ) {
        delete i;
    }

    m_elementItemList.clear();

    int maxWidth = pseTables::instance()->getTabletype( m_psTableType )->coordsMax().x();
    // a parameter for the linear function. y(x) = ax + b
    double a = 130 / ( maxWidth/2 );

    foreach (intElement, pseTables::instance()->getTabletype( m_psTableType )->elements()) {
        if ( intElement <= 112) {
            group = pseTables::instance()->getTabletype( m_psTableType )->elementCoords( intElement ).x();
            period = pseTables::instance()->getTabletype( m_psTableType )->elementCoords( intElement ).y();

	    // makes a v-like color scheme
            color = 40 - period + ( abs(group - maxWidth/2 ) * a );

            ElementLabel *element = new ElementLabel( intElement, m_applet );
            m_elementItemList.append(element);

            element->setTextBackgroundColor(QColor ( color + period * 8, color - period * 3, color - period * 3));

            connect ( element, SIGNAL ( ElementAction ( QString ) ), m_applet, SLOT ( appendElement ( QString ) ) );

            m_actualPeriodSystem->addItem (element, period, group );
        }
    }
}

void PeriodicGrid::setCurrentPseTyp(int tableTyp)
{
    m_psTableType = tableTyp;
    create();
}

int PeriodicGrid::getCurrentPseTyp() const
{
    return m_psTableType;
}


// Element Label Class
void ElementLabel::drawToolTip ( Plasma::DataEngine::Data &element )
{
    Plasma::ToolTipContent data;
    data.setSubText (
        "<table>"
        "<tr><td align=\"center\" colspan=\"2\"><b>"   + element["number"].toString()  + " " + element["name"].toString()            + "</b></td></tr>"
        "<tr><td align=\"right\"><i>" + i18n ( "Electronegativity:" ) +    "</i> </td><td>" +  element["en"].toString()              + "</td></tr>"
        "<tr><td align=\"right\"><i>" + i18n ( "Molmass:" ) +              "</i> </td><td>" +  element["mass"].toString().left ( 6 ) + " u</td></tr>"
        "<tr><td align=\"right\"><i>" + i18n ( "Boilingpoint:" ) +         "</i> </td><td>" +  element["bp"].toString()              + " K</td></tr>"
        "<tr><td align=\"right\"><i>" + i18n ( "Meltingpoint:" ) +         "</i> </td><td>" +  element["mp"].toString()              + " K</td></tr>"
        "<tr><td align=\"center\" colspan=\"2\">"    + element["electronconf"].toString()                                            + "</td></tr>"
        "</table>"
    );
    Plasma::ToolTipManager::self()->setContent ( this, data );
}

ElementLabel::ElementLabel ( int element,  Plasma::Applet *applet, QGraphicsWidget * parent )
        : Plasma::IconWidget ( parent )
{
    setContentsMargins ( 0,0,0,0 );
    setMinimumSize(0,0);
    Plasma::DataEngine::Data thisElement =
        applet->dataEngine ( "kalzium" )->query ( QString ( "BlueObelisk:Element:" ) + QString::number ( element ) );
    setText ( thisElement["symbol"].toString() );

    drawToolTip ( thisElement );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( ElementPressed() ) );
}

#include "Periodictable.moc"
