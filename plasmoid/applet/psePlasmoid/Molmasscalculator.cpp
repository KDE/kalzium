/***********************************************************************************
* Mass calculator: Plasmoid to calculate mass of a Molecule.
* Copyright (C) 2009, 2010 Etienne Rebetez, etienne.rebetez@oberwallis.ch
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
***********************************************************************************/
// own
#include "Molmasscalculator.h"

// Qt
#include <QApplication>
#include <QClipboard>
#include <QGraphicsLinearLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

// KDE
#include <KLocalizedString>
#include <KConfigDialog>
#include <KConfigGroup>

// Plasma
#include <Plasma/Theme>

// local
#include <psetables.h>

Molmasscalculator::Molmasscalculator(QObject *parent, const QVariantList &args)
        : Plasma::PopupApplet(parent, args),
        m_widget(0),
        m_PeriodWidget(0),
        m_lineedit(0),
        m_MassLabel(0),
        m_switchButton(0)
{
    // Some Applet settings
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAssociatedApplication("kalzium");

    m_triggerTimer = new QTimer();
    m_triggerTimer->setSingleShot(true);
    m_triggerTimer->setInterval(700);

    connect(m_triggerTimer, &QTimer::timeout, this, &Molmasscalculator::ParseMolecule);

    // Needed for the Popupapplet
    setPopupIcon("kalzium");
}

Molmasscalculator::~Molmasscalculator()
{
    if (!hasFailedToLaunch()) {
        saveConfig();
    }

    delete m_switchButton;
    delete m_MassLabel;
    delete m_lineedit;
    delete m_PeriodWidget;
    delete m_widget;
    delete m_triggerTimer;
}

void Molmasscalculator::init()
{
    // checking if the Dataengine is available
    if (!dataEngine("kalzium")->isValid()) {
        setFailedToLaunch(true, i18n("Dataengine \"kalzium\" not found"));
    }

    // loads the configuration
    configChanged();

    // Create the Periodic Table
    m_PeriodWidget = new PeriodicGrid(config().readEntry("tableType", 0), this);

    graphicsWidget();

    // Calculate the demo molecule.
    ParseMolecule();

    //set sizes
    managePeriodSystem();
}

void Molmasscalculator::appendElement(QString elementSymbol)
{
    QString molecule;
    molecule = m_lineedit->text();
    molecule.append(elementSymbol);

    ParseMolecule(molecule);
}

void Molmasscalculator::ParseMolecule(QString strInput)
{
    if (!strInput.isEmpty()) {
        m_molecule = dataEngine("kalzium")->query(QString("Molecule:Parser:") + strInput);
        newCalculatedMass();
    }
}

void Molmasscalculator::ParseMolecule()
{
    ParseMolecule(m_lineedit->text());
}

void Molmasscalculator::newCalculatedMass()
{
    if (m_molecule["molMass"].toString().isEmpty()) {
        m_MassLabel->setText(i18n("Invalid Molecule"));
        return;
    }

    //Set new MassLabel Text
    m_MassLabel->setText(QString::number(m_molecule["molMass"].toDouble(), 'g', 6) + " u");

    //Sets the niceMolecule string in the Lineedit //Configuration Option?
    m_lineedit->setText(m_molecule["niceMolecule"].toString());

    //Copy new Mass to Clipboard
    if (m_copyToClipboard) {
        QApplication::clipboard()->setText(m_molecule["molMass"].toString());
    }
}

QGraphicsWidget *Molmasscalculator::graphicsWidget()
{
    if (m_widget) {
        return m_widget;
    }

    m_widget = new QGraphicsWidget(this);

    QGraphicsLinearLayout *MainLayout = new QGraphicsLinearLayout(Qt::Vertical , m_widget);
    QGraphicsLinearLayout *TopLayout = new QGraphicsLinearLayout(Qt::Horizontal, MainLayout);

    Plasma::Label *MoleculeLabel = new Plasma::Label;
    MoleculeLabel->setText(i18n("Molecule:"));

    m_MassLabel = new Plasma::Label;
    m_MassLabel->setAlignment(Qt::AlignCenter);

    QString css("font-size:18px; color:" + this->palette().text().color().name() + ';');
    m_MassLabel->setStyleSheet(css);

    m_lineedit = new Plasma::LineEdit();
    m_lineedit->setClearButtonEnabled(true);
    m_lineedit->setMinimumWidth(100);
    m_lineedit->setText(i18n("C2H5OH"));
    connect(m_lineedit, &Plasma::LineEdit::textEdited, m_triggerTimer, &QTimer::start);

    m_switchButton = new Plasma::IconWidget();
    connect(m_switchButton, &Plasma::IconWidget::clicked, this, &graphicsWidget::toggleTable);

    TopLayout->addItem(MoleculeLabel);
    TopLayout->addItem(m_lineedit);
    TopLayout->addItem(m_switchButton);
    TopLayout->setSpacing(0);
    TopLayout->setContentsMargins(0,0,0,0);

    MainLayout->addItem(TopLayout);
    MainLayout->addItem(m_MassLabel);
    MainLayout->addItem(m_PeriodWidget);
    MainLayout->setSpacing(2);

    return m_widget;
}

void Molmasscalculator::toggleTable()
{
    if (m_showPeriodicTable) {
        m_showPeriodicTable = false;
    } else {
        m_showPeriodicTable = true;
    }
    managePeriodSystem();
}

void Molmasscalculator::managePeriodSystem()
{
    QString iconName;
    KIconLoader iconLoader;
    int x, y;

    if (m_showPeriodicTable) {
        iconName = "arrow-down";
        m_PeriodWidget->show();
        x = pseTables::instance()->getTabletype(m_PeriodWidget->getCurrentPseTyp())->tableSize().x() * 33;
        y = pseTables::instance()->getTabletype(m_PeriodWidget->getCurrentPseTyp())->tableSize().y() * 34;
    } else {
        iconName = "arrow-right";
        m_PeriodWidget->hide();
        x = 300;
        y = 60;
    }

    m_switchButton->setIcon(iconLoader.loadIcon(iconName, KIconLoader::Small));

    m_widget->setMinimumSize(x, y);
    m_widget->setPreferredSize(x, y);
    m_widget->resize(x, y);
    resize(x, y);
}

void Molmasscalculator::createConfigurationInterface(KConfigDialog* parent)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Apply);
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    parent->setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    parent->connect(buttonBox, &QDialogButtonBox::accepted, this, &createConfigurationInterface::accept);
    parent->connect(buttonBox, &QDialogButtonBox::rejected, this, &createConfigurationInterface::reject);
    mainLayout->addWidget(buttonBox);

    QWidget *widget = new QWidget(parent);

    m_ui.setupUi(widget);

    parent->addPage(widget, i18n("General"), "kalzium");

    m_ui.showPeriodic->setChecked(m_showPeriodicTable);
    m_ui.copyToCliboard->setChecked(m_copyToClipboard);

    foreach (const QString &thisTable, pseTables::instance()->tables()) {
        m_ui.tabletyp->addItem(thisTable);
    }

    m_ui.tabletyp->setCurrentIndex(m_PeriodWidget->getCurrentPseTyp());

    connect(parent->okButton, &QPushButton::clicked,
            this, &Molmasscalculator::configAccepted);
    connect(parent->buttonBox, &QDialogButtonBox::Apply::clicked,
                this, &Molmasscalculator::configAccepted);

    connect(m_ui.showPeriodic, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
    connect(m_ui.copyToCliboard, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
    connect(m_ui.tabletyp, SIGNAL(currentIndexChanged(QString)), parent, SLOT(settingsModified()));
}

void Molmasscalculator::configAccepted()
{
    if (m_ui.showPeriodic->isChecked() != m_showPeriodicTable) {
        m_showPeriodicTable = m_ui.showPeriodic->isChecked();
        managePeriodSystem();
    }

    if (m_ui.copyToCliboard->checkState() != m_copyToClipboard) {
        m_copyToClipboard = m_ui.copyToCliboard->checkState();
    }

    if (m_ui.tabletyp->currentIndex() != m_PeriodWidget->getCurrentPseTyp()) {
        m_PeriodWidget->setCurrentPseTyp(m_ui.tabletyp->currentIndex());
        managePeriodSystem();
    }
    saveConfig();
}

void Molmasscalculator::saveConfig()
{
    config().writeEntry("showPeriodicTable",m_showPeriodicTable);
    config().writeEntry("copyToClipboard",m_copyToClipboard);
    config().writeEntry("tableType",m_PeriodWidget->getCurrentPseTyp());
}

void Molmasscalculator::configChanged()
{
    m_showPeriodicTable = config().readEntry("showPeriodicTable", true);
    m_copyToClipboard = config().readEntry("copyToClipboard", false);
}

#include "Molmasscalculator.moc"
