/*
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ISOTOPETABLESETTINGSDIALOG_H
#define ISOTOPETABLESETTINGSDIALOG_H

#include <QWidget>
#include "kalziumunitcombobox.h"
#include "isotopetablesettingscard.h"
#include "isotopeview.h"
#include "prefs.h"

class IsotopeTableSettingsDialog: public QWidget
{
Q_OBJECT

public:
    explicit IsotopeTableSettingsDialog(QWidget* parent);
    ~IsotopeTableSettingsDialog() override;
    bool hasChanged() const;
    bool isDefault() const;
    int getMode() const {
        return m_mode;
    }

private:
    QVector<IsotopeTableSettingsCard *> m_cards;
    int m_mode;

public Q_SLOTS:
    void setMode(int mode);

Q_SIGNALS:
    void modeChanged(int mode);

};

#endif // ISOTOPETABLESETTINGSDIALOG_H
