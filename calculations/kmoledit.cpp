/*
 * kmoledit.cpp
 *
 * Copyright (C) 2000 Tomislav Gountchev <tomi@idiom.com>
 */

/**
 * The built in editor used to define / undefine groups and elements.
 */

#include <kglobalsettings.h>
#include <klocale.h>
#include <kpushbutton.h>

#include <qlayout.h>

#include "kmoledit.h"

KMolEdit::KMolEdit (QWidget * /*parent*/, const char* name, KMolCalc* kmolcalc) 
    : QDialog (0, name, TRUE) {

        QGridLayout *grid = new QGridLayout ( this, 4, 2 ,8, -1, "KMolEditLayout" );

        // Error messages:
        NAMES = i18n("Names must start with a letter,\nfollowed by any number of lowercase letters.");

        this->kmolcalc = kmolcalc;
        setCaption (i18n("KMol Group Definitions"));

        // formula entry field
        formula = new QLineEdit (this);
        formula->setFocus();
        grid->addMultiCellWidget (formula , 1,1,0,2);

        QLabel* formula_l = new QLabel (i18n("Symbol:"), this, "grouplabel");
        grid->addWidget(formula_l, 0 , 0 );

        // molecular weight or group definition entry field
        definition = new QLineEdit (this);
        grid->addMultiCellWidget (definition , 3,3,0,2);

        QLabel* definition_l = new QLabel (i18n("Formula or weight:"), this, "deflabel");
        definition_l->setFont(KGlobalSettings::generalFont());
        grid->addMultiCellWidget(definition_l, 2,2,0,2);

        KPushButton *define = new KPushButton (i18n ("Define"), this, "define");
        connect (define, SIGNAL(clicked()), this, SLOT(edit()));
        connect (definition, SIGNAL(returnPressed()), define, SLOT(animateClick()));
        grid->addWidget(define, 4,0);

        KPushButton* ok = new KPushButton (i18n ("Done"), this, "done");
        connect (ok, SIGNAL(clicked()), this, SLOT(save()));
        grid->addWidget(ok, 4,1);

        KPushButton* cancel = new KPushButton (i18n ("Cancel"), this, "cancel");
        cancel->setFont(KGlobalSettings::generalFont());
        connect (cancel, SIGNAL(clicked()), this, SLOT(undo()));
        grid->addWidget(cancel, 4,2);
    }
  
/**
 * Parse the user input from this->formula and this->definition. Check syntax,
 * check definition and call the corresponding kmolcalc function to define 
 * element or group. Returns an error message, "OK" if no errors.
 */
bool KMolEdit::edit() {
  QString name(formula->text());
  if (name.isEmpty()) return true; //QString("OK");
  if ((name[0] < 'A' || name[0] > 'Z') && (name[0] < 'a' || name[0] > 'z')) 
    { // Not a valid syntax
      error(NAMES);
      return false;//QString (i18n("Bad formula."));
    }
  for (uint i = 1; i < name.length(); i++)
    if ((name[i] < 'a' || name[i] > 'z') && name[i] != '*' && name[i] != '\'')
      {
	error(NAMES);
	return false; //QString (i18n("Bad formula."));
      }
  QString def(definition->text());
  if (def.isEmpty()) {
    kmolcalc->undefineGroup(name); // empty definition field used to erase records!!
    formula->setFocus();
    return true;//QString("OK");
  }
  bool ok = TRUE;
  double weight = def.toDouble(&ok);
  if (ok) { // element definition requested, weight entered
    if (weight <= 0) {
      QString err (i18n("Molecular weight must be positive."));
      error (err);
      return false;// err;
    }
    kmolcalc->defineElement(name, weight);
  }
  else { // group definition requested, formula entered
    QString err = kmolcalc->defineGroup(name, def); // try to parse and define formula
    if (err != "OK") {
      error (err);
      return false;//err;
    }
  }
  formula->setFocus();
  return true;//QString("OK");
}

// show error message
void KMolEdit::error(const QString& mess) {
  QMessageBox::information (0, i18n("Error"), mess, QMessageBox::Ok);
}

 
/** 
 * Save changes to definition file and close editor window.
 */
void KMolEdit::save() {
  if (!edit()) return;
  kmolcalc->writeElstable();
  kmolcalc->readElstable();
  done(0);
}

/**
 * Undo changes not yet saved by re-reading definition file.
 */
void KMolEdit::undo() {
  kmolcalc->readElstable();
  done(0);
}

#include "kmoledit.moc"
