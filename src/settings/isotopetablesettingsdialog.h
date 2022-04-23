/*
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ISOTOPETABLESETTINGSDIALOG_H
#define ISOTOPETABLESETTINGSDIALOG_H

#include "isotopetablesettingscard.h"
#include "isotopeview.h"
#include "kalziumunitcombobox.h"
#include "prefs.h"
#include <QWidget>

class IsotopeTableSettingsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit IsotopeTableSettingsDialog(QWidget *parent);
    virtual ~IsotopeTableSettingsDialog();
    bool hasChanged() const;
    bool isDefault() const;
    int getMode() const
    {
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
