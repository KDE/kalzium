/*
 * kmolcalc.cpp
 *
 * Copyright (C) 2000 Tomislav Gountchev <tomi@idiom.com>
 */

/**
 * KMOLCALC is the calculation engine. It knows about a hashtable of user defined atomic
 * weights and group definitions ELSTABLE, and the currently processed formula, stored
 * as a list of elements and their coefficients, ELEMENTS.
 */

#include <klocale.h>
#include <kstandarddirs.h>
#include "kmolcalc.h"


/**
 * Construct a new calculator object.
 */
KMolCalc::KMolCalc() {
  elements = new ElementList;
  elstable = NULL;
  readElstable();
}

KMolCalc::~KMolCalc() {
  delete elements;
}

void KMolCalc::readElstable() {
  weight = -1; // not calculated yet
  delete elstable;
  elstable = new QDict<SubUnit> (197, TRUE);
  elstable->setAutoDelete(TRUE);
  QStringList files = KGlobal::dirs()->findAllResources("appdata", "kmolweights");
  mwfile = locateLocal("data", "kmol")+"/kmolweights";
  QFile f(mwfile);
  QString* latest_f = &mwfile;
  for (uint i=0; i<files.count(); i++) {
  	if (QFileInfo(files[i]).lastModified() > QFileInfo(*latest_f).lastModified()) {
		latest_f = &files[i];
    }
  }
  QFile lf(*latest_f);
  if (f.exists()) readMwfile(f);
  if (!f.exists()) {
  	readMwfile(lf);
  	writeElstable();
  } else if (QFileInfo(f).lastModified() < QFileInfo(lf).lastModified()) {
    // announce
    QMessageBox::information
      (0, i18n("Warning"), i18n("Found new global Mw file.\nLocal definitions will be updated."), QMessageBox::Ok);
    readMwfile(lf);
    writeElstable();
  }

}


/**
 * Parse a string S and construct the ElementList this->ELEMENTS, representing the
 * composition of S. Returns 0 if successful, or an error code (currently -1) if
 * parsing failed.
 * The elements is S must be valid element or group symbols, as stored in this->ELSTABLE.
 * See help files for correct formula syntax.
 */
QString KMolCalc::readFormula(const QString& s) {
  weight = -1;
  delete elements;
  elements = new ElementList;
  return KMolCalc::readGroup(s, elements);
}

// read a formula group recursively. Called by readFormula.
QString KMolCalc::readGroup(const QString& s, ElementList* els) {
  if (s.isEmpty()) return i18n("Enter a formula."); //ERROR
  int sl = s.length();
  int i = 0;
  QString errors ("OK");
  bool ok = TRUE;
  while (i < sl && ((s[i] <= '9' && s[i] >= '0') || s[i] == '.')) i++;
  double prefix = (i == 0 ? 1 : s.left(i).toDouble(&ok));
  if (! ok || i == sl || prefix == 0) return i18n("Bad formula."); // ERROR
  ElementList* elstemp = new ElementList;
  while (i < sl) {
    int j = i;
    if (s[i] == '(') {
      ElementList* inner = new ElementList;
      int level = 1; // count levels of nested ( ).
      while (1) {
	if (i++ == sl) {
	  delete inner;
	  delete elstemp;
	  return i18n("Bad formula."); //ERROR
	}
	if (s[i] == '(') level++;
	if (s[i] == ')') level--;
	if (level == 0) break;
      }
      errors = KMolCalc::readGroup(s.mid(j+1, i-j-1), inner);
      j = ++i;
      while (i < sl && ((s[i] <= '9' && s[i] >= '0') || s[i] == '.')) i++;
      double suffix = (i == j ? 1 : s.mid(j, i-j).toDouble(&ok));
      if (! ok || suffix == 0) {
	delete inner;
	delete elstemp;
	return i18n("Bad formula."); // ERROR
      }
      inner->addTo(*elstemp, suffix);
      delete inner;
      inner = NULL;
    } else if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')) {
      while (++i < sl && ((s[i] >= 'a' && s[i] <= 'z') || s[i] == '*' ||
			  s[i] == '\''));
      QString elname = s.mid(j, i-j);
      j = i;
      while (i < sl && ((s[i] <= '9' && s[i] >= '0') || s[i] == '.')) i++;
      double suffix = (i == j ? 1 : s.mid(j, i-j).toDouble(&ok));
      if (! ok || suffix == 0) {
	delete elstemp;
	return i18n("Bad formula."); // ERROR
      }
      SubUnit* group = elstable->find(elname);
      if (group == 0) {
	delete elstemp;
	return i18n("Undefined symbol: ") + elname; //ERROR
      }
      group->addTo(*elstemp, suffix);
    } else if (s[i] == '+') {
      if (elstemp->isEmpty()) {
	delete elstemp;
	return i18n("Bad formula."); //ERROR
      }
      elstemp->addTo(*els, prefix);
      delete elstemp;
      errors = KMolCalc::readGroup(s.mid(i+1, sl-i-1), els);
      return errors;
    } else {
      delete elstemp;
      return i18n("Bad formula."); //ERROR
    }
  }
  elstemp->addTo(*els, prefix);
  delete elstemp;
  return errors;
}

/**
 * Calculate and return the molecular weight of the current chemical formula.
 */
double KMolCalc::getWeight() {
  if (weight == -1) weight = elements->getWeight(elstable);
  return weight;
}

/**
 * Return the elemental composition of the current formula, as a string of tab-separated
 * element - percentage pairs, separated by newlines.
 */
QString KMolCalc::getEA() {
  if (weight == -1) weight = elements->getWeight(elstable);
  if (weight == -1) return i18n("ERROR: Couldn't get Mw..."); // ERROR
  return elements->getEA(elstable, weight);
}

/**
 * Return the empirical formula of the current compound as a QString.
 */
QString KMolCalc::getEmpFormula() {
  return elements->getEmpFormula();
}

// Read the element definition file.
void KMolCalc::readMwfile(QFile& f) {
  if (! f.open(IO_ReadOnly)) return; //ERROR
  QTextStream fs (&f);
  QString line;
  while (! fs.eof()) {
    line = fs.readLine();
    SubUnit* s = SubUnit::makeSubUnit(line);
	elstable->replace(s->getName(), s);
  }
  f.close();
}

/**
 * Save the element definitions file.
 */
void KMolCalc::writeElstable() {
  QFile f(mwfile);
  if (! f.open(IO_WriteOnly)) return; //ERROR
  QTextStream fs (&f);
  QString line;
  QDictIterator<SubUnit> it(*elstable);
  while (it.current()) {
    it.current()->writeOut(line);
    fs << line << endl;
    ++it;
  }
  f.close();
}

/**
 * Remove a group or element definition from ELSTABLE.
 */
void KMolCalc::undefineGroup (const QString& name) {
  elstable->remove (name);
}

/**
 * Add a new element name - atomic weight record to the ELSTABLE hashtable. Assumes
 * NAME has valid syntax.

 */
void KMolCalc::defineElement (const QString& name, double weight) {
  Element* el = new Element(name, weight);
  elstable->replace(name, el);
}

/**
 * Add a new group definition to the ELSTABLE. Returns 0 if OK, -1 if parsing FORMULA
 * fails. Assumes the syntax of grpname is correct.
 */
QString KMolCalc::defineGroup (const QString& grpname, const QString& formula) {
  ElementList* els = new ElementList(grpname);
  QString error = readGroup(formula, els);
  if (error != "OK") return error;
  if (els->contains(grpname)) return QString("Can't define a group recursively!\n");
  elstable->replace(grpname, els);
  return QString("OK");
}
