/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tablesdialog.h"

#include <kdebug.h>
#include <klocale.h>
#include <kicon.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>
#include <kcombobox.h>
#include <kpagewidgetmodel.h>
#include <kstandardaction.h>
#include <ktoolinvocation.h>

#include <QLayout>
#include <QHeaderView>
#include <QTableWidget>

TablesDialog::TablesDialog( QWidget *parent )
    : KPageDialog( parent )
{
	setFaceType( List );
	setButtons( Help | Close );
	setDefaultButton( Close );

	createGreekSymbolTable();
	createNumbersTable();
}

void TablesDialog::createGreekSymbolTable()
{
	QWidget *frame = new QWidget();
	KPageWidgetItem *item = addPage( frame, i18n( "Greek symbols" ) );
	item->setHeader( i18n( "Greek numbers" ) );
	item->setIcon( KIcon( "numbers" ) );
	QVBoxLayout *layout = new QVBoxLayout( frame );
	layout->setMargin( 0 );
	
	QTableWidget *table = new QTableWidget(frame);
	table->verticalHeader()->hide();
	table->setHorizontalHeaderItem( 0, new QTableWidgetItem( i18n( "Capital Greek Letter" ) ) );
	table->setHorizontalHeaderItem( 1, new QTableWidgetItem( i18n( "Small Greek Letter" ) ) );
	table->setHorizontalHeaderItem( 2, new QTableWidgetItem( i18n( "Name" ) ) );

	table->setColumnCount( 3 );
	table->setRowCount( 24 );
	
	layout->addWidget( table );

	table->setItem( 0, 0, new QTableWidgetItem( QString( QChar( 913 ) ) ) ); //capital Alpha
	table->setItem( 1, 0, new QTableWidgetItem( QString( QChar( 914 ) ) ) ); 
	table->setItem( 2, 0, new QTableWidgetItem( QString( QChar( 915 ) ) ) ); 
	table->setItem( 3, 0, new QTableWidgetItem( QString( QChar( 916 ) ) ) ); 
	table->setItem( 4, 0, new QTableWidgetItem( QString( QChar( 917 ) ) ) ); 
	table->setItem( 5, 0, new QTableWidgetItem( QString( QChar( 918 ) ) ) ); 
	table->setItem( 6, 0, new QTableWidgetItem( QString( QChar( 919 ) ) ) ); 
	table->setItem( 7, 0, new QTableWidgetItem( QString( QChar( 920 ) ) ) ); 
	table->setItem( 8, 0, new QTableWidgetItem( QString( QChar( 921 ) ) ) ); 
	table->setItem( 9, 0, new QTableWidgetItem( QString( QChar( 922 ) ) ) ); 
	table->setItem( 10,0, new QTableWidgetItem( QString( QChar( 923 ) ) ) ); 
	table->setItem( 11,0, new QTableWidgetItem( QString( QChar( 924 ) ) ) ); 
	table->setItem( 12,0, new QTableWidgetItem( QString( QChar( 925 ) ) ) ); 
	table->setItem( 13,0, new QTableWidgetItem( QString( QChar( 926 ) ) ) ); 
	table->setItem( 14,0, new QTableWidgetItem( QString( QChar( 927 ) ) ) ); 
	table->setItem( 15,0, new QTableWidgetItem( QString( QChar( 928 ) ) ) ); 
	table->setItem( 16,0, new QTableWidgetItem( QString( QChar( 929 ) ) ) ); 
	table->setItem( 17,0, new QTableWidgetItem( QString( QChar( 931 ) ) ) ); 
	table->setItem( 18,0, new QTableWidgetItem( QString( QChar( 932 ) ) ) ); 
	table->setItem( 19,0, new QTableWidgetItem( QString( QChar( 933 ) ) ) ); 
	table->setItem( 20,0, new QTableWidgetItem( QString( QChar( 934 ) ) ) ); 
	table->setItem( 21,0, new QTableWidgetItem( QString( QChar( 935 ) ) ) ); 
	table->setItem( 22,0, new QTableWidgetItem( QString( QChar( 936 ) ) ) ); 
	table->setItem( 23,0, new QTableWidgetItem( QString( QChar( 937 ) ) ) ); 

	//small letters
	table->setItem( 0, 1, new QTableWidgetItem( QString( QChar( 945 ) ) ) ); //small alpha
	table->setItem( 1, 1, new QTableWidgetItem( QString( QChar( 946 ) ) ) ); 
	table->setItem( 2, 1, new QTableWidgetItem( QString( QChar( 947 ) ) ) ); 
	table->setItem( 3, 1, new QTableWidgetItem( QString( QChar( 948 ) ) ) ); 
	table->setItem( 4, 1, new QTableWidgetItem( QString( QChar( 949 ) ) ) ); 
	table->setItem( 5, 1, new QTableWidgetItem( QString( QChar( 950 ) ) ) ); 
	table->setItem( 6, 1, new QTableWidgetItem( QString( QChar( 951 ) ) ) ); 
	table->setItem( 7, 1, new QTableWidgetItem( QString( QChar( 952 ) ) ) ); 
	table->setItem( 8, 1, new QTableWidgetItem( QString( QChar( 953 ) ) ) ); 
	table->setItem( 9, 1, new QTableWidgetItem( QString( QChar( 954 ) ) ) ); 
	table->setItem( 10,1, new QTableWidgetItem( QString( QChar( 955 ) ) ) ); 
	table->setItem( 11,1, new QTableWidgetItem( QString( QChar( 956 ) ) ) ); 
	table->setItem( 12,1, new QTableWidgetItem( QString( QChar( 957 ) ) ) ); 
	table->setItem( 13,1, new QTableWidgetItem( QString( QChar( 958 ) ) ) ); 
	table->setItem( 14,1, new QTableWidgetItem( QString( QChar( 959 ) ) ) ); 
	table->setItem( 15,1, new QTableWidgetItem( QString( QChar( 960 ) ) ) ); 
	table->setItem( 16,1, new QTableWidgetItem( QString( QChar( 961 ) ) ) ); 
	table->setItem( 17,1, new QTableWidgetItem( QString( QChar( 962 ) )+", "+QString( QChar( 963 ) ) ) ); //there are two greek letters for sigma
	table->setItem( 18,1, new QTableWidgetItem( QString( QChar( 964 ) ) ) ); 
	table->setItem( 19,1, new QTableWidgetItem( QString( QChar( 965 ) ) ) ); 
	table->setItem( 20,1, new QTableWidgetItem( QString( QChar( 966 ) ) ) ); 
	table->setItem( 21,1, new QTableWidgetItem( QString( QChar( 967 ) ) ) ); 
	table->setItem( 22,1, new QTableWidgetItem( QString( QChar( 968 ) ) ) ); 
	table->setItem( 23,1, new QTableWidgetItem( QString( QChar( 969 ) ) ) ); 
	
	//english names
	table->setItem( 0, 2, new QTableWidgetItem( i18n("alpha" ) ) );
	table->setItem( 1, 2, new QTableWidgetItem( i18n("beta" ) ) ); 
	table->setItem( 2, 2, new QTableWidgetItem( i18n("gamma" ) ) ); 
	table->setItem( 3, 2, new QTableWidgetItem( i18n("delta" ) ) ); 
	table->setItem( 4, 2, new QTableWidgetItem( i18n("epsilon" ) ) ); 
	table->setItem( 5, 2, new QTableWidgetItem( i18n("zeta" ) ) ); 
	table->setItem( 6, 2, new QTableWidgetItem( i18n("eta" ) ) ); 
	table->setItem( 7, 2, new QTableWidgetItem( i18n("theta" ) ) ); 
	table->setItem( 8, 2, new QTableWidgetItem( i18n("iota" ) ) ); 
	table->setItem( 9, 2, new QTableWidgetItem( i18n("kappa" ) ) ); 
	table->setItem( 10,2, new QTableWidgetItem( i18n("lambda" ) ) ); 
	table->setItem( 11,2, new QTableWidgetItem( i18n("my" ) ) ); 
	table->setItem( 12,2, new QTableWidgetItem( i18n("ny" ) ) ); 
	table->setItem( 13,2, new QTableWidgetItem( i18n("xi" ) ) ); 
	table->setItem( 14,2, new QTableWidgetItem( i18n("omicron" ) ) ); 
	table->setItem( 15,2, new QTableWidgetItem( i18n("pi" ) ) ); 
	table->setItem( 16,2, new QTableWidgetItem( i18n("rho" ) ) ); 
	table->setItem( 17,2, new QTableWidgetItem( i18n("sigma" ) ) ); 
	table->setItem( 18,2, new QTableWidgetItem( i18n("tau" ) ) ); 
	table->setItem( 19,2, new QTableWidgetItem( i18n("ypsilon" ) ) ); 
	table->setItem( 20,2, new QTableWidgetItem( i18n("phi" ) ) ); 
	table->setItem( 21,2, new QTableWidgetItem( i18n("chi" ) ) ); 
	table->setItem( 22,2, new QTableWidgetItem( i18n("pse" ) ) ); 
	table->setItem( 23,2, new QTableWidgetItem( i18n("omega" ) ) ); 
}

void TablesDialog::createNumbersTable()
{
	QWidget *frame = new QWidget();
	KPageWidgetItem *item = addPage( frame, i18n( "Numbers" ) );
	item->setHeader( i18n( "Greek and Roman Numbers" ) );
	item->setIcon( KIcon( "numbers" ) );
	QVBoxLayout *layout = new QVBoxLayout( frame );
	layout->setMargin( 0 );
	
	QTableWidget *table = new QTableWidget(frame);
	table->verticalHeader()->hide();

	QStringList l; 
		l.append( i18n( "Number" ) );
		l.append( i18n( "Greek Word" ) );
		l.append( i18n( "Roman Symbol" ) );
		table->setHorizontalHeaderLabels( l );

	table->setColumnCount( 3 );
	table->setRowCount( 28 );
	
	layout->addWidget( table );

	table->setItem( 0, 0, new QTableWidgetItem( i18n( "0.5" )  ) );
	table->setItem( 1, 0, new QTableWidgetItem( i18n( "1" )  ) ); 
	table->setItem( 2, 0, new QTableWidgetItem( i18n( "1.5" )  ) ); 
	table->setItem( 3, 0, new QTableWidgetItem( i18n( "2"  ) ) ); 
	table->setItem( 4, 0, new QTableWidgetItem( i18n( "2.5" ) ) ); 
	table->setItem( 5, 0, new QTableWidgetItem( i18n( "3"  ) ) ); 
	table->setItem( 6, 0, new QTableWidgetItem( i18n( "4"  ) ) ); 
	table->setItem( 7, 0, new QTableWidgetItem( i18n( "5"  ) ) ); 
	table->setItem( 8, 0, new QTableWidgetItem( i18n( "6"  ) ) ); 
	table->setItem( 9, 0, new QTableWidgetItem( i18n( "7"  ) ) ); 
	table->setItem( 10,0, new QTableWidgetItem( i18n( "8"  ) ) ); 
	table->setItem( 11,0, new QTableWidgetItem( i18n( "9"  ) ) ); 
	table->setItem( 12,0, new QTableWidgetItem( i18n( "10"  ) ) ); 
	table->setItem( 13,0, new QTableWidgetItem( i18n( "11"  ) ) ); 
	table->setItem( 14,0, new QTableWidgetItem( i18n( "12"  ) ) ); 
	table->setItem( 15,0, new QTableWidgetItem( i18n( "13"  ) ) ); 
	table->setItem( 16,0, new QTableWidgetItem( i18n( "14"  ) ) ); 
	table->setItem( 17,0, new QTableWidgetItem( i18n( "15"  ) ) ); 
	table->setItem( 18,0, new QTableWidgetItem( i18n( "16"  ) ) ); 
	table->setItem( 19,0, new QTableWidgetItem( i18n( "17"  ) ) ); 
	table->setItem( 20,0, new QTableWidgetItem( i18n( "18"  ) ) ); 
	table->setItem( 21,0, new QTableWidgetItem( i18n( "19"  ) ) ); 
	table->setItem( 22,0, new QTableWidgetItem( i18n( "20"  ) ) ); 
	table->setItem( 23,0, new QTableWidgetItem( i18n( "40"  ) ) ); 
	table->setItem( 24,0, new QTableWidgetItem( i18n( "50"  ) ) ); 
	table->setItem( 25,0, new QTableWidgetItem( i18n( "60"  ) ) ); 
	table->setItem( 26,0, new QTableWidgetItem( i18n( "90"  ) ) ); 
	table->setItem( 27,0, new QTableWidgetItem( i18n( "100"  ) ) ); 

	//greek names of the numbers
	table->setItem( 0, 1, new QTableWidgetItem( "hemi"  ) );
	table->setItem( 1, 1, new QTableWidgetItem( "mono"  ) ); 
	table->setItem( 2, 1, new QTableWidgetItem( "sesqui"  ) ); 
	table->setItem( 3, 1, new QTableWidgetItem( "di, bi"  ) ); 
	table->setItem( 4, 1, new QTableWidgetItem( "hemipenta"  ) ); 
	table->setItem( 5, 1, new QTableWidgetItem( "tri"  ) ); 
	table->setItem( 6, 1, new QTableWidgetItem( "tetra"  ) ); 
	table->setItem( 7, 1, new QTableWidgetItem( "penta"  ) ); 
	table->setItem( 8, 1, new QTableWidgetItem( "hexa"  ) ); 
	table->setItem( 9, 1, new QTableWidgetItem( "hepta"  ) ); 
	table->setItem( 10,1, new QTableWidgetItem( "octa"  ) ); 
	table->setItem( 11,1, new QTableWidgetItem( "nona, ennea"  ) ); 
	table->setItem( 12,1, new QTableWidgetItem( "deca"  ) ); 
	table->setItem( 13,1, new QTableWidgetItem( "hendeca, undeca"  ) ); 
	table->setItem( 14,1, new QTableWidgetItem( "dodeca"  ) ); 
	table->setItem( 15,1, new QTableWidgetItem( "trideca"  ) ); 
	table->setItem( 16,1, new QTableWidgetItem( "tetradeca"  ) ); 
	table->setItem( 17,1, new QTableWidgetItem( "pentadeca"  ) ); 
	table->setItem( 18,1, new QTableWidgetItem( "hexadeca"  ) ); 
	table->setItem( 19,1, new QTableWidgetItem( "heptadeca"  ) ); 
	table->setItem( 20,1, new QTableWidgetItem( "octadeca"  ) ); 
	table->setItem( 21,1, new QTableWidgetItem( "nonadeca"  ) ); 
	table->setItem( 22,1, new QTableWidgetItem( "eicosa"  ) ); 
	table->setItem( 23,1, new QTableWidgetItem( "tetraconta"  ) ); 
	table->setItem( 24,1, new QTableWidgetItem( "pentaconta"  ) ); 
	table->setItem( 25,1, new QTableWidgetItem( "hexaconta"  ) ); 
	table->setItem( 26,1, new QTableWidgetItem( "nonaconta"  ) ); 
	table->setItem( 27,1, new QTableWidgetItem( "hecta"  ) ); 
	
	//roman symbols
	table->setItem( 1, 2, new QTableWidgetItem( "I" ) );
	table->setItem( 3, 2, new QTableWidgetItem( "II" ) );
	table->setItem( 5, 2, new QTableWidgetItem( "III" ) );
	table->setItem( 6, 2, new QTableWidgetItem( "IV" ) );
	table->setItem( 7, 2, new QTableWidgetItem( "V" ) );
	table->setItem( 8, 2, new QTableWidgetItem( "VI" ) );
	table->setItem( 9, 2, new QTableWidgetItem( "VII" ) );
	table->setItem( 10,2, new QTableWidgetItem( "VIII" ) );
	table->setItem( 11,2, new QTableWidgetItem( "IX" ) );
	table->setItem( 12,2, new QTableWidgetItem( "X" ) );
	table->setItem( 13,2, new QTableWidgetItem( "XI" ) );
	table->setItem( 14,2, new QTableWidgetItem( "XII" ) ); 
	table->setItem( 15,2, new QTableWidgetItem( "XIII" ) );
	table->setItem( 16,2, new QTableWidgetItem( "XIV" ) );
	table->setItem( 17,2, new QTableWidgetItem( "XV" ) );
	table->setItem( 18,2, new QTableWidgetItem( "XVI" ) );
	table->setItem( 19,2, new QTableWidgetItem( "XVII" ) ); 
	table->setItem( 20,2, new QTableWidgetItem( "XVIII" ) );
	table->setItem( 21,2, new QTableWidgetItem( "XIV" ) );
	table->setItem( 22,2, new QTableWidgetItem( "XX" ) );
	table->setItem( 23,2, new QTableWidgetItem( "XL" ) );
	table->setItem( 24,2, new QTableWidgetItem( "L" ) );
	table->setItem( 25,2, new QTableWidgetItem( "LX" ) );
	table->setItem( 26,2, new QTableWidgetItem( "XC" ) );
	table->setItem( 27,2, new QTableWidgetItem( "C" ) );
	
}

TablesDialog::~TablesDialog()
{
}

#include "tablesdialog.moc"

