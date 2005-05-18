#ifndef GLOSSARYDIALOG_H
#define GLOSSARYDIALOG_H
/***************************************************************************

    copyright            : (C) 2005 by Carsten Niehaus
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


#include <khtml_part.h>
#include <khtmlview.h>
#include <kdialog.h>

#include <qdom.h>

class KHTMLPart;
class KHTMLView;
class QListBox;

class KnowledgeItem
{
	public:
		KnowledgeItem();
		~KnowledgeItem();
};

class GlossaryDialog : public KDialog
{
    Q_OBJECT
    
	public:
        	GlossaryDialog( QWidget *parent=0, const char *name=0);
	
	private:
		KHTMLPart *m_htmlpart;
		KHTMLView *m_htmlview;
		QString m_htmlbasestring;
		QListBox *itembox;

		void populateList();

		/**
		 * @return the formated html-code for the item with a
		 * specific @p name
		 */
		QString itemHtml(const QString& name);

		bool loadLayout( QDomDocument&, const QString& );
		QValueList<KnowledgeItem*> readItems( QDomDocument& );


	private slots:
		void itemClicked( QListBoxItem* );
};
#endif // GLOSSARYDIALOG_H

