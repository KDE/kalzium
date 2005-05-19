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

		void setName( const QString& s ){
			m_name = s;
		}
		void setDesc( const QString& s){
			m_desc = s;
		}
		void setRef( const QString& s){
			m_ref = s;
		}

		QString name() const {
			return m_name;
		}
		
		QString desc() const {
			return m_desc;
		}
		
		QString ref() const {
			return m_ref;
		}

	private:
		QString m_name,
				m_desc,
				m_ref;
};

class GlossaryDialog : public KDialog
{
    Q_OBJECT
    
	public:
        	GlossaryDialog( QWidget *parent=0, const char *name=0);
	
	private:
		KHTMLPart *m_htmlpart;
		QString m_htmlbasestring;
		QListBox *itembox;

		void populateList();

		/**
		 * @return the formated html-code for the @p item 
		 **/
		QString itemHtml( KnowledgeItem* item );

		/**
		 * this method parses the references: It checks for the ","
		 * inside the QString and puts the words into html-links
		 */
		QString parseReferences( const QString& );
		
		bool loadLayout( QDomDocument&, const QString& );

		QValueList<KnowledgeItem*> m_itemList;
			
		QValueList<KnowledgeItem*> readItems( QDomDocument& );
	
	public slots:
		void closeEvent(QCloseEvent*e);

	private slots:
		void itemClicked( QListBoxItem* );
		/**
		 * The user clicked on a href. Emit the corresponding item
		 */
		void displayItem( const KURL& url, const KParts::URLArgs& args );
	
	signals:
		void closed();
};
#endif // GLOSSARYDIALOG_H

