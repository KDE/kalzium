/***************************************************************************
 *   Copyright (C) 2009     by Kashyap R Puranik, kashthealien@gmail.com   *
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

#include "calculator.h"

#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>

#include <KActionCollection>
#include <KConfigGroup>
#include <KHelpClient>
#include <KLocalizedString>
#include <KStandardAction>
#include <KToolInvocation>

calculator::calculator(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(i18n("Chemical Calculator"));
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Help|QDialogButtonBox::Close);
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &calculator::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &calculator::reject);
    mainLayout->addWidget(buttonBox);
    buttonBox->button(QDialogButtonBox::Close)->setDefault(true);

    ui.setupUi(mainWidget);

    int maxTextWidth = 0;
    QStyleOptionViewItem option;
    option.initFrom(ui.tree);
    for (int i = 0; i < ui.tree->topLevelItemCount(); ++i) {
        maxTextWidth = qMax(maxTextWidth, ui.tree->itemDelegate()->sizeHint(option, ui.tree->model()->index(i, 0)).width());
    }
    // 20 because we want some margins, not a too tight text
    ui.tree->setMaximumWidth(qMax(ui.tree->maximumWidth(), maxTextWidth + 20));

    // Add the nuclear calculator to the user interface
    m_nuclearCalculator = new nuclearCalculator(this);
    ui.stack->addWidget(m_nuclearCalculator);
    // Add the gas calculator to the user interface
    m_gasCalculator = new gasCalculator(this);
    ui.stack->addWidget(m_gasCalculator);
    // Add the concentration calculator to the user interface
    m_concCalculator = new concCalculator(this);
    ui.stack->addWidget(m_concCalculator);
    // Add the molecular mass Calculator widget to the user interface
    m_moleCalculator = new MolcalcWidget(this);
    ui.stack->addWidget(m_moleCalculator);
    // Add the molecular mass Calculator widget to the user interface
    m_titraCalculator = new titrationCalculator(this);
    ui.stack->addWidget(m_titraCalculator);

#ifdef HAVE_FACILE
    // Add the equation balancer widget to the user interface
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.tree);
    treeItem->setText(0, i18n("Equation Balancer"));

    m_equationBalancer = new EQChemDialog(this);
    ui.stack->addWidget(m_equationBalancer);
#endif
    // Add an image to the file
    ui.pic->setPixmap((QIcon::fromTheme(QStringLiteral("calculate"))).pixmap(128,128));

    // Connect the tree item selection signal to the corresponding slot
    connect(ui.tree, &QTreeWidget::itemClicked, this,
            &calculator::slotItemSelection);

    ui.tree->setCurrentItem(ui.tree->topLevelItem(0), 0, QItemSelectionModel::ToggleCurrent);

    // help clicked
    connect(buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, &calculator::slotHelp);
}

calculator :: ~calculator()
{
}

void calculator::slotItemSelection(QTreeWidgetItem *item)
{
    if (item == nullptr) {
        return;
    }

    //DEBUG
    qDebug() << "Item clicked: " << item->text(0);

    QString s = item->text(0);

    if (!(s.compare(i18n("Introduction")))) {
        ui.stack->setCurrentWidget(ui.intro);
    } else if (!(s.compare(i18n("Nuclear Calculator")))) {
        // if nuclear calculator is selected, show the widget in the user interface
        ui.stack->setCurrentWidget(m_nuclearCalculator);
    } else if (!(s.compare(i18n("Gas Calculator")))) {
        // if gas calculator is selected, show the widget in the user interface
        ui.stack->setCurrentWidget(m_gasCalculator);
    } else if (!(s.compare(i18n("Concentration Calculator")))) {
        // if the concentration calculator is selected, show the widget in the UI
        ui.stack->setCurrentWidget(m_concCalculator);

    // The equation balancer needs FACILE library, if it's present HAVE_FACILE = 1
#ifdef HAVE_FACILE
    } else if (!(s.compare(i18n("Equation Balancer")))) {
    // If the equation balancer was selected, open it in the UI.
        ui.stack->setCurrentWidget(m_equationBalancer);
#endif

    } else if (!(s.compare(i18n("Molecular mass Calculator")))) {
        ui.stack->setCurrentWidget(m_moleCalculator);
    } else if (!(s.compare(i18n("Titration Calculator")))) {
        ui.stack->setCurrentWidget(m_titraCalculator);
    }
}


void calculator::slotHelp()
{
    KHelpClient::invokeHelp(QStringLiteral("tools.html#perf_calculation"), QStringLiteral("kalzium"));
}
