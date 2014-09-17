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

#ifndef KALZIUMSEARCH_H
#define KALZIUMSEARCH_H

#include <QList>
#include <QObject>

#include "element.h"
//class Element;

/**
 * Represent a search.
 *
 * @author Pino Toscano
 */
class Search : public QObject
{
    Q_OBJECT

public:
    /**
     * The kind of search we can perform
     */
    enum SearchKind {
        SearchByName = 0x01,
        SearchBySymbol = 0x02,
        SearchAll = 0xFF
    };

    /**
     * Construct a new empty search.
     */
    Search();

    /**
     * @return the search text
     */
    QString searchText() const;

    /**
     * @return the kind of search
     */
    SearchKind searchKind() const;

    /**
     * is the current Search active?
     * @return whether this search is active
     */
    bool isActive() const;

    /**
     * @return the found elements
     */
    const QList<Element*>& foundElements() const;

    /**
     * @return whether the element @p el matches the search
     */
    bool matches(Element* el) const;

    /**
     * @return whether the element @p el matches the search
     * overloaded function to use direct the element number.
     */
    bool matches(int el) const;

public slots:
    /**
     * Search the @p text by looking at the element using the
     * specified @p kind
     */
    void doSearch(const QString& text, SearchKind kind);
    /**
     * Reset the current search (and put it not active).
     */
    void resetSearch();

signals:
    /**
     * The current search has changed (ie the found elements have
     * changed)
     */
    void searchChanged();
    /**
     * The current search has been reset.
     */
    void searchReset();

private:
    bool m_isActive;

    QString m_searchText;
    SearchKind m_searchKind;

    QList<Element*> m_foundElements;
};

#endif // KALZIUMSEARCH_H
