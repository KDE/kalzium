#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

/***************************************************************************
 *     copyright            : (C) 2005 by Martin Pfeiffer
 *     email                : hubipete@gmx.net
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QWidget>
#include <QListWidget>
#include "printwidgetbase.h"

class PrintListWidget;

/**
 * @author Martin Pfeiffer <hubipete@gmx.net>
 */
class PrintWidget : public QWidget, private Ui::PrintWidgetBase
{
	Q_OBJECT

	public:
		/**
 		 * public constructor
 		 * @param parent the widgets parent
 		 */
		PrintWidget( QWidget *parent );
	
	private slots:
		void slotPrint();

	private:
		PrintListWidget* m_printList;
};

/**
 * @author Martin Pfeiffer <hubipete@gmx.net>
 */
class PrintListWidget : public QListWidget
{
	Q_OBJECT

	public:
		/**
		 * public constructor
		 * @param parent the widgets parent
		 */
		PrintListWidget( QWidget *parent );
	
	protected:
		virtual void dropEvent( QDropEvent* event );
		virtual void dragEnterEvent( QDragEnterEvent* event );
		virtual void dragMoveEvent( QDragMoveEvent* event );
};

#endif // PRINTWIDGET_H

