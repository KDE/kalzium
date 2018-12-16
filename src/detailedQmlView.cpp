/***************************************************************************
 *   Copyright (C) 2011   by Etienne Rebetez etienne.rebetez@oberwallis.ch *
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

#include "detailedQmlView.h"

#include <QDebug>
#include <QDeclarativeView>
#include <QStandardPaths>
#include <QVBoxLayout>

#include <prefs.h>
#include "kalziumdataobject.h"
#include "kalziumutils.h"

DetailedQmlView::DetailedQmlView(QWidget *parent) : QWidget(parent)
{
    m_elementProperty = KalziumElementProperty::instance();

    QVBoxLayout *layout = new QVBoxLayout(this);

    QDeclarativeView *view = new QDeclarativeView(this);

    QString qmlPath = QStandardPaths::locate(QStandardPaths::DataLocation, "qml/", QStandardPaths::LocateDirectory);

    m_context = view->rootContext();

    view->setSource(QUrl::fromLocalFile(qmlPath + "detailview.qml"));

    layout->addWidget(view);
    layout->setMargin(0);
    setLayout(layout);

//     resize(200, 200);

    setElement(1);

}

DetailedQmlView::~DetailedQmlView()
{
}

void DetailedQmlView::setElement(int el)
{
    m_element = KalziumDataObject::instance()->element(el);

    m_context->setContextProperty("itemWidth",
                                  width());

    m_context->setContextProperty("itemHeight",
                                  height());

    m_context->setContextProperty("backgroundColor",
                                  m_elementProperty->getElementColor(el));

    QColor gradientColor = m_elementProperty->getElementColor(el);
    qreal h, s, v, a;
    gradientColor.getHsvF(&h, &s, &v, &a);
    gradientColor.setHsvF(h, s, v*0.7, a);
    m_context->setContextProperty("backgroundColor2", gradientColor);

    // values
    m_context->setContextProperty("atomicNumber",
                                  m_element->dataAsString(ChemicalDataObject::atomicNumber));

    m_context->setContextProperty("atomicSymbol",
                                  m_element->dataAsString(ChemicalDataObject::symbol));

    m_context->setContextProperty("atomName",
                                  m_element->dataAsString(ChemicalDataObject::name));

    m_context->setContextProperty("atomicMass",
                                  KalziumUtils::prettyUnit(m_element, ChemicalDataObject::mass));

    m_context->setContextProperty("electronConf",
                                  KalziumUtils::prettyUnit(m_element, ChemicalDataObject::electronicConfiguration));

    m_context->setContextProperty("electroNegativity",
                                  KalziumUtils::prettyUnit(m_element, ChemicalDataObject::electronegativityPauling));

    if (Prefs::colorschemebox() == 2) { //The iconic view is the 3rd view (0,1,2,...)
        m_context->setContextProperty("showSvg", true);
    } else {
        m_context->setContextProperty("showSvg", false);
    }

}

void DetailedQmlView::itemPressed()
{
    QVariant value;
    value = m_context->contextProperty("idSymbol");
    qDebug() << "from qml:" << value;
}

void DetailedQmlView::resizeEvent(QResizeEvent* event)
{
    m_context->setContextProperty("itemWidth", width());
    m_context->setContextProperty("itemHeight", height());

    QWidget::resizeEvent(event);
}
