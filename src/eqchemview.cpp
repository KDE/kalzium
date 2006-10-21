/***************************************************************************
 *   Copyright (C) 2004, 2005, 2006 by Thomas Nagy <tnagy2^8@yahoo.fr>
 *   Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QLabel>
#include <QString>

#include <kdebug.h>
#include "config-kalzium.h"

#include "eqchemview.h"

#include "ui_equationview.h"

#include <stdlib.h>

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
    QString equation( ui.lineEdit->text() );
    equation.replace("+", "+");
    equation.replace("->", " -> ");
    equation.append(" ");
    equation.prepend(" ");
    
    char * result = solve_equation( equation.toLatin1() );

    QString answer = QString(result);

    kDebug() << "Answer: " << answer << endl;


    ui.question_label->setText(equation);
    ui.answer_label->setText(answer);

    // mem leak ?
    free(result);
}

EQChemDialog::EQChemDialog( QWidget *parent )
  : KDialog( parent )
{
    setCaption( i18n( "Solve Chemical Equations Viewer" ) );
    setButtons( Help | Apply | Close );
    setDefaultButton( Apply );

    connect( this, SIGNAL( applyClicked() ), 
            this, SLOT( compute() ) );
	
    ui.setupUi( mainWidget() );
}

#include "eqchemview.moc"

