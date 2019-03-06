/***************************************************************************
 *   Copyright (C) 2005-2008 by Carsten Niehaus <cniehaus@kde.org>         *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "isotopetabledialog.h"

#include "isotopeitem.h"
#include "isotopescene.h"
#include "legendwidget.h"
#include "kalziumschemetype.h"

#include <isotope.h>

#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <KConfigGroup>

#include <prefs.h>

IsotopeTableDialog::IsotopeTableDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(i18n("Isotope Table"));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &IsotopeTableDialog::reject);
    mainLayout->addWidget(buttonBox);
    buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
    ui.setupUi(mainWidget);
    ui.guide->setGuidedView(ui.gv);

    connect(ui.gv->scene(), SIGNAL(itemSelected(IsotopeItem*)),
            this, SLOT(updateDockWidget(IsotopeItem*)));
    connect(ui.gv, &IsotopeView::zoomLevelChanged,
            this, &IsotopeTableDialog::slotZoomLevelChanged);
    connect(ui.Slider, &QAbstractSlider::valueChanged,
            this, &IsotopeTableDialog::zoom);

    //Here comes the legend part
    QList< QPair<QString, QColor> > items;

    items << qMakePair(i18nc("alpha ray emission", "alpha"), QColor(Qt::red));
    items << qMakePair(i18nc("Electron capture method", "EC"), QColor(Qt::blue));
    items << qMakePair(i18nc("Many ways", "Multiple"), QColor(Qt::green));
    items << qMakePair(i18nc("Beta plus ray emission", "Beta +"), QColor(Qt::yellow));
    items << qMakePair(i18nc("Beta minus ray emission", "Beta -"), QColor(Qt::white));
    items << qMakePair(i18nc("Stable isotope", "Stable"), QColor(Qt::magenta));
    items << qMakePair(i18nc("Unknown Decay", "unknown"), QColor(Qt::darkGray));

    foreach (const legendPair &pair, items) {
        LegendItem *item = new LegendItem(pair);
        ui.infoWidget->layout()->addWidget(item);
    }
    ui.infoWidget->setMinimumWidth(150);
}

void IsotopeTableDialog::zoom (int level)
{
    double zoom = level / 2.0;
    (ui.gv)->setZoom(zoom);
}

void IsotopeTableDialog::updateDockWidget(IsotopeItem * item)
{
    Isotope *s = item->isotope();

    QString header = i18n("<h1>%1 (%2)</h1>", s->parentElementSymbol(), s->parentElementNumber());
    QString mag = i18n("Magnetic moment: %1",
                       s->magmoment().isEmpty()?
                           i18nc("Unknown magnetic moment", "Unknown"):s->magmoment());

    QString halflife;
    if (s -> halflife() > 0.0) {
        halflife = i18n ("Halflife: %1 %2", s->halflife(), s->halflifeUnit());
    } else {
        halflife = i18n ("Halflife: Unknown");
    }

    QString abundance = i18n("Abundance: %1 %", !s->abundance().isEmpty() ? s->abundance() : QStringLiteral("0"));
    QString nucleons = i18n("Number of nucleons: %1", s->nucleons());
    QString spin = i18n("Spin: %1", s->spin().isEmpty()?
                                        i18nc("Unknown spin", "Unknown"): s->spin());
    QString exactMass = i18n("Exact mass: %1 u", s->mass());

    QString html = header + "<br />" + nucleons + "<br />" + mag  + "<br />" + exactMass + "<br />"
                   + spin +"<br />" + abundance + "<br />" + halflife;

    ui.label->setText(html);
}

void IsotopeTableDialog::slotZoomLevelChanged(double value)
{
    const bool b = ui.Slider->blockSignals(true);
    ui.Slider->setValue(qreal(value * 2));
    ui.Slider->blockSignals(b);
}

