/* 
 * Gnome Chemistry Utils
 * element.cc 
 *
 * Copyright (C) 2002-2007 Jean Bréfort <jean.brefort@normalesup.org>
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

// #include "config.h"
#include "element.h"
#include "xml-utils.h"
#include <kstandarddirs.h>
#include <cmath>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlmemory.h>
#include <clocale>
#include <cmath>
#include <libintl.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <QString>
#include <QByteArray>

using namespace std;

static set<string>units;

static void ReadValue (char const *source, GcuValue &value)
{
	const char *buf, *dot;
	value.value = strtod (source, const_cast <char**> (&buf));
	dot = strchr (source, '.');
	value.prec = (dot)? buf - dot - 1: 0;
	value.delta = (*buf == '(')? strtol (buf + 1, NULL, 10): 0;
}

static void ReadDimensionalValue (char const *source, GcuDimensionalValue &value)
{
	const char *buf, *dot;
	value.value = strtod (source, const_cast <char**> (&buf));
	dot = strchr (source, '.');
	value.prec = (dot)? buf - dot - 1: 0;
	value.delta = (*buf == '(')? strtol (buf + 1, NULL, 10): 0;
}

namespace gcu
{

class EltTable
{
public:
	EltTable();
	virtual ~EltTable();
	
	Element* operator [] (int Z);
	Element* operator [] (string Symbol);
	
	void AddElement(Element* Elt);

private:
	vector<Element*> Elements;
	map <string, Element*> EltsMap;
};

EltTable Table;

EltTable::EltTable()
{
//	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
//	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	xmlDocPtr xml;
	char* DefaultName;
	char *lang = getenv ("LANG");
	setlocale (LC_ALL, lang);
	char *old_num_locale, *buf, *num;
	unsigned char Z;
	map <string, string> Langs;
	Langs["de"] = ("German");
	Langs["fr"] = ("French");
	Langs["it"] = ("Italian");
	Langs["pl"] = ("Polish");
	Langs["pt_BR"] = ("Brazilian");
	Langs["ru"] = ("Russian");
	QString temp = KStandardDirs::locate( "data", "kalzium/data/gcu/elements.xml" );
	QByteArray ba = temp.toLatin1();
	if (!(xml = xmlParseFile ((ba.data())))) {
		cout << "Can't find and read elements.xml";
	}
//	old_num_locale = strdup (setlocale (LC_NUMERIC, NULL));
	setlocale (LC_NUMERIC, "C");
	xmlNode* node = xml->children, *child;
	if (strcmp ((const char*) node->name, "gpdata")) {
		cout << "Incorrect file format: elements.xml";
	}
	node = node->children;
	Element* Elt;
	while (node)
	{
		if (strcmp ((const char*) node->name, "text"))
		{
			if (strcmp ((const char*) node->name, "element")) {
				cout<<"Incorrect file format: elements.xml";
			}
			buf = (char*) xmlGetProp (node, (xmlChar*) "symbol");
			num = (char*) xmlGetProp (node, (xmlChar*) "Z");
			Elt = new Element (Z = atoi (num), buf);
			xmlFree (num);
			xmlFree (buf);
			num = (char*) xmlGetProp (node, (xmlChar*) "max_bonds");
			Elt->m_MaxBonds = atoi (num);
			xmlFree (num);
			num = (char*) xmlGetProp (node, (xmlChar*) "valence");
			if (num) {
				Elt->m_DefaultValence = atoi (num);
				xmlFree (num);
			} else
				Elt->m_DefaultValence = -1;
			child = node->children;
			DefaultName = NULL;
			while (child)
			{
				if (!strcmp ((const char*) child->name, "text")) {
					child = child->next;
					continue;
				}
				if (!strcmp((const char*)child->name, "name")) {
					buf = (char*) xmlNodeGetLang (child);
					if ((buf) && (lang)){
						string Lang = Langs[buf];
						char *Name = (char*) xmlNodeGetContent (child);
						if (Lang.length ())
							Elt->names[Lang] = Name;
						if (!strncmp (lang, buf, 2))
							Elt->name = Name;
						xmlFree (Name);
					} else if (!buf) {
						DefaultName = (char*) xmlNodeGetContent (child);
						Elt->names[("English")] = DefaultName;
					}
					xmlFree (buf);
				} else if (!strcmp ((const char*) child->name, "color")) {
					buf = (char*) xmlGetProp (child, (xmlChar*) "red");
					if (buf) {
						Elt->m_DefaultColor[0] = strtod (buf, NULL);
						xmlFree (buf);
					}
					buf = (char*) xmlGetProp (child, (xmlChar*) "green");
					if (buf) {
						Elt->m_DefaultColor[1] = strtod (buf, NULL);
						xmlFree (buf);
					}
					buf = (char*) xmlGetProp (child, (xmlChar*) "blue");
					if (buf) {
						Elt->m_DefaultColor[2] = strtod (buf, NULL);
						xmlFree (buf);
					}
				}				child = child->next;
			}
			if ((Elt->name.length () == 0) && DefaultName) Elt->name = DefaultName;
			if (DefaultName)
				xmlFree (DefaultName);
			AddElement (Elt);
		}
		node = node->next;
	}
	setlocale (LC_NUMERIC, old_num_locale);
	free (old_num_locale);
	xmlFreeDoc (xml);
}

EltTable::~EltTable()
{
	map<string, Element*>::iterator i;
	for (i = EltsMap.begin(); i != EltsMap.end(); i++)
		if ((*i).second) delete (*i).second;
	EltsMap.clear();
	Elements.clear();
}

Element* EltTable::operator[](int Z)
{
	return ((unsigned) Z < Elements.size ())? Elements[Z]: NULL;
}

Element* EltTable::operator[](string Symbol)
{
	return EltsMap[Symbol];
}

void EltTable::AddElement(Element* Elt)
{
	if ((unsigned) Elt->GetZ () >= Elements.size ())  {
		unsigned old_size = Elements.size ();
		Elements.resize (old_size + 10);
		for (unsigned i = old_size; i < old_size + 10; i++)
			Elements[i] = NULL;
	}
	Elements[Elt->GetZ ()] = Elt;
	EltsMap[Elt->GetSymbol ()] = Elt;
}

Element::Element(int Z, const char* Symbol):
	m_AtomicWeight (NULL),
	m_MetallicCached (false)
{
	m_Z = Z;
	strncpy(m_Symbol, Symbol, 3);
	m_Symbol[3] = 0;
	m_MaxBonds = 0;
	m_BestSide = true;
	switch (m_Z)
	{
		case 8:
		case 16:
		case 34:
		case 52:
		case 9:
		case 17:
		case 35:
		case 53:
			m_BestSide = false;
	}
	m_DefaultColor[0] = m_DefaultColor[1] = m_DefaultColor[2] = 0.0;
	if (m_Z <= 2) {
		m_nve = m_tve = m_Z;
		m_maxve = 2;
	} else if (m_Z <= 10) {
		m_nve = m_tve = m_Z - 2;
		m_maxve = 8;
	} else if (m_Z <= 18) {
		m_nve = m_tve = m_Z - 10;
		m_maxve = 8;
	} else if (m_Z <= 29) {
		m_nve = m_tve = m_Z - 18;
		m_maxve = 18;
	} else if (m_Z <= 36) {
		m_tve = m_Z - 18;
		m_nve = m_tve - 10;
		m_maxve = 18;
	} else if (m_Z <= 47) {
		m_nve = m_tve = m_Z - 36;
		m_maxve = 18;
	} else if (m_Z <= 54) {
		m_tve = m_Z - 36;
		m_nve = m_tve - 10;
		m_maxve = 18;
	} else if (m_Z <= 70) {
		m_nve = m_tve = m_Z - 54;
		m_maxve = 32;
	} else if (m_Z <= 79) {
		m_tve = m_Z - 54;
		m_nve = m_tve - 14;
		m_maxve = 32;
	} else if (m_Z <= 86) {
		m_tve = m_Z - 54;
		m_nve = m_tve - 24;
		m_maxve = 32;
	} else if (m_Z <= 102) {
		m_nve = m_tve = m_Z - 86;
		m_maxve = 32;
	} else if (m_Z <= 111) {
		m_tve = m_Z - 86;
		m_nve = m_tve - 14;
		m_maxve = 32;
	} else { // Assume m_Z <= 118
		m_tve = m_Z - 86;
		m_nve = m_tve - 24;
		m_maxve = 32;
	}
}

Element::~Element()
{
	while (!m_radii.empty()) {
		delete m_radii.back();
		m_radii.pop_back();
	}
	while (!m_en.empty()) {
		delete m_en.back();
		m_en.pop_back();
	}
	while (!m_isotopes.empty ()) {
		delete (m_isotopes.back ());
		m_isotopes.pop_back ();
	}
	map<string, Value*>::iterator i, iend = props.end ();
	for (i = props.begin (); i != iend; i++)
		delete (*i).second;
	props.clear ();
}

const char* Element::Symbol(int Z)
{
	Element* Elt = Table[Z];
	return (Elt)? Elt->GetSymbol(): NULL;
}

bool Element::BestSide(int Z)
{
	Element* Elt = Table[Z];
	return (Elt)? Elt->GetBestSide(): true;
}

int Element::Z(const char* symbol)
{
	Element* Elt = Table[symbol];
	return (Elt)? Elt->GetZ(): 0;
}

Element* Element::GetElement(int Z)
{
	return Table[Z];
}

Element* Element::GetElement(const char* symbol)
{
	return Table[symbol];
}

unsigned Element::GetMaxBonds(int Z)
{
	Element* Elt = Table[Z];
	return (Elt)? Elt->GetMaxBonds(): 0;
}

bool Element::GetRadius (GcuAtomicRadius* radius)
{
	Element* Elt = Table[radius->Z];
	if (!Elt || !Elt->m_radii.size ())
		return false;
	for (int i = 0; Elt->m_radii[i]; i++) {
		if (radius->type != Elt->m_radii[i]->type)
			continue;
		if (radius->charge != Elt->m_radii[i]->charge)
			continue;
		if ((radius->cn >= 0) &&(radius->cn != Elt->m_radii[i]->cn))
			continue;
		if ((radius->spin != GCU_N_A_SPIN) &&(radius->spin != Elt->m_radii[i]->spin))
			continue;
		if (!radius->scale) {
			*radius = *Elt->m_radii[i];
			return true;
		} else if (Elt->m_radii[i]->scale && !strcmp(radius->scale, Elt->m_radii[i]->scale)) {
			radius->value = Elt->m_radii[i]->value;
			return true;
		}
	}
	return false;
}

bool Element::GetElectronegativity(GcuElectronegativity* en)
{
	Element* Elt = Table[en->Z];
	if (!Elt) return false;
	if (!en->scale)
	{
		*en = *Elt->m_en[0];
		return true;
	}
	for (int i = 0; Elt->m_en[i]; i++)
		if (!strcmp(en->scale, Elt->m_en[i]->scale))
		{
			en->value = Elt->m_en[i]->value;
			return true;
		}

		return false;
}

const GcuAtomicRadius** Element::GetRadii()
{
	return (const GcuAtomicRadius**) &m_radii.front();
}

const GcuElectronegativity** Element::GetElectronegativities()
{
	return (const GcuElectronegativity**) &m_en.front();
}

void Element::LoadRadii ()
{
	xmlDocPtr xml;
	char *old_num_locale, *buf, *num;
	unsigned char Z;
	static bool loaded = false;
	if (loaded)
		return;
	LoadBODR ();
	QString temp = KStandardDirs::locate( "data", "kalzium/data/gcu/radii.xml" );
	QByteArray ba = temp.toLatin1();
	if (!(xml = xmlParseFile (ba.data()))) {
		cout<< ("Can't find and read radii.xml")<< endl;
	}
	old_num_locale = strdup (setlocale (LC_NUMERIC, NULL));
	setlocale (LC_NUMERIC, "C");
	xmlNode* node = xml->children, *child;
	if (strcmp ((const char*) node->name, "gpdata")) {
		cout<< ("Incorrect file format: radii.xml");
	}
	node = node->children;
	Element* Elt;
	set<string>::iterator it = units.find ("pm");
	if (it == units.end ()) {
		units.insert ("pm");
		it = units.find ("pm");
	}
	while (node) {
		if (strcmp ((const char*) node->name, "text"))
		{
			if (strcmp ((const char*) node->name, "element")) {
				cout<<("Incorrect file format: radii.xml")<<endl;
			}
			num = (char*) xmlGetProp (node, (xmlChar*) "Z");
			Elt = Table[Z = atoi (num)];
			if (!Elt) {
				node = node->next;
				continue;
			}
			child = node->children;
			while (child)
			{
				if (!strcmp ((const char*) child->name, "text")) {
					child = child->next;
					continue;
				}
				if (!strcmp ((const char*) child->name, "radius")) {
					GcuAtomicRadius* radius = new GcuAtomicRadius;
					radius->Z = Z;	//FIXME: is it really useful there?
					buf = (char*) xmlGetProp (child, (xmlChar*) "type");
					if (!buf ||
						((!((!strcmp (buf, "ionic")) && (radius->type = GCU_IONIC))) &&
						(!((!strcmp (buf, "metallic")) && (radius->type = GCU_METALLIC))) &&
						(!((!strcmp (buf, "atomic")) && ((radius->type = GCU_ATOMIC) || true))))) {
						//invalid radius
						delete radius;
						if (buf)
							xmlFree (buf);
						child = child->next;
						continue;
					}
					buf = (char*) xmlGetProp (child, (xmlChar*) "scale");
					if (buf) {
						radius->scale = strdup (buf);
						xmlFree (buf);
					} else
						radius->scale = NULL;
					buf = (char*) xmlGetProp (child, (xmlChar*) "charge");
					if (buf) {
						radius->charge = strtol (buf, NULL, 10);
						xmlFree (buf);
					} else
						radius->charge = 0;
					buf = (char*) xmlGetProp (child, (xmlChar*) "cn");
					if (buf) {
						radius->cn = strtol (buf, NULL, 10);
						xmlFree (buf);
					} else
						radius->cn = -1;
					buf = (char*) xmlGetProp (child, (xmlChar*)"spin");
					if ((!buf) ||
						((!((!strcmp (buf, "low")) && (radius->spin = GCU_LOW_SPIN))) &&
						(!((!strcmp (buf, "high")) && (radius->spin = GCU_HIGH_SPIN)))))
						radius->spin = GCU_N_A_SPIN;
					if (buf)
						xmlFree (buf);
					buf = (char*) xmlGetProp (child, (xmlChar*) "value");
					if (buf) {
						ReadDimensionalValue (buf, radius->value) ;
						radius->value.unit = (*it).c_str ();
						Elt->m_radii.push_back (radius);
						xmlFree (buf);
					} else
						delete radius;
				} else {
					cout<< ("Invalid radius node")<< endl;
				}
				child = child->next;
			}
			Elt->m_radii.push_back (NULL);
		}
		node = node->next;
	}
	setlocale (LC_NUMERIC, old_num_locale);
	free (old_num_locale);
	xmlFreeDoc (xml);
	loaded = true;
}

void Element::LoadElectronicProps ()
{
	xmlDocPtr xml;
	char *old_num_locale, *buf, *num, *dot, *end;
	unsigned char Z;
	unsigned i;
	static bool loaded = false;
	if (loaded)
		return;
	QString temp = KStandardDirs::locate( "data", "kalzium/data/gcu/elecprops.xml");
	QByteArray ba = temp.toLatin1();
	if (!(xml = xmlParseFile (ba.data()))) {
		cout<< ("Can't find and read elecprops.xml")<< endl;
	}
	old_num_locale = strdup (setlocale (LC_NUMERIC, NULL));
	setlocale (LC_NUMERIC, "C");
	xmlNode* node = xml->children, *child;
	if (strcmp ((const char*) node->name, "gpdata")) {
		cout<< ("Incorrect file format: elecprops.xml")<< endl;
	}
	node = node->children;
	Element* Elt;
	while (node) {
		if (strcmp ((const char*) node->name, "text")) {
			if (strcmp ((const char*) node->name, "element")) {
				cout<< ("Incorrect file format: elecprops.xml");
			}
			num = (char*) xmlGetProp (node, (xmlChar*) "Z");
			Elt = Table[Z = atoi (num)];
			child = node->children;
			while (child) {
				if (!strcmp ((const char*) child->name, "text")) {
					child = child->next;
					continue;
				}
				if (!strcmp ((const char*) child->name, "en")) {
					GcuElectronegativity* en = new GcuElectronegativity;
					en->Z = Z;	//FIXME: is it really useful there?
					buf = (char*) xmlGetProp (child, (xmlChar*) "scale");
					if (buf) {
						en->scale = GetStaticScale (buf);
						xmlFree (buf);
					} else
						en->scale = NULL;
					buf = (char*) xmlGetProp (child, (xmlChar*) "value");
					if (buf) {
						en->value.value = strtod (buf, &end);
						dot = strchr (buf, '.');
						en->value.prec = (dot)? end - dot - 1: 0;
						en->value.delta = 0; // we should use a generic parser
						Elt->m_en.push_back (en);
						xmlFree (buf);
					} else
						delete en;	//without a value, the structure is useless and is discarded
				} else if (!strcmp ((const char*) child->name, "config")) {
					buf = (char*) xmlNodeGetContent (child);
					char *cur = buf;
					bool nonvoid = false;
					if (buf[0] == '[') {
						Elt->ElecConfig.append (buf, 4);
						cur += 4;
						nonvoid = true;
					}
					while (cur && *cur) {
						if (nonvoid) {
							cur++;
							Elt->ElecConfig.append (" ");
						}
						Elt->ElecConfig.append (cur, 2);
						cur += 2;
						i = 1;
						while (cur[i] > ' ')
							i++;
						Elt->ElecConfig.append ("<sup>");
						Elt->ElecConfig.append (cur, i);
						Elt->ElecConfig.append ("</sup>");
						cur += i;
					}
					Elt->ElecConfig.append (" ");
					xmlFree (buf);
				} else if (!strcmp ((const char*) child->name, "ei")) {
					unsigned rank;
					buf = (char*) xmlGetProp (child, (xmlChar*) "rank");
					if (buf) {
						rank = strtol (buf, NULL, 10);
						xmlFree (buf);
					} else
						rank = 1;
					if ((i = Elt->m_ei.size ()) < rank) {
						Elt->m_ei.resize (rank);
						for (; i < rank; i++)
							Elt->m_ei[i].value = NAN;
					}
					rank--;
					buf = (char*) xmlGetProp (child, (xmlChar*) "value");
					if (buf) {
						ReadValue (buf, (GcuValue&)Elt->m_ei[rank]) ;
						xmlFree (buf);
					} else {
						//no need to read the unit
						Elt->m_ei[rank].value = NAN;
						break;
					}
					buf = (char*) xmlGetProp (child, (xmlChar*) "unit");
					if (buf) {
						string str(buf);
						set<string>::iterator it = units.find (str);
						if (it == units.end ()) {
							units.insert (str);
							it = units.find (str);
						}
						Elt->m_ei[rank].unit = (*it).c_str ();
						xmlFree (buf);
					} else
						Elt->m_ei[rank].unit = "MJ.mol<sup>-1</sup>";
				} else if (!strcmp ((const char*) child->name, "ae")) {
					unsigned rank;
					buf = (char*) xmlGetProp (child, (xmlChar*) "rank");
					if (buf) {
						rank = strtol (buf, NULL, 10);
						xmlFree (buf);
					} else
						rank = 1;
					if ((i = Elt->m_ae.size ()) < rank) {
						Elt->m_ae.resize (rank);
						for (; i < rank; i++)
							Elt->m_ae[i].value = NAN;
					}
					rank--;
					buf = (char*) xmlGetProp (child, (xmlChar*) "value");
					if (buf) {
						ReadValue (buf, (GcuValue&)Elt->m_ae[rank]) ;
						xmlFree (buf);
					} else {
						//no need to read the unit
						Elt->m_ae[rank].value = NAN;
						break;
					}
					buf = (char*) xmlGetProp (child, (xmlChar*) "unit");
					if (buf) {
						string str(buf);
						set<string>::iterator it = units.find (str);
						if (it == units.end ()) {
							units.insert (str);
							it = units.find (str);
						}
						Elt->m_ae[rank].unit = (*it).c_str ();
						xmlFree (buf);
					} else
						Elt->m_ae[rank].unit = "kJ.mol<sup>-1</sup>";
				} else {
					cout<< ("Invalid property node")<< endl;
				}
				child = child->next;
			}
			Elt->m_en.push_back (NULL);
		}
		node = node->next;
	}
	setlocale (LC_NUMERIC, old_num_locale);
	free (old_num_locale);
	xmlFreeDoc (xml);
	loaded = true;
}

void Element::LoadIsotopes ()
{
	xmlDocPtr xml;
	char *old_num_locale, *num;
	unsigned char Z;
	static bool loaded = false;
	if (loaded)
		return;
	QString temp = KStandardDirs::locate( "data", "kalzium/data/gcu/isotopes.xml");
	QByteArray ba = temp.toLatin1();
	if (!(xml = xmlParseFile (ba.data()))) {
		cout<< ("Can't find and read isotopes.xml")<< endl;
	}
	old_num_locale = strdup (setlocale (LC_NUMERIC, NULL));
	setlocale (LC_NUMERIC, "C");
	xmlNode* node = xml->children, *child;
	if (strcmp ((const char*) node->name, "gpdata")) {
		cout<< ("Incorrect file format: isotopes.xml")<< endl;
	}
	node = node->children;
	Element *Elt;
	Isotope *Is;
	int minA, maxA, niso;
	while (node) {
		if (strcmp ((const char*) node->name, "text")) {
			if (strcmp ((const char*) node->name, "element")) {
				cout<< ("Incorrect file format: isotopes.xml")<< endl;
			}
			minA = maxA = niso = 0;
			num = (char*) xmlGetProp (node, (xmlChar*) "Z");
			Elt = Table[Z = atoi (num)];
			xmlFree (num);
			if (Elt == NULL)	// This should not occur
				continue;
			child = node->children;
			while (child) {
				if (!strcmp ((const char*) child->name, "text")) {
					child = child->next;
					continue;
				}
				if (!strcmp ((const char*) child->name, "isotope")) {
					Is = new Isotope ();
					num = (char*) xmlGetProp (child, (xmlChar*) "A");
					if (num) {
						Is->A = strtol (num, NULL, 10);
						xmlFree (num);
					}
					num = (char*) xmlGetProp (child, (xmlChar*) "weight");
					if (num) {
						ReadValue (num, Is->mass);
						xmlFree (num);
					}
					num = (char*) xmlGetProp (child, (xmlChar*) "abundance");
					if (num) {
						ReadValue (num, Is->abundance);
						xmlFree (num);
						niso++;
						if (minA == 0)
							minA = maxA = Is->A;
						else {
							if (minA > Is->A)
								minA = Is->A;
							else if (maxA < Is->A)
								maxA = Is->A;
						}
					}
					Elt->m_isotopes.push_back (Is);
				}
				child = child->next;
			}
			if (minA > 0) {
				IsotopicPattern *pattern = new IsotopicPattern (minA, maxA);
				vector<Isotope*>::iterator i, iend = Elt->m_isotopes.end ();
				for (i = Elt->m_isotopes.begin (); i != iend; i++) {
					if ((*i)->abundance.value != 0.)
						pattern->SetValue ((*i)->A, (*i)->abundance.value);
				}
				pattern->Normalize ();
				niso = pattern->GetMonoNuclNb ();
				i = Elt->m_isotopes.begin ();
				while ((*i)->A != niso)
					i++;
				pattern->SetMonoMass (SimpleValue ((*i)->mass));
				Elt->m_patterns.push_back (pattern);
			}
		}
		node = node->next;
	}
	setlocale (LC_NUMERIC, old_num_locale);
	free (old_num_locale);
	xmlFreeDoc (xml);
	loaded = true;
}

void Element::LoadAllData ()
{
	LoadRadii ();
	LoadElectronicProps ();
	LoadIsotopes ();
	LoadBODR ();
}

IsotopicPattern *Element::GetIsotopicPattern (unsigned natoms)
{
	if (m_patterns.size () == 0)
		return NULL;
	IsotopicPattern *pat, *pattern, *result = NULL;
	if (natoms == 0)
		return NULL;
	unsigned i = 1;
	while ((natoms & 1) == 0) {
		natoms >>= 1;
		i++;
	}
	while (natoms) {
		if (i == 1) {
			result = m_patterns[0];
			result->Ref ();
		} else if (natoms & 1) {
			while (m_patterns.size () < i) {
				pat = m_patterns[m_patterns.size () - 1]->Square ();
				pattern = pat->Simplify ();
				pat->Unref ();
				m_patterns.push_back (pattern);
			}
			pattern = m_patterns[i - 1];
			if (result) {
				pat = result->Multiply (*pattern);
				result->Unref ();
				result = pat->Simplify ();
				pat->Unref ();
			} else {
				result = pattern;
				result->Ref ();
			}
		}
		natoms >>= 1;
		i++;
	}
	return result;
}

GcuDimensionalValue const *Element::GetIonizationEnergy (unsigned rank)
{
	return (rank <= m_ei.size ())? &m_ei[rank - 1]: NULL;
}

GcuDimensionalValue const *Element::GetElectronAffinity (unsigned rank)
{
	return (rank <= m_ae.size ())? &m_ae[rank - 1]: NULL;
}

void Element::LoadBODR ()
{
	static bool loaded = false;
	if (loaded)
		return;
	loaded = true;
	char *old_num_locale;
	old_num_locale = strdup (setlocale (LC_NUMERIC, NULL));
	setlocale (LC_NUMERIC, "C");
	QString temp = KStandardDirs::locate( "data", "kalzium/data/gcu/elements.xml");
	QByteArray ba = temp.toLatin1();
	xmlDocPtr xml = xmlParseFile (ba.data());
	if (!xml)
		return;
	set<string>::iterator it = units.find ("pm");
	if (it == units.end ()) {
		units.insert ("pm");
		it = units.find ("pm");
	}
	xmlNodePtr node = xml->children, child;
	char *buf = NULL, *unit;
	Element *elt;
	map <string, Value*> props;
	if (!strcmp ((char const*) node->name, "list")) {
		node = node->children;
		while (node) {
			if (!strcmp ((char const*) node->name, "atom")) {
				elt = NULL;
				child = node->children;
				while (child) {
					if (!strcmp ((char const*) child->name, "scalar")) {
						Value *val;
						buf = (char*) xmlGetProp (child, (xmlChar const*) "dataType");
						if (!strcmp (buf, "xsd:float")) {
							char *end, *dot;
							xmlFree (buf);
							buf = (char*) xmlNodeGetContent (child);
							double x = strtod (buf, &end);
							dot = strchr (buf, '.');
							int prec = (dot)? end - dot - 1: 0;
							unit = (char*) xmlGetProp (child, (xmlChar const*) "units");
							xmlFree (buf);
							buf = (char*) xmlGetProp (child, (xmlChar const*) "errorValue");
							int delta;
							if (buf)
								delta = strtol (buf, NULL, 10);
							else
								delta = 0;
							xmlFree (buf);
							if (unit) {
								DimensionalValue *v = new DimensionalValue ();
								if (!strcmp (unit, "units:atmass"))
									v->val.unit = "g.mol<sup>−1</sup>";
								else if (!strcmp (unit, "units:ev"))
									v->val.unit = "eV";
								else if (!strcmp (unit, "units:ang"))
									v->val.unit = "Å";
								else if (!strcmp (unit, "siUnits:kelvin"))
									v->val.unit = "K";
//								else if (!strcmp (unit, ""))
//									v->val.unit = "";
								else
									v->val.unit = "";
								xmlFree (unit);
								v->val.value = x;
								v->val.prec = prec;
								v->val.delta = delta;
								val = v;
							} else {
								SimpleValue *v = new SimpleValue ();
								v->val.value = x;
								v->val.prec = prec;
								v->val.delta = delta;
								val = v;
							}
							buf = (char*) xmlGetProp (child, (xmlChar const*) "dictRef");
							if (elt)
								elt->props[(strncmp (buf, "bo:", 3))? buf: buf + 3] = val;
							xmlFree (buf);
						} else if (!strcmp (buf, "xsd:String") || !strcmp (buf, "xsd:string")) {
							buf = (char*) xmlGetProp (child, (xmlChar const*) "dictRef");
							char *val = (char*) xmlNodeGetContent (child);
							if (elt)
								elt->sprops[(strncmp (buf, "bo:", 3))? buf: buf + 3] = val;
							xmlFree (buf);
							xmlFree (val);
						} else if (!strcmp (buf, "xsd:int") || !strcmp (buf, "xsd:Integer")) {
							xmlFree (buf);
							buf = (char*) xmlNodeGetContent (child);
							int val = strtol (buf, NULL, 10);
							xmlFree (buf);
							buf = (char*) xmlGetProp (child, (xmlChar const*) "dictRef");
							if (!strcmp (buf, "bo:atomicNumber"))
								elt = Table[val];
							else if (elt)
								elt->iprops[(strncmp (buf, "bo:", 3))? buf: buf + 3] = val;
							xmlFree (buf);
						} else if (!strcmp (buf, "xsd:date")) {
							// assumeing only the discovery year is useful for us
							xmlFree (buf);
							buf = (char*) xmlNodeGetContent (child);
							int val = strtol (buf, NULL, 10);
							xmlFree (buf);
							buf = (char*) xmlGetProp (child, (xmlChar const*) "dictRef");
							if (elt && !strcmp (buf, "bo:discoveryDate"))
								elt->iprops["discoveryDate"] = val;
							xmlFree (buf);
						}
					}
					if (props.size () > 0) {
						if (elt) {
							map <string, Value*>:: iterator i, iend = props.end ();
							for (i = props.begin (); i != iend; i++)
								elt->props[(*i).first] = (*i).second;
						}
						props.clear ();
					}
					child = child->next;
				}
				if (elt) {
					DimensionalValue const *v = dynamic_cast <DimensionalValue const *> (elt->GetProperty ("radiusVDW"));
					double r;
					if (v) {
						r = v->GetAsDouble ();
						GcuDimensionalValue const val = v->GetValue ();
						int prec = val.prec;
						if (!strcmp (val.unit, "Å")) {
							r *= 100.;
							prec = (prec > 2)? prec - 2: 0;
						} else if (!strcmp (val.unit, "pm"))
							r = -1.; // FIXME: allow other units if needed
						if (r > 0.) {
							GcuAtomicRadius* radius = new GcuAtomicRadius;
							radius->value.value = r;
							radius->value.prec = prec;
							radius->value.delta = val.delta;
							radius->value.unit = "pm";
							radius->Z = elt->GetZ ();
							radius->type = GCU_VAN_DER_WAALS;
							radius->charge = 0;
							radius->scale = NULL;
							radius->cn = -1;
							radius->spin = GCU_N_A_SPIN;
							elt->m_radii.push_back (radius);
						}
					}
					v = dynamic_cast <DimensionalValue const *> (elt->GetProperty ("radiusCovalent"));
					if (v) {
						r = v->GetAsDouble ();
						GcuDimensionalValue const val = v->GetValue ();
						int prec = val.prec;
						if (!strcmp (val.unit, "Å")) {
							r *= 100.;
							prec = (prec > 2)? prec - 2: 0;
						} else if (!strcmp (val.unit, "pm"))
							r = -1.; // FIXME: allow other units if needed
						if (r > 0.) {
							GcuAtomicRadius* radius = new GcuAtomicRadius;
							radius->value.value = r;
							radius->value.prec = prec;
							radius->value.delta = val.delta;
							radius->value.unit = "pm";
							radius->Z = elt->GetZ ();
							radius->type = GCU_COVALENT;
							radius->charge = 0;
							radius->scale = NULL;
							radius->cn = -1;
							radius->spin = GCU_N_A_SPIN;
							elt->m_radii.push_back (radius);
						}
					}
				}
			}
			node = node->next;
		}
	}
	xmlFreeDoc (xml);
	setlocale (LC_NUMERIC, old_num_locale);
	free (old_num_locale);
}

Value const *Element::GetProperty (char const *property_name)
{
	map<string, Value*>::iterator i = props.find (property_name);
	return (i == props.end ())? NULL: (*i).second;
}

int Element::GetIntegerProperty (char const *property_name)
{
	map<string, int>::iterator i = iprops.find (property_name);
	return (i == iprops.end ())? GCU_ERROR: (*i).second;
}

string &Element::GetStringProperty (char const *property_name)
{
	static string empty_string;
	map<string, string>::iterator i = sprops.find (property_name);
	return (i == sprops.end ())? empty_string: (*i).second;
}

DimensionalValue const *Element::GetWeight ()
{
	if (m_AtomicWeight == NULL)
		m_AtomicWeight = dynamic_cast<DimensionalValue const*> (props["mass"]);
	return m_AtomicWeight;
}

bool Element::IsMetallic ()
{
	if (!m_MetallicCached) {
		LoadRadii ();
		GcuAtomicRadius r;
		r.Z = m_Z;
		r.type = GCU_METALLIC;
		r.charge = 0;
		r.spin = GCU_N_A_SPIN;
		r.cn = -1;
		r.scale = NULL;
		m_Metallic = GetRadius (&r) || (m_Z > 99 && m_Z < 118);
		m_MetallicCached = true;
	}
	return m_Metallic;
}

} // namespace gcu
