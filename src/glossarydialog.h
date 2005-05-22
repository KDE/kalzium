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
#include <kdialogbase.h>

#include <qdom.h>

class QChar;
class QListViewItem;
class KHTMLView;
class KListView;
class KListViewSearchLine;

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

class ToolItem
{
	public:
		ToolItem();
		~ToolItem();

		void setName( const QString& s ){
			m_name = s;
		}
		void setDesc( const QString& s){
			m_desc = s;
		}
		void setPicture( const QString& s){
			m_pic = s;
		}

		QString name() const {
			return m_name;
		}
		
		QString desc() const {
			return m_desc;
		}
		
		QString picture() const {
			return m_pic;
		}

	private:
		QString m_name;
		QString m_desc;
		QString m_pic;
};

class GlossaryDialog : public KDialogBase
{
    Q_OBJECT
    
	public:
        	GlossaryDialog( QWidget *parent=0, const char *name=0);
		~GlossaryDialog();
	
	private:
		KHTMLPart *m_htmlpart;
		QString m_htmlbasestring;
		KListView *m_glosstree;

		void populateTree();
		QListViewItem* findTreeWithLetter( const QChar&, QListViewItem* );

		KListViewSearchLine *m_search;

		/**
		 * @return the formated html-code for the @param item 
		 **/
		QString itemHtml( KnowledgeItem* item );
		QString toolHtml( ToolItem* item );

		/**
		 * this method parses the references: It checks for the ","
		 * inside the QString and puts the words into html-links
		 */
		QString parseReferences( const QString& );
		
		bool loadLayout( QDomDocument&, const QString& );

		QValueList<KnowledgeItem*> m_itemList;
		QValueList<ToolItem*> m_toolList;
			
		QValueList<KnowledgeItem*> readItems( QDomDocument& );
		QValueList<ToolItem*> readTools( QDomDocument& );
	
	private slots:
		void slotClicked( QListViewItem * );
		/**
		 * The user clicked on a href. Emit the corresponding item
		 */
		void displayItem( const KURL& url, const KParts::URLArgs& args );

		void slotClose();
	
	signals:
		void closed();
};
#endif // GLOSSARYDIALOG_H

