/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "searchwidget.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QStyle>

#include <KLineEdit>
#include <KLocalizedString>

#include "kalziumdataobject.h"
#include "search.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
    , m_searchLine(new KLineEdit(this))
{
    auto mainlay = new QVBoxLayout(this);
    mainlay->setContentsMargins({});
    mainlay->setSpacing(0);

    auto wrapper = new QVBoxLayout;
    wrapper->setContentsMargins(
        style()->pixelMetric(QStyle::PM_LayoutLeftMargin),
        style()->pixelMetric(QStyle::PM_LayoutTopMargin),
        style()->pixelMetric(QStyle::PM_LayoutRightMargin),
        style()->pixelMetric(QStyle::PM_LayoutBottomMargin)
    );
    wrapper->addWidget(m_searchLine);

    m_searchLine->setClearButtonEnabled(true);
    m_searchLine->setPlaceholderText(i18n("Search..."));
    m_searchLine->setTrapReturnKey(true);
    connect(m_searchLine, &QLineEdit::textChanged, this, &SearchWidget::searchTextChanged);
    connect(m_searchLine, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setMaximumHeight(1);

    mainlay->addLayout(wrapper);
    mainlay->addWidget(separator);
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

void SearchWidget::searchTextChanged(const QString &)
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

    const QString txt = m_searchLine->text();
    if (!txt.isEmpty()) {
        s->doSearch(txt, Search::SearchAll);
    } else {
        s->resetSearch();
    }
}

#include "moc_searchwidget.cpp"
