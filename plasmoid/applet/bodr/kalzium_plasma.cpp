/***************************************************************************
    copyright            : (C) 2008 by Carsten Niehaus
    email                : cniehaus@kde.org
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kalzium_plasma.h"

#include <KConfigGroup>
#include <KFontDialog>

#include <QColor>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QVBoxLayout>

KalziumPlasma::KalziumPlasma(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_dialog(0),
    m_font(QFont())
{
    m_theme.setImagePath("widgets/testtube"),
    m_engine = dataEngine("kalzium");

    m_dialog = 0;
    m_label1 = 0;
    m_lineedit = new Plasma::LineEdit(this);
    m_lineedit->setDefaultText(i18n("Enter the atomic number."));
    connect(m_lineedit, SIGNAL(editingFinished()),
            this, SLOT(textChanged()));

    setHasConfigurationInterface(true);
    setAcceptDrops(false);
    setAcceptsHoverEvents(true);
//     setDrawStandardBackground(false);

    resize(385,177);
}

void KalziumPlasma::init()
{
    qDebug() << "initializing Kalzium";

    configChanged();

    m_currentSource = "BlueObelisk:RandomElement";
    // connect to random source and update ever 5 seconds
    m_engine->connectSource(m_currentSource, this, 5000);

    m_theme.setContainsMultipleImages(false);

    m_label1 = new QGraphicsTextItem(this);
    m_label1->setPos(m_theme.elementRect("name").topLeft());
    m_label1->setFont(m_font);
    m_label1->setDefaultTextColor(Qt::white);
}

void KalziumPlasma::configChanged()
{
    KConfigGroup cg = config();
    m_font = cg.readEntry("font",QFont());
}

void KalziumPlasma::constraintsUpdated(Plasma::Constraints constraints)
{
//     setDrawStandardBackground(false);
    prepareGeometryChange();
    if (constraints & Plasma::SizeConstraint) {
        //m_theme.resize(contentSize().toSize());
        m_theme.resize(size());
    }

    m_label1->setPos(m_theme.elementRect("canvas").topLeft());
}

KalziumPlasma::~KalziumPlasma()
{
    delete m_dialog;
}

void KalziumPlasma::dataUpdated(const QString& source, const Plasma::DataEngine::Data &data)
{
    qDebug() << "dataUpdated" << source;
    if (source != m_currentSource) {
        return;
    }

    QString bp = data["bp"].toString();
    QString mp = data["mp"].toString();
    QString mass = data["mass"].toString();
    QString symbol = data["symbol"].toString();
    QString name = data["name"].toString();
    QString text;
    text = QString(i18n("\nName: %1", name));
    text.append(QString(i18n("\nSymbol: %1", symbol)));
    text.append(QString(i18n("\nBoiling point: %1", bp)));
    text.append(QString(i18n("\nMelting point: %1", mp)));
    text.append(QString(i18n("\nMass: %1", mass)));
    if (m_label1)  {
//         m_label1->setAlignment(Qt::AlignLeft);
        m_label1->setPlainText(text);
    }
}

void KalziumPlasma::paintInterface(QPainter *p,
                       const QStyleOptionGraphicsItem *option,
                       const QRect &contentsRect)
{
    Q_UNUSED(option);

    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setRenderHint(QPainter::Antialiasing);

    // Now we draw the applet, starting with our svg
    //m_theme.resize((int)contentsRect.width(), (int)contentsRect.height());
    m_theme.resize(size());
    //m_theme.paint(p, (int)contentsRect.left(), (int)contentsRect.top());
    m_theme.paint(p, 0, 0);

    m_lineedit->setPos(0, 150);
}

void KalziumPlasma::showConfigurationInterface()
{
    if (m_dialog == 0) {
        m_dialog = new QDialog;
        m_dialog->setWindowIcon(QIcon::fromTheme("kalzium"));
        m_dialog->setWindowTitle(i18n("KalziumPlasma Configuration"));
        ui.setupUi(m_dialog->mainWidget());
        m_dialog->mainWidget()->layout()->setMargin(0);
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel|QDialogButtonBox::Apply);
        QWidget *mainWidget = new QWidget(this);
        QVBoxLayout *mainLayout = new QVBoxLayout;
        m_dialog->setLayout(mainLayout);
        mainLayout->addWidget(mainWidget);
        QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
        okButton->setDefault(true);
        okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
        m_dialog->connect(buttonBox, &QDialogButtonBox::accepted, this, &KalziumPlasma::accept);
        m_dialog->connect(buttonBox, &QDialogButtonBox::rejected, this, &KalziumPlasma::reject);
        mainLayout->addWidget(buttonBox);
        connect(okButton, &QPushButton::clicked,
                this, &KalziumPlasma::configAccepted);
        connect(buttonBox, &QDialogButtonBox::Apply::clicked,
                this, &KalziumPlasma::configAccepted);
        connect(ui.fontSelectButton, SIGNAL(clicked()),
                this, SLOT(showFontSelectDlg()));
    }

    m_dialog->show();
}

void KalziumPlasma::showFontSelectDlg()
{
    KFontDialog::getFont(m_font);
}

void KalziumPlasma::configAccepted()
{
    prepareGeometryChange();

    KConfigGroup cg = config();
    cg.writeEntry("font", m_font);
    m_label1->setFont(m_font);

    emit configNeedsSaving();
}

void KalziumPlasma::textChanged()
{
    m_engine->disconnectSource(m_currentSource, this);

    QString currentText = m_lineedit->toPlainText();
    if (!currentText.isEmpty()) {
        // simply assume the user entered the atomic number of the element
        m_currentSource = QString("BlueObelisk:Element:") + currentText;
    } else {
        m_currentSource = QString("BlueObelisk:RandomElement");
    }
    m_engine->connectSource(m_currentSource, this, 5000);
}

#include "kalzium_plasma.moc"
