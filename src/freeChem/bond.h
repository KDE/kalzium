// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * bond.h 
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

#ifndef GCU_BOND_H
#define GCU_BOND_H

#include <list>
#include "object.h"

/*!\file*/
namespace gcu
{

class Atom;
class Cycle;

/*!\class Bond gcu/bond.h
This class is used to represent bonds.

*/
class Bond: public Object
{
public:
/*!
The default constructor. Creates a bond with order set to 0.
*/
	Bond ();
/*!
@param first a pointer to the first Atom instance of the bond.
@param last a pointer to the last Atom instance of the bond.
@param order the order of the new bond.

The distinction between first and last is important mostly for stereobonds. The Bond instance is created
and adds itself to the two atoms calling Atom::AddBond.
*/
	Bond (Atom* first, Atom* last, unsigned char order);
/*!
The destructor of Bond.
*/
	virtual ~Bond ();
	
/*!
@param which the index of the Atom instance to be returned.

@return a pointer to the Atom instance: first Atom if which is 0, last if which is 1 or NULL.
*/
	virtual Atom* GetAtom (int which) const; //0 = first, 1 = last, others reserved (for multicentered bonds?)
/*!
@param pAtom a pointer to an Atom instance.
@param which an index which might be used for multicentered bonds (not supported); the default value should
always be used even if the implementation does not use it in this version.
@return the last atom if pAtom is the first, the first if pAtom is the last or NULL if pAtom is not concerned by this Bond.
*/
	virtual Atom* GetAtom (Atom* pAtom, int which = 0) const;	//"which" is just a place holder for multicenter bonds; returns an atom different from pAtom
															//i.e. the other end of the bond
/*!
@return the Bond order.
*/
	unsigned char GetOrder () const;
/*!
@param Order the new bond order.

The value used is not checked. Should be a significant value (1 to 3, exceptionally 4).
*/
	void SetOrder (unsigned char Order);
/*!
@param xml the xmlDoc used to save the document.
@return a pointer to the xmlNode representing this Bond or NULL if an error occured.
*/
	virtual xmlNodePtr Save (xmlDocPtr xml) const;
/*!
@param node a pointer to the xmlNode containing the serialized Bond.

Loads a bond from an xmlNode.
*/
	virtual bool Load (xmlNodePtr node);
/*!
@param n the increment to add to the bond order

If the resulting bond order is greater than 4, the remainder of the division by 4 is retained.
*/
	virtual void IncOrder (int n = 1);
/*!
@param node a pointer to the xmlNode containing the serialized Bond.

This virtual method is called at the end of the Bond::Load method. The default behavior is to do nothing.
It might be overrided for derived class when it is not convenient to override the Bond::Load method.
*/
	virtual bool LoadNode (xmlNodePtr node);
/*!
@param xml the xmlDoc used to save the document.
@param node a pointer to the xmlNode to which this Bond is serialized.

This virtual method is called at the end of the Bond::Save method. The default behavior is to do nothing.
It might be overrided for derived class when it is not convenient to override the Bond::Save method.
*/
	virtual bool SaveNode (xmlDocPtr xml, xmlNodePtr node) const;
/*!
@param x the x component of the transation vector.
@param y the y component of the transation vector.
@param z the z component of the transation vector.

Does nothing. Just a slight optimization to avoid execution of Object::Move
*/
	virtual void Move (double x, double y, double z = 0.);
/*!
@param m the 2D Matrix of the transformation.
@param x the x component of the center of the transformation.
@param y the y component of the center of the transformation.

Does nothing. Just a slight optimization to avoid execution of Object::Transform2D
*/
	virtual void Transform2D (Matrix2D& m, double x, double y);

/*!
@param property the identity of the property as defined in objprops.h.
@param value the value of the property as a string.

Used by the gcu::Loader mechanism to load properties of bonds.
@return true on success.
*/
	bool SetProperty (unsigned property, char const *value);

/*!
@param property the identity of the property as defined in objprops.h.

Used by the gcu::Loader mechanism to retrieve properties of bonds.
@return the value of the property as a string.
*/
	std::string GetProperty (unsigned property) const;

/*!
@param pCycle a cycle to add to the list.
*/
	virtual void AddCycle (Cycle* pCycle);

/*!
@param pCycle a cycle to remove from the list.
*/
	virtual void RemoveCycle (Cycle* pCycle);

/*!
Clears the bond cycles list.
*/
	virtual void RemoveAllCycles ();

/*!
@return the number of cycles to which the bond belongs.
*/
	unsigned IsCyclic () {return m_Cycles.size ();}

/*!
@param i a C++ std::map iterator.
@param pCycle a cycle.

@return the first cycle to which the bond belongs, excluding the one passed
as second argument.
*/
	Cycle* GetFirstCycle (std::list<Cycle*>::iterator& i, Cycle * pCycle);

/*!
@param i a C++ std::map iterator initialized by Bond::GetFirstCycle.
@param pCycle a cycle.

@return the next cycle to which the bond belongs, excluding the one passed
as second argument.
*/
	Cycle* GetNextCycle (std::list<Cycle*>::iterator& i, Cycle * pCycle);

/*!
@param pCycle a cycle in which the bond might be.

@return true if the bond is in the cycle.
*/
	bool IsInCycle (Cycle* pCycle);

/*!
@return the length of the bond when projected to the xy plane.
*/
	double Get2DLength ();

/*!
@param pAtom one of the atoms of the bond.

@return the direction of the bond starting from the atom.
*/
	double GetAngle2DRad (Atom* pAtom);

/*!
@param oldAtom the atom to replace.
@param newAtom the new atom.

To sucess, this needs that the new atom can accpt all the bonds from the old one.
@return true on success.
*/
	bool ReplaceAtom (Atom* oldAtom, Atom* newAtom);

protected:
/*!
The order of the bond.
*/
	unsigned char m_order;
/*!
A pointer to the first Atom instance of the Bond.
*/
	Atom *m_Begin;
/*!
A pointer to the last Atom instance of the Bond.
*/
	Atom *m_End;// only 2 centered bonds, other bonds should be covered by derived classes

/*!
The gcu::Cycle instnces to which the bond pertains.
*/
	std::list<Cycle*> m_Cycles;
};

} // namespace gcu

#endif // GCU_BOND_H
