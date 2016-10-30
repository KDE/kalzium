/***************************************************************************
 *   Copyright (C) 2006      by Pino Toscano, toscano.pino@tiscali.it      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "search.h"

#include "element.h"
#include "kalziumdataobject.h"

Search::Search() : m_isActive(false), m_searchKind(Search::SearchAll)
{
}

QString Search::searchText() const
{
    return m_searchText;
}

Search::SearchKind Search::searchKind() const
{
    return m_searchKind;
}

bool Search::isActive() const
{
    return m_isActive;
}

const QList<Element*>& Search::foundElements() const
{
    return m_foundElements;
}

bool Search::matches(Element* e) const
{
    return m_foundElements.contains(e);
}

bool Search::matches(int el) const
{
    Element *element =  KalziumDataObject::instance()->element(el);
    return matches(element);
}

void Search::doSearch(const QString& text, SearchKind kind)
{
    m_isActive = true;
    m_searchText = text;
    m_searchKind = kind;
    QList<Element*> newresults;
    foreach (Element *e, KalziumDataObject::instance()->ElementList) {
        bool found = false;
        if (!found
            && e->dataAsString(ChemicalDataObject::name).contains(text, Qt::CaseInsensitive)) {
            found = true;
        }
        if (!found
            && e->dataAsString(ChemicalDataObject::symbol).contains(text, Qt::CaseInsensitive)) {
            found = true;
        }
        if (found) {
            newresults << e;
        }
    }
    if (newresults != m_foundElements) {
        m_foundElements = newresults;
        emit searchChanged();
    }
}

void Search::resetSearch()
{
    if (!m_isActive) {
        return;
    }

    m_foundElements.clear();
    m_isActive = false;
    emit searchReset();
}

