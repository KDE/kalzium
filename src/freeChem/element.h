// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * element.h 
 *
 * Copyright (C) 2002-2008 Jean Bréfort <jean.brefort@normalesup.org>
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

#ifndef GCU_ELEMENT_H
#define GCU_ELEMENT_H

#include <map>
#include <string>
#include <vector>
#include "chemistry.h"
#include "isotope.h"
#include "value.h"

/*!\file */ 
/*!\def GCU_ERROR
Defines a standard error value.
*/
#define GCU_ERROR (1 << (sizeof(int) - 1))

/*!\namespace gcu
\brief The Gnome Chemistry Utils C++ classes

The namespace used for all C++ classes provided by the Gnome Chemistry Utils base library.
*/
namespace gcu
{

class EltTable;

/*!\class Element gcu/element.h
	Represents a chemical element.
	This class has no public constructor or destructor. The instances are created by the framework
	from data in the elements.xml file and
	a user cannot create a new element.
*/
class Element
{
friend class EltTable;
private:
	/*!
	\param Z: the atomic number corresponding to the element
	\param Symbol: the atomic symbol of the element

	This constructor is private and cannot be used ouside of this class.
	*/
	Element (int Z, const char* Symbol);
	virtual ~Element ();

public:
	/*!
	@param Z: the atomic number of a chemical element.
	@return The chemical symbol of the element whose atomic number is Z or NULL if the element is unknown.
	*/
	static const char* Symbol (int Z);
	/*!
	@param Z: the atomic number of a chemical element.

	This static method is used to know on what side of the symbol of the element whose atomic number is Z
	attached hydrogens should be written.
	@return true if hydrogens should be written on the right and false when it should be written on the left side.
	*/
	static bool BestSide (int Z);
	/*!
	@param symbol: the symbol of a chemical element.
	@return The atomic number of the element whose chemical symbol is used as parameter or 0 if the element is unknown.
	*/
	static int Z (const char* symbol);
	/*!
	@param Z: the atomic number of a chemical element.
	@return a pointer to the Element whose atomic number is Z or NULL if the element is unknown.
	*/
	static Element* GetElement (int Z);
	/*!
	@param symbol: the symbol of a chemical element.
	@return a pointer to the Element whose symbol is used as parameter or NULL if the element is unknown.
	*/
	static Element* GetElement (const char* symbol);
	/*!
	@param radius: a pointer to a GcuAtomicRadius structure.
	
	Before calling this function, most fields in the GcuAtomicRadius structure must be filled:
	- Z: the atomic number, mandatory
	- type: the type of the radius searched
	- charge: the charge of the atom. 0 for all radii except ionic radii.
	- cn: the coordination number or -1 if not significant
	- spin: the spin state or GCU_N_A_SPIN if not significant
	- scale: the name of the scale (e.g. "Pauling") or NULL
	
	The programs searches a value corresponding to the fields having a non default value. If one is found
	the other fields are given the corresponding values f the first match before returning.
	
	@return true if a radius has been found and false if not.
	*/
	static bool GetRadius (GcuAtomicRadius* radius);
	/*!
	@param en: a pointer to a GcuElectronegativity structure.
	
	Before calling this function, the following fields in the GcuElectronegativity structure must be filled:
	- Z: the atomic number, mandatory
	- type: the gcu_radius_type, mandatory
	- charge: the charge of the atom, mandatory; must be 0 for non ionic radii
	and non null for ionic radii.
	- scale: the name of the scale (e.g. "Pauling") or NULL
	
	The programs searches an electronegativity value for the element in the scale if given. If one is found
	the value and the scale (if NULL on calling)  are given the corresponding values of the first match before returning.

	@return true if a match has been found and false if not.
	*/
	static bool GetElectronegativity (GcuElectronegativity* en);
	/*!
	@param Z: the atomic number of a chemical element.
	
	The value returned by this method might be too low in some cases and is only indicative. Instances of the Atom class
	accept any number of bonds. This behavior might change in future versions.
	@return the maximum number of bonds an atom of the element can be involved in.
	*/
	static unsigned GetMaxBonds (int Z);
	/*!
	Loads the atomic radii database.
	*/
	static void LoadRadii ();
	/*!
	Loads the atomic electronic properties database.
	*/
	static void LoadElectronicProps ();
	/*!
	Loads the isotopes database.
	*/
	static void LoadIsotopes ();
	/*!
	Loads the Blue Obelisk Database.
	*/
	static void LoadBODR ();
	/*!
	Loads all databases.
	*/
	static void LoadAllData ();

	/*!
	\return The atomic number of the chemical element.
	*/
	int GetZ () {return m_Z;}
	/*!
	\return The chemical symbol of the element.
	*/
	const char* GetSymbol () {return m_Symbol;}
	/*!
	\return The default valence of the element for some elements, mainly non metals. For others, the returned value is -1
	and should not be taken into account.
	*/
	char GetDefaultValence () {return m_DefaultValence;}
	/*!
	The value returned by this method might be too low in some cases and is only indicative. Instances of the Atom class
	accept any number of bonds. This behavior might change in future versions.
	@return the maximum number of bonds an atom of the element can be involved in.
	*/
	unsigned GetMaxBonds () {return m_MaxBonds;}
	/*!
	This static method is used to know on what side of the symbol of the element
	attached hydrogens should be written.
	@return true if hydrogens should be written on the right and false when it should be written on the left side.
	*/
	bool GetBestSide () {return m_BestSide;}
	/*!
	Retrieves the default color used for the element.
	@return an array of three double values for the red, green and blue components of the color.
	*/
	double* GetDefaultColor () {return m_DefaultColor;}
	/*!
	@return the name of the element in the current locale or in english if the current locale is not supported in the database.
	*/
	const char* GetName () {return name.c_str();}
	/*!
	@return a pointer to the array of pointers to GcuAtomicRadius structures for all known radii for the element.
	Last value in the array is NULL.
	*/
	const GcuAtomicRadius** GetRadii ();
	/*!
	@return a pointer to the array of pointers to GcuElectronegativity structures for all known electronegativities for the element.
	Last value in the array is NULL.
	*/
	const GcuElectronegativity** GetElectronegativities ();
	/*!
	@return the number of valence electrons of the neutral atom.
	*/
	unsigned GetValenceElectrons () {return m_nve;}
	/*!
	@return the number of valence electrons of the neutral atom,
	including d and f electrons.
	*/
	unsigned GetTotalValenceElectrons () {return m_tve;}
	/*!
	@return the maximume number of valence electrons of the neutral atom,
	including d and f electrons.
	*/
	unsigned GetMaxValenceElectrons () {return m_maxve;}
	/*!
	@return the atomic molar mass of the element.
	*/
	DimensionalValue const *GetWeight ();
	/*!
	@param natoms: atoms count.

	@return the isotopic pattern correponding to a fragment containing n atoms of the
	element.
	*/
	IsotopicPattern *GetIsotopicPattern (unsigned natoms);
	/*!
	@return the fundamental electronic configuration for the element. The
	returned string is formated as a pango markup, with electron numbers
	for each sublevel as superscript.
	*/
	std::string const& GetElectronicConfiguration () {return ElecConfig;}
	/*!
	@return the map of known names for the element indexed by language.
	*/
	std::map<std::string, std::string> const& GetNames () {return names;}
	/*!
	@param rank: the rank of the ionization.

	@return the requested ionization energy as a &GcuDimensionalValue.
	*/
	GcuDimensionalValue const *GetIonizationEnergy (unsigned rank = 1);
	/*!
	@param rank: the rank of the electron affinity.

	@return the requested electron affinity as a &GcuDimensionalValue.
	For most elements, only the first is known.
	*/
	GcuDimensionalValue const *GetElectronAffinity (unsigned rank = 1);
	/*!
	@param property_name: the name of the property as used in the Blue
	Obelisk Data Repository (without the "bo:" prefix).

	@return the requested property if known, or NULL.
	*/
	Value const *GetProperty (char const *property_name);
	/*!
	@param property_name: the name of the property as used in the Blue
	Obelisk Data Repository (without the "bo:" prefix).

	@return the requested string property if known as a reference or an empty string.
	*/
	std::string &GetStringProperty (char const *property_name);
	/*!
	@param property_name: the name of the property as used in the Blue
	Obelisk Data Repository (without the "bo:" prefix).

	@return the requested integer property if known, or G_MININT32.
	*/
	int GetIntegerProperty (char const *property_name);

	/*!
	@return true if the element is a metal, false otherwise.
	*/
	bool IsMetallic ();

private:
	unsigned char m_Z, m_nve, m_tve, m_maxve;
	char m_Symbol[4];
	DimensionalValue const *m_AtomicWeight;
	char m_DefaultValence;
	unsigned char m_MaxBonds;
	bool m_BestSide, m_Metallic, m_MetallicCached;
	double m_DefaultColor[3];
	std::string name;
	std::vector<GcuAtomicRadius*> m_radii;
	std::vector<GcuElectronegativity*> m_en;
	std::vector<Isotope*> m_isotopes;
	std::vector<IsotopicPattern*> m_patterns;
	std::vector<GcuDimensionalValue> m_ei;
	std::vector<GcuDimensionalValue> m_ae;
	std::map<std::string, std::string> names;
	std::map<std::string, Value*> props;
	std::map<std::string, std::string> sprops;
	std::map<std::string, int> iprops;
	std::string ElecConfig;
};

} // namespace gcu

#endif // GCU_ELEMENT_H
