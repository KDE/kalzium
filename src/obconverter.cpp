/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus <cniehaus@kde.org>
 *   Copyright (C) 2006 by Jerome Pansanel                                 *
 *   j.pansanel@pansanel.net
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/


#include "obconverter.h"

// OpenBabel includes
#include <openbabel/obconversion.h>

// Qt includes
#include <QRegExp>
#include <QListWidget>
#include <QProcess>
#include <QString>


// KDE includes
#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>
#include <kpushbutton.h>
#include <kfiledialog.h>
#include <kurl.h>
#include <kmessagebox.h>

#include <unistd.h>
#include <sys/stat.h>

using namespace std;
using namespace OpenBabel;

OBConverter::OBConverter( QWidget *parent )
    : KDialog( parent )
{
    setCaption( i18n( "OpenBabel Frontend" ) );
    setButtons( Help | Close );
    setDefaultButton( None );

    ui.setupUi( mainWidget() );
    
    setupWindow();
}

void OBConverter::setupWindow()
{
    // Supported format object 
    OBSupportedFormat *supportedFormat = new OBSupportedFormat();

    // Add the supported fileformats to the GUI
    QStringList InputType = supportedFormat->getInputFormat();
    ui.InputTypeComboBox->addItems( InputType );

    QStringList OutputType = supportedFormat->getOutputFormat();
    ui.OutputTypeComboBox->addItems( OutputType );

    // Create connection
    connect(ui.addFileButton, 
            SIGNAL( clicked() ), SLOT( slotAddFile() ));
 
    connect(ui.deleteFileButton, 
            SIGNAL( clicked() ), SLOT( slotDeleteFile() ));
 
    connect(ui.selectAllFileButton, 
            SIGNAL( clicked() ), SLOT( slotSelectAll() ));
 
    connect(ui.convertFileButton, 
            SIGNAL( clicked() ), SLOT( slotConvert() ));
 
    connect(ui.FileListView, 
            SIGNAL( executed() ), SLOT(slotGuessInput()));
}

void OBConverter::slotAddFile()
{
    KUrl::List fl = KFileDialog::getOpenUrls( );

    foreach ( const KUrl& u , fl ) {
        new QListWidgetItem( u.fileName(), ui.FileListView);
    }
}


void OBConverter::slotSelectAll()
{
    ui.FileListView->selectAll();
}


void OBConverter::slotDeleteFile()
{
    QList<QListWidgetItem*> p = ui.FileListView->selectedItems();
    foreach ( QListWidgetItem *item, p) {
        delete item;
    }
}

void OBConverter::slotGuessInput()
{
    QList <QListWidgetItem*> p = ui.FileListView->selectedItems ();
    bool first=true;
    QString suffix;
    if( p.count() )
    {
        foreach (QListWidgetItem * item , p ) {
            if (first){
                first=false;
                suffix=item->text().remove(QRegExp("^.*\\."));
            } else {
                if (item->text().remove(QRegExp("^.*\\.")) == suffix) continue;
                else return;
            }
        }
        foreach (QListWidgetItem * item , p ) {
            if (first){
                first=false;
                suffix=item->text().remove(QRegExp("^.*\\."));
            } else {
                if (item->text().remove(QRegExp("^.*\\.")) == suffix) continue;
                else return;
            }

//X //assertion: 'suffix' is the only suffix among the selected files.
//X for(int i=0; i<InputTypeComboBox->count();i++){
//X     if (InputTypeComboBox->text(i).find(QRegExp("^"+suffix+" -- "))>=0){
//X         InputTypeComboBox->setCurrentItem(i);
//X         break;
//X     }
//X }
        }
    }
}

void OBConverter::slotConvert()
{
}

OBSupportedFormat::OBSupportedFormat()
{
    OBConversion conv;
    Formatpos pos;
    OBFormat* pFormat = NULL;
    const char* str = NULL;

    while( OBConversion::GetNextFormat(pos, str, pFormat) )
    {
        if(( pFormat->Flags() & NOTWRITABLE) && (pFormat->Flags() & NOTREADABLE) ){
            continue;
        }
        QString Description = pFormat->Description();
        Description.truncate(Description.indexOf("\n"));
        char buf[20];
        snprintf(buf,sizeof(buf),"%s -- ",str);
        Description.prepend(buf);
        if ( !(pFormat->Flags() & NOTREADABLE) ){
            m_InputFormat.append(Description);
        }
        if ( !(pFormat->Flags() & NOTWRITABLE) ){
            m_OutputFormat.append(Description);
        }
    }
}

OBSupportedFormat::~OBSupportedFormat()
{
}

QStringList OBSupportedFormat::getInputFormat() const
{
    return m_InputFormat;
}

QStringList OBSupportedFormat::getOutputFormat() const
{
    return m_OutputFormat;
}

#include "obconverter.moc"

