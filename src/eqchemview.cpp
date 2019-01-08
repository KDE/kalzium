/***************************************************************************
 *   Copyright (C) 2004, 2005, 2006 by Thomas Nagy <tnagy2^8@yahoo.fr>     *
 *   Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>              *
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

#include "eqchemview.h"

#include <QClipboard>
#include <QDebug>

#include <LineEditUrlDropEventFilter>

#include "ui_equationview.h"

#include <stdlib.h>

#include <config-kalzium.h>

#ifdef HAVE_FACILE
extern "C" {
    char* solve_equation(const char *);
}
#else
char* solve_equation(const char *) {
    return NULL;
}
#endif

void EQChemDialog::compute()
{
    QString equation(ui.lineEdit->text());
    equation.replace(QLatin1String("->"), QLatin1String(" -> "));
    equation.append(' ');
    equation.prepend(' ');

    char * result = solve_equation(equation.toLatin1().constData());

    QString answer = QString(result);

    qDebug() << "Answer: " << answer;

    ui.answer_label->setText(answer);

    // mem leak ?
    free(result);
}

EQChemDialog::EQChemDialog(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    LineEditUrlDropEventFilter *dropUrlEventFilter = new LineEditUrlDropEventFilter(parent);
    dropUrlEventFilter->installEventFilter(ui.lineEdit);

    ui.lblHelp->setText(getHelpText());

    connect(ui.calculateButton, &QAbstractButton::clicked,
            this, &EQChemDialog::compute);
    connect(ui.btnCopy, &QAbstractButton::clicked,
            this, &EQChemDialog::copyAnswer);
}

void EQChemDialog::copyAnswer()
{
    qDebug() << "EQChemDialog::copyAnswer()";
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui.answer_label->text(), QClipboard::Clipboard);
}

const QString EQChemDialog::getHelpText()
{
    return i18nc(
               "Help text for the chemical equation solver",
               "The equation solver allows you to balance a chemical equation.<br> "
               "<br>"
               "<b>Using Variables</b><br>"
               "To express variable quantities of an element, put a single character in front "
               "of the element's symbol, as shown in this example:<br>"
               "<i>aH + bO -> 5H2O</i> (Result: <b>10</b> H + <b>5</b> O -&gt; <b>5</b> H<sub>2</sub>O)<br>"
               "Solving this expression will give you the needed amount of Hydrogen and Oxygen.<br>"
               "<br>"
               "<b>Defining electric charges</b><br>"
               "Use box brackets to specify the electric charge of an element, as shown in this example:<br>"
               "<i>4H[+] + 2O -> cH2O[2+]</i> (Result: <b>4</b> H<b><sup>+</sup></b> + <b>2</b> O -&gt; <b>2</b> H<b><sub>2</sub></b>O<b><sup>2+</sub></b>)"
           );
}

QSize EQChemDialog::sizeHint() const
{
    QSize size;
    size.setWidth(200);
    return size;
}
