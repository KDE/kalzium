/*
 * kmoledit.h
 *
 * Copyright (C) 2000 Tomislav Gountchev <tomi@idiom.com>
 */

#ifndef KMOLEDIT_H
#define KMOLEDIT_H


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapplication.h>
#include <qstring.h>
#include <qdict.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include "kmolelements.h"
#include "kmolcalc.h"

/**
 * The built in editor used to define / undefine groups and elements.
 */
class KMolEdit : public QDialog {

  Q_OBJECT

public:
  KMolEdit (QWidget* parent, const char* name, KMolCalc* kmolcalc);
  

private slots:
  
/**
 * Parse the user input from this->formula and this->definition. Check syntax,
 * check definition and call the corresponding kmolcalc function to define element
 * or group.
 */
 bool edit();
 
/** 
 * Save changes to definition file and close editor window.
 */
 void save();

/**
 * Undo changes not yet saved by re-reading definition file.
 */
 void undo();

 private:

 void error(const QString& mess);

  KMolCalc* kmolcalc;
  QLineEdit *formula, *definition;
  QString NAMES;

};


#endif

