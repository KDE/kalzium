/***************************************************************************

                           KalziumGraph.h  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#ifndef _KALZIUMGRAPH_H
#define _KALZIUMGRAPH_H

class KComboBox;
class KLineEdit;
class QGridLayout;
class QLabel;
class KIntNumInput;
class KalziumGraphDataContainer;
class QListView;
class QListViewItem;

#include <kdialog.h>
#include <qframe.h>
class KalziumGraph;

class KalziumGraphDialog : public KDialog
{
	Q_OBJECT

	public:
		KalziumGraphDialog( QWidget *parent=0, const char *name =0 );
		QGridLayout *grid;
		KalziumGraph *graph;
		QListView *list;
	
	private:
		KComboBox *kcb;
		KIntNumInput *from, *to;
		QLabel *topicLabel;
		bool valuesAreOk();
		void fillList( int, int , KalziumGraphDataContainer*);
	
	public slots:
		void slotokClicked();
		void itemSelected( QListViewItem* );
};

class KalziumGraphDataContainer
{
	public:
		KalziumGraphDataContainer( int, int, int);
		double Data[ 109 ];
		QString Symbols[ 109 ];
};

//Datenpräsentation
class KalziumGraph : public QFrame
{
	Q_OBJECT

	public:
		KalziumGraph( int , int , QWidget *parent = 0, const char *name=0, KalziumGraphDataContainer *data=0);

		KalziumGraphDataContainer *data;
		int fromRange_, toRange_;

	private:
		double getMax();
		double getMin();
		void calculate( int&, int& );


	protected slots:
		virtual void paintEvent( QPaintEvent * );


};

#endif
