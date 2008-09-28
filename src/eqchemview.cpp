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

#include "eqchemview.h"

#include <QClipboard>
#include <QLabel>
#include <KTextBrowser>

#include <kdebug.h>

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
    QString equation( ui.lineEdit->text() );
//    equation.replace("+", "+");
    equation.replace("->", " -> ");
    equation.append(" ");
    equation.prepend(" ");
    
    char * result = solve_equation( equation.toLatin1() );

    QString answer = QString(result);

    kDebug() << "Answer: " << answer;


//    ui.question_label->setText(equation);
    ui.answer_label->setText(answer);

    // mem leak ?
    free(result);
}

EQChemDialog::EQChemDialog( QWidget *parent )
  : KDialog( parent )
{
    setCaption( i18n( "Solve Chemical Equations Viewer" ) );
    setButtons( Help | User1 | Close );
    setDefaultButton( None );

    m_helpWindow = NULL;

    ui.setupUi( mainWidget() );
	setButtonGuiItem( User1, KGuiItem( i18n( "Copy" ), "edit-copy", i18n( "Copy answer to clipboard" ) ) );
    
    connect( ui.calculateButton , SIGNAL( clicked() ), 
            this, SLOT( compute() ) );
    connect( this , SIGNAL( user1Clicked() ), 
            this, SLOT( slotUser1() ) );
    connect( this , SIGNAL( helpClicked() ),
            this, SLOT( slotHelp() ) );
}

void EQChemDialog::copyAnswer()
{
    kDebug() << "EQChemDialog::copyAnswer()";
    QClipboard *clipboard = QApplication::clipboard(); clipboard->setText( ui.answer_label->text(), QClipboard::Clipboard);
}

void EQChemDialog::slotUser1()
{
   copyAnswer(); 
}

void EQChemDialog::slotHelp()
{
    if( m_helpWindow == NULL )
    {
        m_helpWindow = new KDialog( this );
        m_helpWindow->setMinimumSize( 500, 300 );
        m_helpWindow->showButton( KDialog::Cancel, false );
        KTextBrowser *helpText = new KTextBrowser;
        helpText->setHtml( i18nc(
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
        ) );
        helpText->setOpenExternalLinks( true );
        m_helpWindow->setMainWidget( helpText );
        m_helpWindow->setCaption( i18nc( "Window title for the chemical solver's help", "Chemical Solver Help" ) );
        m_helpWindow->show();
    }
    else
    {
        if( !m_helpWindow->isVisible() )
        {
            m_helpWindow->show();
        }
        m_helpWindow->activateWindow();
    }
}


#include "eqchemview.moc"

