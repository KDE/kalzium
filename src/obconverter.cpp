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

#include <QLabel>
#include <QString>

#include <kdebug.h>
#include "config-kalzium.h"

#include "obconverter.h"

#include "ui_obconverterwidget.h"

// OpenBabel includes
#include <openbabel/obconversion.h>

// Qt includes
#include <QLayout>
#include <QRegExp>
#include <QGroupBox>
#include <QListWidget>

// KDE includes
#include <klocale.h>
#include <kaction.h>
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
 
    connect(ui.selectToggleFileButton, 
            SIGNAL( clicked() ), SLOT( slotSelectToggle() ));
 
    connect(ui.convertFileButton, 
            SIGNAL( clicked() ), SLOT( slotConvert() ));
 
    connect(ui.FileListView, 
            SIGNAL( executed() ), SLOT(slotGuessInput()));
}

void OBConverter::addFile( const QString &filename )
{
//X     new KListViewItem(FileListView, filename);
}

void OBConverter::slotNew()
{
    ui.FileListView->clear();
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
    QString iformat=ui.InputTypeComboBox->currentText();
    QString oformat=ui.OutputTypeComboBox->currentText();
    iformat=iformat.remove(QRegExp(" --.*"));
    oformat=oformat.remove(QRegExp(" --.*"));
//    QRegExp isuffix="."+iformat+"$";
    QRegExp isuffix;
    QList <QListWidgetItem*> p = ui.FileListView->selectedItems ();
 
    if (p.count()==0){
        KMessageBox::sorry(0,i18n("You must select some files first."));
        return;
    }

    QStringList cmdList;
    {
        QString ifname;
        QString ofname = ifname;
        ofname = ofname.remove(isuffix)+"."+oformat;
        struct stat results;
        bool proceed=true;
        if (true){//stat(ofname, &results) == 0){//toLatin?
            //something named ofname already exists
            switch( KMessageBox::questionYesNo(
                        0, 
                        i18n( "The file %1 already exist. Do you want to overwrite if possible?").arg(ofname), 
                        i18n("Authorization")
                        )
                  )
            {
                case KMessageBox::No:
                    proceed=false;
                    break;
                default:
                    break;
            }
        }
        if (proceed){
            cmdList.append(QString("babel -i%1 %2 -o%3 %4").arg(iformat,ifname,oformat,ofname));
        }
        }
        switch(KMessageBox::questionYesNoList
                (
                 0,
                 i18n("OK to run these commands?"),
                 cmdList,
                 i18n("List of commands")))
        {
            case KMessageBox::Yes:
                for(QStringList::Iterator it = cmdList.begin(); 
                        it != cmdList.end(); 
                        ++it) {
                    QString cmd = *it;
//X                     if (fork() == 0)
//X                     {
//X                         execlp("/bin/sh",
//X                                 "/bin/sh",
//X                                 "-c",
//X                                 cmd.latin1(), 
//X                                 (char *) 0);
//X                         fprintf(stderr,"exec failed\n");
//X                     }
//X                     else { /* parent */ }
                }
                break;
            default:
                break;
        }
    }

/////////////////////////////////

OBSupportedFormat::OBSupportedFormat()
{
    OBConversion conv;
    Formatpos pos;
    OBFormat* pFormat;
    const char* str=NULL;
    while( OBConversion::GetNextFormat(pos, str, pFormat) )
    {
        if(( pFormat->Flags() & NOTWRITABLE) && (pFormat->Flags() & NOTREADABLE) )
        {
            continue;
        }
//X         QString Description = pFormat->Description();
//X         Description.truncate(Description.find("\n"));
//X         char buf[20];
//X         snprintf(buf,sizeof(buf),"%s -- ",str);
//X         Description.prepend(buf);
//X         if ( !(pFormat->Flags() & NOTREADABLE) )
//X         {
//X             InputFormat.append(Description);
//X         }
//X         if ( !(pFormat->Flags() & NOTWRITABLE) )
//X         {
//X             OutputFormat.append(Description);
//X         }
    }
}

OBSupportedFormat::~OBSupportedFormat()
{
}

QStringList OBSupportedFormat::getInputFormat()
{
    return( InputFormat );
}

QStringList OBSupportedFormat::getOutputFormat()
{
    return( OutputFormat );
}

#include "obconverter.moc"

