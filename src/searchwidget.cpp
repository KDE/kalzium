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

#include "searchwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#include <KLineEdit>
#include <KLocalizedString>

#include "kalziumdataobject.h"
#include "search.h"

SearchWidget::SearchWidget(QWidget *parent) : QWidget(parent), m_timer(nullptr)
{
    QHBoxLayout *mainlay = new QHBoxLayout(this);
    mainlay->setMargin(2);
    mainlay->setSpacing(5);

    mainlay->addWidget(new QLabel(i18n("Search:"), this));

    m_searchLine = new KLineEdit(this);
    m_searchLine->setClearButtonEnabled(true);
    m_searchLine->setTrapReturnKey(true);
    connect(m_searchLine, &QLineEdit::textChanged,
             this, &SearchWidget::searchTextChanged);
    connect(m_searchLine, SIGNAL(returnPressed()),
             this, SLOT(slotReturnPressed()));
    mainlay->addWidget(m_searchLine);
}

SearchWidget::~SearchWidget()
{
  delete m_searchLine;
  delete m_timer;
}

void SearchWidget::giveFocus()
{
    m_searchLine->setFocus(Qt::MouseFocusReason);
    m_searchLine->setCursorPosition(m_searchLine->text().length());
}

void SearchWidget::searchTextChanged(const QString&)
{
    if (m_timer) {
        m_timer->stop();
    } else {
        m_timer = new QTimer(this);
        m_timer->setSingleShot(true);
        connect(m_timer, &QTimer::timeout, this, &SearchWidget::doSearch);
    }
    // 1/3 of second should be ok
    m_timer->start(333);
}

void SearchWidget::slotReturnPressed()
{
    if (m_timer) {
        m_timer->stop();
    }
    doSearch();
}

void SearchWidget::doSearch()
{
    Search *s = KalziumDataObject::instance()->search();
    if (!s) {
        return;
    }

    QString txt = m_searchLine->text();
    if (txt.length() > 0) {
        s->doSearch(txt, Search::SearchAll);
    } else {
        s->resetSearch();
    }
}
