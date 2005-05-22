/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>
#include <klocale.h>
#include <kiconloader.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kglobal.h>
#include <klineedit.h>
#include <klistview.h>
#include <klistviewsearchline.h>
#include <kmessagebox.h>

#include <qlabel.h>
#include <qheader.h>
#include <qimage.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qregexp.h>
#include <qsizepolicy.h>
#include <qsplitter.h>

#include "glossarydialog.h"
#include "detailinfodlg.h"
#include "orbitswidget.h"
#include "prefs.h"
#include "isotopewidget.h"
#include "detailedgraphicaloverview.h"

GlossaryDialog::GlossaryDialog( QWidget *parent, const char *name)
    : KDialogBase( Plain, i18n( "Glossary of chemical expressions" ), Close, Close, parent, name, false )
{
	QString m_baseHtml = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	m_baseHtml.append("kalzium/data/");
	m_baseHtml.append("bg.jpg");
	
	QString m_htmlbasestring = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body background=\"" ;
	m_htmlbasestring.append( m_baseHtml );
	m_htmlbasestring.append("\">");
	
	QVBoxLayout *vbox = new QVBoxLayout( plainPage(), 0, KDialog::spacingHint() );
	vbox->activate();

	QSplitter *vs = new QSplitter( plainPage() );
	vbox->addWidget( vs );
	
	m_glosstree = new KListView( vs, "treeview" );
	m_glosstree->addColumn( "entries" );
	m_glosstree->header()->hide();
	m_glosstree->setFullWidth( true );
	m_glosstree->setRootIsDecorated( true );

	m_htmlpart = new KHTMLPart( vs, "html-part" );

	QHBoxLayout *hbox = new QHBoxLayout( 0L, 0, KDialog::spacingHint() );
	hbox->activate();

	QLabel *lbl = new QLabel( plainPage() );
	lbl->setText( i18n( "Search:" ) );
	hbox->addWidget( lbl );

	m_search = new KListViewSearchLine( plainPage(), m_glosstree, "search-line" );
	hbox->addWidget( m_search );
	QSizePolicy p = m_search->sizePolicy();
	m_search->setSizePolicy( QSizePolicy( p.horData(), QSizePolicy::Fixed ) );
	vbox->addLayout( hbox );

	QDomDocument doc( "foo" );
	QString filename = "knowledge.xml";

	if ( loadLayout( doc, filename ) )
		m_itemList = readItems( doc );

	populateTree();

	connect( m_htmlpart->browserExtension(), SIGNAL( openURLRequestDelayed( const KURL &, const KParts::URLArgs & ) ), this, SLOT( displayItem( const KURL &, const KParts::URLArgs & ) ) );
	connect( m_glosstree, SIGNAL(clicked( QListViewItem * )), this, SLOT(slotClicked( QListViewItem * )));
	connect( this, SIGNAL(closeClicked()), SLOT(slotClose()) );

	resize( 550, 400 );
}

GlossaryDialog::~GlossaryDialog()
{
}

void GlossaryDialog::displayItem( const KURL& url, const KParts::URLArgs& )
{
	QString myurl = url.host().lower();
	m_search->setText( "" );
	
	m_search->updateSearch( "" );
	QListViewItem *found = 0;
	QListViewItemIterator it( m_glosstree );
	QListViewItem *item;
	while ( it.current() )
	{
		item = it.current();
		// using the "host" part of a kurl as reference
		if ( item->text(0).lower() == myurl )
		{
			found = item;
			break;
		}
		++it;
	}
	slotClicked( found );
}

void GlossaryDialog::populateTree()
{
	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
	
	QListViewItem *main = new QListViewItem( m_glosstree, i18n( "Main Glossary" ) );
	main->setExpandable( true );
	main->setSelectable( false );
	for ( ; it != itEnd ; ++it )
	{
		QChar thisletter = ( *it )->name().upper()[0];
		QListViewItem *thisletteritem = findTreeWithLetter( thisletter, main );
		if ( !thisletteritem )
		{
			thisletteritem = new QListViewItem( main, thisletter );
			thisletteritem->setExpandable( true );
			thisletteritem->setSelectable( false );
		}
		new QListViewItem( thisletteritem, ( *it )->name() );
	}
	main->sort();
}

QListViewItem* GlossaryDialog::findTreeWithLetter( const QChar& l, QListViewItem* i )
{
	QListViewItem *it = i->firstChild();
        while ( it )
	{
		if ( it->text(0)[0] == l )
			return it;
		it = it->nextSibling();
        }
	return 0;
}

void GlossaryDialog::slotClicked( QListViewItem *item )
{
	if ( !item )
		return;
	
	QString html = m_htmlbasestring;
	
	/**
	 * The next lines are searching for the correct KnowledgeItem
	 * in the m_itemList. When it is found the HTML will be
	 * generated
	 */
	QValueList<KnowledgeItem*>::iterator it = m_itemList.begin();
	const QValueList<KnowledgeItem*>::iterator itEnd = m_itemList.end();
	bool found = false;
	KnowledgeItem *i = 0;
	while ( !found && it != itEnd )
	{
		if ( ( *it )->name() == item->text( 0 ) )
		{
			i = *it;
			found = true;
		}
		++it;
	}
	
	html.append( itemHtml( i ) );
	html.append( "</body></html>" );
	m_htmlpart->begin();
	m_htmlpart->write( html );
	m_htmlpart->end();
}

QString GlossaryDialog::itemHtml( KnowledgeItem* item )
{
	if ( !item ) return "";

	QString code = "<h1>";
	code.append( item->name() );
	code.append( "</h1>" );

	QString pic_path = locate("data", "kalzium/data/knowledgepics/");
	code.append(  item->desc() );
	if ( !item->ref().isNull() )
	{
		QString refcode = parseReferences( item->ref() );
		code.append( refcode );
	}
	return code;
}

QString GlossaryDialog::parseReferences( const QString& ref )
{
	QString code = ref;

	QString htmlcode = i18n( "<h3>References</h3>" );
	
	int pos, l;
	for ( int num = 0; num < ref.contains( "," ); ++num )
	{
		pos = code.find( "," );
		l = code.length();
		QString tmp = code.left( pos );
		QString new_code = code.right( l-pos-1 );

		htmlcode.append( "<a href=\"item://" );
		htmlcode.append( tmp );
		htmlcode.append( "\">" );
		htmlcode.append( tmp );
		htmlcode.append( "</a><br>" );

		code = new_code;
	}
	htmlcode.append( "<a href=\"item://" );
	htmlcode.append( code );
	htmlcode.append( "\">" );
	htmlcode.append( code );
	htmlcode.append( "</a>" );

	return htmlcode;
}

bool GlossaryDialog::loadLayout( QDomDocument &questionDocument, const QString& filename )
{
        KURL url;
        url.setPath( locate("data", "kalzium/data/"));
		url.setFileName( filename );

        QFile layoutFile( url.path() );

        if (!layoutFile.exists())
        {
                KMessageBox::information( 0, i18n("Error"), i18n( "Loading File - Error" ) );
        }

		//TODO really needed?
        if (!layoutFile.open(IO_ReadOnly))
                return false;

        ///Check if document is well-formed
        if (!questionDocument.setContent(&layoutFile))
        {
                kdDebug() << "wrong xml" << endl;
                layoutFile.close();
                return false;
        }
        layoutFile.close();

        return true;
}

QValueList<KnowledgeItem*> GlossaryDialog::readItems( QDomDocument &itemDocument )
{
	QValueList<KnowledgeItem*> list;

	QDomNodeList itemList;
	QDomElement itemElement;

	itemList = itemDocument.elementsByTagName( "item" );

	const uint num = itemList.count();
	for ( uint i = 0; i < num; ++i )
	{
		KnowledgeItem *item = new KnowledgeItem();
		
		itemElement = ( const QDomElement& ) itemList.item( i ).toElement();
		
		QDomNode nameNode = itemElement.namedItem( "name" );
		QDomNode descNode = itemElement.namedItem( "desc" );
		QDomNode refNode =  itemElement.namedItem( "ref" );

		QDomElement tmpElement = descNode.toElement();
		QDomNodeList domList = tmpElement.elementsByTagName( "img" );
//X 		for ( uint i = 0; i < domList.count() ; i++ )
//X 		{
//X 			kdDebug() << domList.item( i ).toElement().attribute( "src" ) << endl;
//X 		}
		
		item->setName( nameNode.toElement( ).text() );
		item->setDesc( descNode.toElement( ).text() );
//X 		kdDebug() << "item->desc() " << item->desc() << endl;
		item->setRef( refNode.toElement( ).text() );
		
		list.append( item );
	}
	
	return list;
}

void GlossaryDialog::slotClose()
{
	emit closed();
	accept();
}

KnowledgeItem::KnowledgeItem()
{
}

KnowledgeItem::~KnowledgeItem()
{
}

#include "glossarydialog.moc"
