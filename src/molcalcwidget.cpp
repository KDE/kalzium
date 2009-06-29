/***************************************************************************
 *   Copyright (C) 2003-2005, 2006 by Carsten Niehaus, cniehaus@kde.org    *
 *   Copyright (C) 2005      by Inge Wallin,     inge@lysator.liu.se       *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 *                                                                         *
 ***************************************************************************/

#include "molcalcwidget.h"

//libscience
#include <element.h>

#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "search.h"

#include <kdebug.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <klineedit.h>
#include <kstandarddirs.h>

#include <QApplication>
#include <QLabel>
#include <QLayout>
#include <QToolButton>
#include <QTimer>
#include <QKeyEvent>
#include <QFile>

MolcalcWidget::MolcalcWidget( QWidget *parent )
    : QWidget( parent )
{
	m_parser = new MoleculeParser( KalziumDataObject::instance()->ElementList );

    m_timer = new QTimer(this);
    m_timer->setSingleShot( true );
	
	ui.setupUi( this );
	
	connect( ui.calcButton, SIGNAL( clicked() ), this, SLOT( slotCalculate() ) );
	connect( ui.formulaEdit, SIGNAL( returnPressed() ), this, SLOT( slotCalculate() ) );
    connect( m_timer, SIGNAL( timeout() ), this, SLOT( slotCalculate() ) );
    connect( ui.alias, SIGNAL(clicked()), this, SLOT( addAlias()));

	ui.formulaEdit->setClearButtonShown(true);

	clear();

}

MolcalcWidget::~MolcalcWidget()
{
	delete m_parser;
}


void MolcalcWidget::clear()
{
	// Clear the data.
	m_mass = 0;
	m_elementMap.clear();

	//stop the selection in the periodic table
	KalziumDataObject::instance()->search()->resetSearch();

	// Clear the widgets.
	ui.resultLabel->clear();
	ui.resultMass->clear();
	
	ui.resultComposition->setText( i18n("To start, enter\na formula in the\nwidget above and\nclick on 'Calc'.") );

	ui.resultMass->setToolTip( QString() );
	ui.resultComposition->setToolTip( QString() );
	ui.resultLabel->setToolTip( QString() );
}


void MolcalcWidget::updateUI()
{
    kDebug() << "MolcalcWidget::updateUI()";

    if ( m_validInput ){
        kDebug() << "m_validInput == true";

        // The complexString stores the whole molecule like this:
        // 1 Seaborgium. Cumulative Mass: 263.119 u (39.2564 %)
        QString complexString;
		double mass;
		QString str;
		int i = 0;								 			// counter
		int col = m_elementMap.elements().count();		// number of columns
		ui.table->setRowCount(col);
		
        foreach (ElementCount * count , m_elementMap.map()) {
            // Update the resultLabel
            mass = count->element()->dataAsVariant( ChemicalDataObject::mass ).toDouble();
/* Using a table widget instead of strings
            str += i18nc( "For example: \"Carbon 1 12.000 \"",
            		 QString("%1     %2 atoms     %3 u     %4 u     %5%\n" )
                    .arg(count->element()->dataAsString( ChemicalDataObject::name), -20)
                    .arg(count->count(), -8)
                    .arg(count->element()->dataAsString( ChemicalDataObject::mass), -10)
                    .arg(mass * count->count())
                    .arg(mass * count->count()/ m_mass *100).toLatin1().data());
*/
			ui.table->setItem((int)i, 0, new QTableWidgetItem(tr("%1")
			.arg(count->element()->dataAsString( ChemicalDataObject::name))));
			ui.table->setItem((int)i, 1, new QTableWidgetItem(tr("%1")
			.arg(count->count())));
			ui.table->setItem((int)i, 2, new QTableWidgetItem(tr("%1")
			.arg(count->element()->dataAsString( ChemicalDataObject::mass))));
			ui.table->setItem((int)i, 3, new QTableWidgetItem(tr("%1")
			.arg(mass * count->count())));
			ui.table->setItem((int)i, 4, new QTableWidgetItem(tr("%1")
			.arg(mass * count->count()/ m_mass *100).toLatin1().data()));

			i++;
        }
        
        // The composition
        ui.resultComposition->setText( compositionString(m_elementMap) );

        // The mass
        ui.resultMass->setText( i18n( "Molecular mass: %1 u", m_mass ) );

        ui.resultMass->setToolTip(        complexString );
        ui.resultComposition->setToolTip( complexString );

#if 0
        // FIXME
        //select the elements in the table
        QList<Element*> list = m_elementMap.elements();
        KalziumDataObject::instance()->findElements( list );
#endif
    }
    else{//the input was invalid, so tell this the user
        kDebug() << "m_validInput == false";
        ui.resultComposition->setText( i18n( "Invalid input" ) );
        ui.resultLabel->setText( QString() );
        ui.resultMass->setText( QString() );

        ui.resultMass->setToolTip(        i18n( "Invalid input" ) );
        ui.resultComposition->setToolTip( i18n( "Invalid input" ) );
        ui.resultLabel->setToolTip(       i18n( "Invalid input" ) );
    }
}

QString MolcalcWidget::compositionString( ElementCountMap &_map )
{
	QString  str;

  foreach (ElementCount * count, _map.map()) {
      str += i18n( "%1<sub>%2</sub> " ,
              count->element()->dataAsString( ChemicalDataObject::symbol ) ,
              count->count() );
  }

	return str;
}


// ----------------------------------------------------------------
//                            slots


void MolcalcWidget::slotCalculate()
{
    kDebug() << "MolcalcWidget::slotCalcButtonClicked()";
	QString  molecule = ui.formulaEdit->text();

	// Parse the molecule, and at the same time calculate the total
	// mass, and the composition of it.
	if ( !molecule.isEmpty() )
		m_validInput = m_parser->weight(molecule, &m_mass, &m_elementMap);
	kDebug() << "done calculating.";

	updateUI();
}

void MolcalcWidget::keyPressEvent(QKeyEvent * /* e */)
{
    m_timer->start(500);
}

void MolcalcWidget::addAlias()
{
	QString shortForm = ui.shortForm->text();
	QString fullForm  = ui.fullForm ->text();
	
	// Temporary variables required for weight function of the molecule parser
	double x;
	ElementCountMap y;
	
	ui.aliasMessage->setText("");
	if ( shortForm.length() != 2)
	{
		ui.aliasMessage->setText(i18n
		("Symbol should consist of a Capital letter followed by a small one."));
		return;
	}
	
	if ( shortForm.at(0).category() != QChar::Letter_Uppercase ||
			shortForm.at(1).category() != QChar::Letter_Lowercase)
	{
		ui.aliasMessage->setText(i18n
		("Symbol should consist of a Capital letter followed by a small one."));
		return;
	}
	
	if ( m_parser->weight(shortForm, & x , & y))
	{
		ui.aliasMessage->setText(i18n
		("Symbol already being used"));
		return;
	}
	
	if (fullForm =="" || ! m_parser->weight(fullForm, & x, & y))
	{
		ui.aliasMessage->setText(i18n
		("Expansion is invalid, please specify a valid expansion"));
		return;
	}
	
	// Open the file to write
	QString fileName = KStandardDirs::locate( "data", "libkdeedu/data/symbols2.csv");
	QFile file(fileName);
	
	if (!(!file.open(QIODevice::WriteOnly| QIODevice::Append | QIODevice::Text)))
    {
    	QTextStream out(&file);
    	out << "\"" + shortForm + "\",\"" + fullForm + "\"\n";
    	kDebug() << fileName << "is the file.";
	    kDebug() << "\"" + shortForm + "\",\"" + fullForm + "\"\n";
    	ui.aliasMessage->setText(i18n("done!"));
		return;
    }
    else
    {
    	ui.aliasMessage->setText((i18n
		("Unable to find the user defined alias file."))+fileName);
		return;
    }
}

#include "molcalcwidget.moc"
