/*
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "isotopetablesettingsdialog.h"

#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>

#include <KLocalizedString>
#include <KUnitConversion/Converter>

IsotopeTableSettingsDialog::IsotopeTableSettingsDialog(QWidget *parent) : QWidget(parent)
{
    m_mode = Prefs::isotopeTableMode();

    auto card0 = new IsotopeTableSettingsCard(this, 0);
    card0->setZoom(0.07);
    card0->setText("One part to the side of the other");
    card0->setRadioButtonObjectName("isotopeTableMode_0");
    m_cards.append(card0);
    auto card1 = new IsotopeTableSettingsCard(this, 1);
    card1->setZoom(0.04);
    card1->setText("Both parts continuous");
    card0->setRadioButtonObjectName("isotopeTableMode_1");
    m_cards.append(card1);
    auto card2 = new IsotopeTableSettingsCard(this, 2);
    card2->setZoom(0.04);
    card2->setText("Horizontally");
    card0->setRadioButtonObjectName("isotopeTableMode_2");
    m_cards.append(card2);
    auto card3 = new IsotopeTableSettingsCard(this, 3);
    card3->setZoom(0.05);
    card3->setText("Horizontally (shifted)");
    card0->setRadioButtonObjectName("isotopeTableMode_3");
    m_cards.append(card3);

    m_cards[m_mode]->setChecked(true);

    connect(card0, &IsotopeTableSettingsCard::checked, this, &IsotopeTableSettingsDialog::setMode);
    connect(card1, &IsotopeTableSettingsCard::checked, this, &IsotopeTableSettingsDialog::setMode);
    connect(card2, &IsotopeTableSettingsCard::checked, this, &IsotopeTableSettingsDialog::setMode);
    connect(card3, &IsotopeTableSettingsCard::checked, this, &IsotopeTableSettingsDialog::setMode);

    auto layout = new QGridLayout(this);
    layout->addWidget(card0, 0, 0);
    layout->addWidget(card1, 0, 1);
    layout->addWidget(card2, 1, 0);
    layout->addWidget(card3, 1, 1);

    setLayout(layout);
}

IsotopeTableSettingsDialog::~IsotopeTableSettingsDialog()
= default;

bool IsotopeTableSettingsDialog::hasChanged() const
{
    return m_mode != Prefs::isotopeTableMode();
}

bool IsotopeTableSettingsDialog::isDefault() const
{
    return m_mode == 0;
}

void IsotopeTableSettingsDialog::setMode(int mode)
{
    m_mode = mode;
    unsigned short i = 0;
    for (auto card : m_cards)
    {
        if (i != m_mode)
            card->setChecked(false);
        i++;
    }
    m_cards[m_mode]->setChecked(true);
    emit modeChanged(m_mode);
}
