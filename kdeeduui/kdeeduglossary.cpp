/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kdeeduglossary.h"

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kglobal.h>
#include <klistview.h>
#include <klistviewsearchline.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>

#include <qfile.h>
#include <qlabel.h>
#include <q3header.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qstringlist.h>
#include <qtoolbutton.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QHBoxLayout>
#include <Q3ValueList>
#include <QVBoxLayout>

Glossary::Glossary()
{
	// setting a generic name for a new glossary
	m_name = i18n( "Glossary" );
}

Glossary::~Glossary()
{
}

bool Glossary::loadLayout( QDomDocument &Document, const KURL& url )
{
        QFile layoutFile( url.path() );

        if (!layoutFile.exists())
	{
		kdDebug() << "no such file: " << layoutFile.name() << endl;
		return false;
	}

        if (!layoutFile.open(QIODevice::ReadOnly))
                return false;

        ///Check if document is well-formed
        if (!Document.setContent(&layoutFile))
        {
                kdDebug() << "wrong xml" << endl;
                layoutFile.close();
                return false;
        }
        layoutFile.close();

        return true;
}

bool Glossary::isEmpty() const
{
	return m_itemlist.count() == 0;
}


Glossary* Glossary::readFromXML( const KURL& url, const QString& path )
{
	QDomDocument doc( "document" );

	Glossary *glossary = new Glossary();
	
	glossary->setPicturePath( path );

	if ( glossary->loadLayout( doc, url ) )
	{
		Q3ValueList<GlossaryItem*> itemList;
		itemList = glossary->readItems( doc );
		glossary->setItemlist( itemList );
		glossary->fixImagePath();
	}

	return glossary;
}

void Glossary::fixImagePath()
{
	kdDebug() << "Glossary::fixImagePath()" << endl;
	Q3ValueList<GlossaryItem*>::iterator it = m_itemlist.begin();
	const Q3ValueList<GlossaryItem*>::iterator itEnd = m_itemlist.end();
	QString path = m_picturepath;
	QString firstpart = "<img src=\"";
	firstpart += path;

	for ( ; it != itEnd ; ++it )
	{
		( *it )->setDesc( ( *it )->desc().replace("[img]", firstpart ) );
		( *it )->setDesc( ( *it )->desc().replace("[/img]", "\" />" ) );
	}
}
	
Q3ValueList<GlossaryItem*> Glossary::readItems( QDomDocument &itemDocument )
{
	Q3ValueList<GlossaryItem*> list;

	QDomNodeList itemList;
	QDomNodeList refNodeList;
	QDomElement itemElement;
	QStringList reflist;

	itemList = itemDocument.elementsByTagName( "item" );

	const uint num = itemList.count();
	for ( uint i = 0; i < num; ++i )
	{
		reflist.clear();
		GlossaryItem *item = new GlossaryItem();
		
		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
		
		QDomNode nameNode = itemElement.namedItem( "name" );
		QDomNode descNode = itemElement.namedItem( "desc" );
		
		QString picName = itemElement.namedItem( "picture" ).toElement().text();
		QDomElement refNode = ( const QDomElement& ) itemElement.namedItem( "references" ).toElement();

		QString desc = descNode.toElement().text();
		if ( !picName.isEmpty() )
			desc.prepend("[img]"+picName +"[/img]" );

		item->setName( i18n( nameNode.toElement( ).text().utf8() ) );
		
		item->setDesc( desc.replace("[b]", "<b>" ) );
		item->setDesc( item->desc().replace("[/b]", "</b>" ) );
		item->setDesc( item->desc().replace("[i]", "<i>" ) );
		item->setDesc( item->desc().replace("[/i]", "</i>" ) );
		item->setDesc( item->desc().replace("[sub]", "<sub>" ) );
		item->setDesc( item->desc().replace("[/sub]", "</sub>" ) );
		item->setDesc( item->desc().replace("[sup]", "<sup>" ) );
		item->setDesc( item->desc().replace("[/sup]", "</sup>" ) );
		item->setDesc( item->desc().replace("[br]", "<br />" ) );
		
		item->setDesc( i18n( item->desc().utf8() ) );

		refNodeList = refNode.elementsByTagName( "refitem" );
		for ( uint it = 0; it < refNodeList.count(); it++ )
		{
			reflist << i18n( refNodeList.item( it ).toElement().text().utf8() );
		}
		reflist.sort();
		item->setRef( reflist );
		
		list.append( item );
	}
	
	return list;
}



GlossaryDialog::GlossaryDialog( bool folded, QWidget *parent, const char *name)
    : KDialogBase( Plain, i18n( "Glossary" ), Close, NoDefault, parent, name, false )
{
	//this string will be used for all items. If a backgroundpicture should
	//be used call Glossary::setBackgroundPicture().
	m_htmlbasestring = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body>" ;

	m_folded = folded;
	
	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, KDialog::spacingHint() );
	vbox->activate();

	QHBoxLayout *hbox = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
	hbox->activate();

	QToolButton *clear = new QToolButton( plainPage() );
	clear->setIconSet( SmallIconSet( "locationbar_erase" ) );
	hbox->addWidget( clear );

	QLabel *lbl = new QLabel( plainPage() );
	lbl->setText( i18n( "Search:" ) );
	hbox->addWidget( lbl );

	m_search = new KListViewSearchLine( plainPage(), 0, "search-line" );
	hbox->addWidget( m_search );
	vbox->addLayout( hbox );
	setFocusProxy(m_search);
 
	QSplitter *vs = new QSplitter( plainPage() );
	vbox->addWidget( vs );

	m_glosstree = new KListView( vs, "treeview" );
	m_glosstree->addColumn( "entries" );
	m_glosstree->header()->hide();
	m_glosstree->setFullWidth( true );
	m_glosstree->setRootIsDecorated( true );
 
	m_search->setListView( m_glosstree );
 
	m_htmlpart = new KHTMLPart( vs, "html-part" );
 
	connect( m_htmlpart->browserExtension(), SIGNAL( openURLRequestDelayed( const KURL &, const KParts::URLArgs & ) ), this, SLOT( displayItem( const KURL &, const KParts::URLArgs & ) ) );
	connect( m_glosstree, SIGNAL(clicked( Q3ListViewItem * )), this, SLOT(slotClicked( Q3ListViewItem * )));
	connect( clear, SIGNAL(clicked()), m_search, SLOT(clear()));
 
	resize( 600, 400 );
}

GlossaryDialog::~GlossaryDialog()
{
}

void GlossaryDialog::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Return) {
		e->ignore();
	}
	KDialogBase::keyPressEvent(e);
}

void GlossaryDialog::displayItem( const KURL& url, const KParts::URLArgs& )
{
	// using the "host" part of a kurl as reference
	QString myurl = url.host().lower();
	m_search->setText( "" );
	m_search->updateSearch( "" );
	Q3ListViewItem *found = 0;
	Q3ListViewItemIterator it( m_glosstree );
	Q3ListViewItem *item;
	while ( it.current() )
	{
		item = it.current();
		if ( item->text(0).lower() == myurl )
		{
			found = item;
			break;
		}
		++it;
	}
	slotClicked( found );
}

void GlossaryDialog::updateTree()
{
	m_glosstree->clear();

	Q3ValueList<Glossary*>::const_iterator itGl = m_glossaries.begin();
	const Q3ValueList<Glossary*>::const_iterator itGlEnd = m_glossaries.end();
	
	for ( ; itGl != itGlEnd ; ++itGl )
	{
		Q3ValueList<GlossaryItem*> items = ( *itGl )->itemlist();
		Q3ValueList<GlossaryItem*>::iterator it = items.begin();
		const Q3ValueList<GlossaryItem*>::iterator itEnd = items.end();

		Q3ListViewItem *main = new Q3ListViewItem( m_glosstree, ( *itGl )->name() );
		main->setExpandable( true );
		main->setSelectable( false );
		//XXX TMP!!!
		bool foldinsubtrees = m_folded;

		for ( ; it != itEnd ; ++it )
		{
			if ( foldinsubtrees )
			{
				QChar thisletter = ( *it )->name().upper()[0];
				Q3ListViewItem *thisletteritem = findTreeWithLetter( thisletter, main );
				if ( !thisletteritem )
				{
					thisletteritem = new Q3ListViewItem( main, thisletter );
					thisletteritem->setExpandable( true );
					thisletteritem->setSelectable( false );
				}
				new Q3ListViewItem( thisletteritem, ( *it )->name() );
			}
			else
				new Q3ListViewItem( main, ( *it )->name() );
		}
		main->sort();
	}
}

void GlossaryDialog::addGlossary( Glossary* newgloss )
{
	if ( !newgloss ) return;
	if ( newgloss->isEmpty() ) return;
	m_glossaries.append( newgloss );

	kdDebug() << "Count of the new glossary: " << newgloss->itemlist().count() << endl;
	kdDebug() << "Number of glossaries: " << m_glossaries.count() << endl;

	updateTree();
}

Q3ListViewItem* GlossaryDialog::findTreeWithLetter( const QChar& l, Q3ListViewItem* i )
{
	Q3ListViewItem *it = i->firstChild();
	while ( it )
	{
		if ( it->text(0)[0] == l )
			return it;
		it = it->nextSibling();
	}
	return 0;
}

void GlossaryDialog::slotClicked( Q3ListViewItem *item )
{
	if ( !item )
		return;
	
	/**
	 * The next lines are searching for the correct KnowledgeItem
	 * in the m_itemList. When it is found the HTML will be
	 * generated
	 */
	Q3ValueList<Glossary*>::iterator itGl = m_glossaries.begin();
	const Q3ValueList<Glossary*>::iterator itGlEnd = m_glossaries.end();
	bool found = false;
	GlossaryItem *i = 0;

	QString bg_picture;
	
	while ( !found && itGl != itGlEnd )
	{
		Q3ValueList<GlossaryItem*> items = ( *itGl )->itemlist();
		Q3ValueList<GlossaryItem*>::const_iterator it = items.begin();
		const Q3ValueList<GlossaryItem*>::const_iterator itEnd = items.end();
		while ( !found && it != itEnd )
		{
			if ( ( *it )->name() == item->text( 0 ) )
			{
				i = *it;
				bg_picture = ( *itGl )->backgroundPicture();
				found = true;
			}
			++it;
		}
		++itGl;
	}
	if ( found && i )
	{
		QString html;
		if ( !bg_picture.isEmpty() )
		{
			html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
			html.append( bg_picture );
			html.append("\">");
		}else
			html = m_htmlbasestring;
	
		html += i->toHtml() + "</body></html>";
		m_htmlpart->begin();
		m_htmlpart->write( html );

		m_htmlpart->end();
		return;
	}
}

void GlossaryDialog::slotClose()
{
	emit closed();
	accept();
}

QString GlossaryItem::toHtml() const
{
	QString code = "<h1>" + m_name + "</h1>" + m_desc;

	if ( !m_ref.isEmpty() )
	{
		QString refcode = parseReferences();
		code += refcode;
	}
	return code;
}

QString GlossaryItem::parseReferences() const
{
	QString htmlcode = "<h3>" + i18n( "References" ) + "</h3>";
	
	bool first = true;
	for ( uint i = 0; i < m_ref.size(); i++ )
	{
		if ( !first )
			htmlcode += "<br>";
		else
			first = false;
		htmlcode += QString( "<a href=\"item://%1\">%2</a>" ).arg( m_ref[i], m_ref[i] );
	}

	return htmlcode;
}


#include "kdeeduglossary.moc"
