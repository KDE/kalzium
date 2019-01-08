/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus <cniehaus@kde.org>        *
 *   Copyright (C) 2005 - 2007 by Pino Toscano <pino@kde.org>              *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kdeeduglossary.h"

#include <KActionCollection>
#include <KConfigGroup>
#include <KHTMLPart>
#include <KHTMLView>
#include <KLocalizedString>
#include <KTreeWidgetSearchLine>

#include <QDebug>
#include <QDialogButtonBox>
#include <QDomDocument>
#include <QEvent>
#include <QFile>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QPushButton>
#include <QRegExp>
#include <QSplitter>
#include <QStringList>
#include <QTreeWidget>
#include <QVBoxLayout>

static const int FirstLetterRole = 0x00b00a00;

static const int GlossaryTreeItemType = QTreeWidgetItem::UserType + 1;

class GlossaryTreeItem : public QTreeWidgetItem
{
    public:
        GlossaryTreeItem(Glossary * g, GlossaryItem * gi)
            : QTreeWidgetItem(GlossaryTreeItemType), m_g(g), m_gi(gi)
        {
            setText(0, m_gi->name());
        }

        Glossary *glossary() const
        {
            return m_g;
        }

        GlossaryItem *glossaryItem() const
        {
            return m_gi;
        }

    private:
        Glossary *m_g;
        GlossaryItem *m_gi;
};


struct GlossaryInfo
{
    GlossaryInfo(Glossary* g)
        : glossary(g), folded(true)
    {
    }

    Glossary *glossary;
    bool folded;
};


class GlossaryDialog::Private
{
    public:
        Private(GlossaryDialog *qq) : q(qq)
        {
        }

        ~Private()
        {
            QList<GlossaryInfo>::ConstIterator it = m_glossaries.constBegin();
            QList<GlossaryInfo>::ConstIterator itEnd = m_glossaries.constEnd();
            for (; it != itEnd; ++it) {
                delete (*it).glossary;
            }

            delete m_htmlpart;
        }

        void rebuildTree();
        QTreeWidgetItem* createItem(const GlossaryInfo& gi) const;
        QTreeWidgetItem* findTreeWithLetter(const QChar& l, QTreeWidgetItem* item) const;

        // slots
        void itemActivated(QTreeWidgetItem * item, int column);
        // The user clicked on a href. Find and display the right item
        void displayItem(const QUrl &url, const KParts::OpenUrlArguments& arguments,
                         const KParts::BrowserArguments &browserArguments);

        GlossaryDialog *q;

        QList< GlossaryInfo > m_glossaries;

        KHTMLPart *m_htmlpart;
        QTreeWidget *m_glosstree;
        KTreeWidgetSearchLine *m_search;
        QString m_htmlbasestring;

        KActionCollection* m_actionCollection;
};


Glossary::Glossary(const QUrl &url, const QString& path)
{
    init(url, path);
}

Glossary::Glossary()
{
    init(QUrl(), QString());
}

Glossary::~Glossary()
{
    qDeleteAll(m_itemlist);
}

void Glossary::init(const QUrl &url, const QString& path)
{
    // setting a generic name for a new glossary
    m_name = i18n("Glossary");

    setPicturePath(path);

    if (!url.isEmpty()) {
        QDomDocument doc(QStringLiteral("document"));

        if (loadLayout(doc, url)) {
            setItemlist(readItems(doc));
            if (!m_picturepath.isEmpty()) {
                fixImagePath();
            }
        }
    }
}

bool Glossary::loadLayout(QDomDocument &Document, const QUrl &url)
{
    QFile layoutFile(url.path());

    if (!layoutFile.exists()) {
        qDebug() << "no such file: " << layoutFile.fileName();
        return false;
    }

    if (!layoutFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    // check if document is well-formed
    if (!Document.setContent(&layoutFile)) {
        qDebug() << "wrong xml of " << layoutFile.fileName();
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

void Glossary::setName(const QString& name)
{
    if (name.isEmpty()) {
        return;
    }
    m_name = name;
}

void Glossary::setPicturePath(const QString& path)
{
    if (path.isEmpty()) {
        return;
    }
    m_picturepath = path;
}

void Glossary::setBackgroundPicture(const QString& filename)
{
    if (filename.isEmpty()) {
        return;
    }
    m_backgroundpicture = filename;
}

void Glossary::fixImagePath()
{
    QString imgtag = "<img src=\"file://" + m_picturepath + '/' + "\\1\" />";
    QRegExp exp("\\[img\\]([^[]+)\\[/img\\]");

  foreach (GlossaryItem * item, m_itemlist) {
      QString tmp = item->desc();
      while (exp.indexIn(tmp) > -1) {
          tmp = tmp.replace(exp, imgtag);
      }
      item->setDesc(tmp);
  }
}

QList<GlossaryItem*> Glossary::readItems(QDomDocument &itemDocument)
{
    QList<GlossaryItem*> list;

    QDomNodeList itemList;
    QDomNodeList refNodeList;
    QDomElement itemElement;
    QStringList reflist;

    itemList = itemDocument.elementsByTagName(QStringLiteral("item"));

    const uint num = itemList.count();
    for (uint i = 0; i < num; ++i) {
        reflist.clear();
        GlossaryItem *item = new GlossaryItem();

        itemElement = (const QDomElement&) itemList.item(i).toElement();

        QDomNode nameNode = itemElement.namedItem(QStringLiteral("name"));
        QDomNode descNode = itemElement.namedItem(QStringLiteral("desc"));

        QString picName = itemElement.namedItem(QStringLiteral("picture")).toElement().text();
        QDomElement refNode = (const QDomElement&) itemElement.namedItem(QStringLiteral("references")).toElement();

        QString desc = i18n(descNode.toElement().text().toUtf8().constData());
        if (!picName.isEmpty()) {
            desc.prepend("[img]" + picName + "[/img][brclear][br]");
        }

        item->setName(i18n(nameNode.toElement().text().toUtf8().constData()));

        desc = desc.replace(QLatin1String("[b]"), QLatin1String("<b>"));
        desc = desc.replace(QLatin1String("[/b]"), QLatin1String("</b>"));
        desc = desc.replace(QLatin1String("[i]"), QLatin1String("<i>"));
        desc = desc.replace(QLatin1String("[/i]"), QLatin1String("</i>"));
        desc = desc.replace(QLatin1String("[sub]"), QLatin1String("<sub>"));
        desc = desc.replace(QLatin1String("[/sub]"), QLatin1String("</sub>"));
        desc = desc.replace(QLatin1String("[sup]"), QLatin1String("<sup>"));
        desc = desc.replace(QLatin1String("[/sup]"), QLatin1String("</sup>"));
        desc = desc.replace(QLatin1String("[br]"), QLatin1String("<br />"));
        desc = desc.replace(QLatin1String("[brclear]"), QLatin1String("<br clear=\"left\"/>"));
        item->setDesc(desc);

        refNodeList = refNode.elementsByTagName(QStringLiteral("refitem"));
        for (int it = 0; it < refNodeList.count(); ++it)
        {
            reflist << i18n(refNodeList.item(it).toElement().text().toUtf8().constData());
        }
        item->setRef(reflist);

        list.append(item);
    }

    return list;
}

void Glossary::addItem(GlossaryItem* item)
{
    m_itemlist.append(item);
}

QList<GlossaryItem*> Glossary::itemlist()const
{
    return m_itemlist;
}

void Glossary::clear()
{
    m_itemlist.clear();
}

QString Glossary::name()const
{
    return m_name;
}

void Glossary::setItemlist(QList<GlossaryItem*> list)
{
    m_itemlist = list;
}

QString Glossary::picturePath()const
{
    return m_picturepath;
}

QString Glossary::backgroundPicture()const
{
    return m_backgroundpicture;
}

GlossaryDialog::GlossaryDialog(QWidget *parent) : QDialog(parent), d(new Private(this))
{
    setWindowTitle(i18n("Glossary"));

    //this string will be used for all items. If a backgroundpicture should
    //be used call Glossary::setBackgroundPicture().
    d->m_htmlbasestring = QStringLiteral("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><body%1>");

    QWidget *main = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(main);
    setLayout(vbox);
    vbox->setMargin(0);

    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *lbl = new QLabel(main);
    lbl->setText(i18n("&Search:"));
    hbox->addWidget(lbl);

    d->m_search = new KTreeWidgetSearchLine(main);
    d->m_search->setObjectName(QStringLiteral("search-line"));
    lbl->setBuddy(lbl);
    hbox->addWidget(d->m_search);
    vbox->addLayout(hbox);
    setFocusProxy(d->m_search);

    QSplitter *vs = new QSplitter(main);
    vbox->addWidget(vs);

    d->m_glosstree = new QTreeWidget(vs);
    d->m_glosstree->setObjectName(QStringLiteral("treeview"));
    d->m_glosstree->setHeaderLabel(QStringLiteral("entries"));
    d->m_glosstree->header()->hide();
    d->m_glosstree->setRootIsDecorated(true);

    d->m_search->addTreeWidget(d->m_glosstree);

    d->m_htmlpart = new KHTMLPart(vs);

    connect(d->m_htmlpart->browserExtension(), SIGNAL(openUrlRequestDelayed(QUrl,KParts::OpenUrlArguments,KParts::BrowserArguments)),
            this, SLOT(displayItem(QUrl,KParts::OpenUrlArguments,KParts::BrowserArguments)));
    connect(d->m_glosstree, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this, SLOT(itemActivated(QTreeWidgetItem*,int)));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &GlossaryDialog::reject);
    buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
    vbox->addWidget(buttonBox);

    resize(800, 400);
}

GlossaryDialog::~GlossaryDialog()
{
    delete d;
}

void GlossaryDialog::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Return) {
        e->ignore();
    }
    QDialog::keyPressEvent(e);
}

void GlossaryDialog::Private::displayItem(const QUrl &url, const KParts::OpenUrlArguments &, const KParts::BrowserArguments &)
{
    // using the "path" part of a qurl as reference
    QString myurl = url.path().toLower();

    QTreeWidgetItemIterator it(m_glosstree);
    while (*it) {
        if ((*it)->type() == GlossaryTreeItemType && (*it)->text(0).toLower() == myurl) {
             // force the item to be selected
             m_glosstree->setCurrentItem(*it);
             // display its content
             itemActivated((*it), 0);
             break;
        } else {
            ++it;
        }
     }
}

void GlossaryDialog::Private::rebuildTree()
{
    m_glosstree->clear();

    QList< GlossaryInfo >::ConstIterator it = m_glossaries.constBegin();
    QList< GlossaryInfo >::ConstIterator itEnd = m_glossaries.constEnd();
    for (; it != itEnd; ++it) {
        m_glosstree->addTopLevelItem(createItem(*it));
    }
}

QTreeWidgetItem* GlossaryDialog::Private::createItem(const GlossaryInfo& gi) const
{
    Glossary *glossary = gi.glossary;
    bool folded = gi.folded;
    QTreeWidgetItem *main = new QTreeWidgetItem();
    main->setText(0, glossary->name());
    main->setFlags(Qt::ItemIsEnabled);
    foreach (GlossaryItem *item, glossary->itemlist()) {
        if (folded) {
            QChar thisletter = item->name().toUpper().at(0);
            QTreeWidgetItem *thisletteritem = findTreeWithLetter(thisletter, main);
            if (!thisletteritem) {
                thisletteritem = new QTreeWidgetItem(main);
                thisletteritem->setText(0, QString(thisletter));
                thisletteritem->setFlags(Qt::ItemIsEnabled);
                thisletteritem->setData(0, FirstLetterRole, thisletter);
            }
            thisletteritem->addChild(new GlossaryTreeItem(glossary, item));
        } else {
            main->addChild(new GlossaryTreeItem(glossary, item));
        }

    }
    return main;
}

void GlossaryDialog::addGlossary(Glossary* newgloss, bool folded)
{
    if (!newgloss || newgloss->isEmpty()) {
        return;
    }

    GlossaryInfo gi(newgloss);
    gi.folded = folded;
    d->m_glossaries.append(gi);

    d->m_glosstree->addTopLevelItem(d->createItem(gi));
    d->m_glosstree->sortItems(0, Qt::AscendingOrder);
}

QTreeWidgetItem* GlossaryDialog::Private::findTreeWithLetter(const QChar& l, QTreeWidgetItem* item) const
{
    int count = item->childCount();
    for (int i = 0; i < count; ++i) {
        QTreeWidgetItem *itemchild = item->child(i);
        if (itemchild->data(0, FirstLetterRole).toChar() == l) {
            return itemchild;
        }
    }
    return nullptr;
}

void GlossaryDialog::Private::itemActivated(QTreeWidgetItem * item, int column)
{
    Q_UNUSED(column)
    if (!item || item->type() != GlossaryTreeItemType) {
        return;
    }

    GlossaryTreeItem *glosstreeitem = static_cast<GlossaryTreeItem *>(item);
    GlossaryItem *glossitem = glosstreeitem->glossaryItem();
    QString html;
    QString bg_picture = glosstreeitem->glossary()->backgroundPicture();
    if (!bg_picture.isEmpty()) {
        html = " background=\"file://" + bg_picture + "\"";
    }

    html = m_htmlbasestring.arg(html);
    html += glossitem->toHtml() + "</body></html>";

    m_htmlpart->begin();
    m_htmlpart->write(html);
    m_htmlpart->end();
}

void GlossaryItem::setRef(const QStringList& s)
{
    m_ref = s;
    m_ref.sort();
}

QString GlossaryItem::toHtml() const
{
    QString code = "<h1>" + m_name + "</h1>" + m_desc + parseReferences();

    return code;
}

QString GlossaryItem::parseReferences() const
{
    if (m_ref.isEmpty()) {
        return QString();
    }

    QString htmlcode = "<h3>" + i18n("References") + "</h3><ul type=\"disc\">";
    static QString basehref = QStringLiteral("<li><a href=\"item:%1\" title=\"%2\">%3</a></li>");

    foreach (const QString& ref, m_ref) {
        htmlcode += basehref.arg(QUrl::toPercentEncoding(ref), i18n("Go to '%1'", ref), ref);
    }
    htmlcode += QLatin1String("</ul>");

    return htmlcode;
}

void GlossaryItem::setName(const QString& s)
{
    m_name = s;
}

void GlossaryItem::setDesc(const QString& s)
{
    m_desc = s;
}

void GlossaryItem::setPictures(const QString& s)
{
    m_pic = QStringList(s);
}

QString GlossaryItem::name() const
{
    return m_name;
}

QString GlossaryItem::desc() const
{
    return m_desc;
}

QStringList GlossaryItem::ref() const
{
    return m_ref;
}

QStringList GlossaryItem::pictures() const
{
    return m_pic;
}

#include "moc_kdeeduglossary.cpp"
