// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * gcu/loader.h
 *
 * Copyright (C) 2007-2008 Jean Bréfort <jean.brefort@normalesup.org>
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

#ifndef GCU_OBJ_PROPS_H
#define GCU_OBJ_PROPS_H

/*!\file
This file contains the list of symbolic Ids for objects properties as used in
gcu::Object::GetProperty and gcu::Object::SetProperty in conjunction with
 serialization using the gcu::Loader class.
*/
enum {
	// Common properties
/*!
The unique Id of the object.
*/
	GCU_PROP_ID,
/*!
The position of an object in a 2D representation.
*/
	GCU_PROP_POS2D,
/*!
The position of an object in a 2D representation.
*/
	GCU_PROP_POS3D,
	// Document properties
/*!
The x coordinate of an object
*/
	GCU_PROP_X,
/*!
The y coordinate of an object
*/
	GCU_PROP_Y,
/*!
The z coordinate of an object
*/
	GCU_PROP_Z,
/*!
The file name.
*/
	GCU_PROP_DOC_FILENAME,
/*!
The mime type.
*/
	GCU_PROP_DOC_MIMETYPE,
/*!
The title of the document.
*/
	GCU_PROP_DOC_TITLE,
/*!
A text added to the document as comment.
*/
	GCU_PROP_DOC_COMMENT,
/*!
Who created the file.
*/
	GCU_PROP_DOC_CREATOR,
/*!
The date of file creation.
*/
	GCU_PROP_DOC_CREATION_TIME,
/*!
The date of the last file modification.
*/
	GCU_PROP_DOC_MODIFICATION_TIME,
	// Theme related properties (might be doc properties in some formats)
/*!
The default bond length.
*/
	GCU_PROP_THEME_BOND_LENGTH,
	// Atom properties
/*!
The symbol of an atom.
*/
	GCU_PROP_ATOM_SYMBOL,
/*!
The atomic number of an atom.
*/
	GCU_PROP_ATOM_Z,
/*!
The charge of an atom.
*/
	GCU_PROP_ATOM_CHARGE,
	// Bond properties
/*!
The Id of the atom at the first extremity of the bond.
*/
	GCU_PROP_BOND_BEGIN,
/*!
The Id of the atom at the last extremity of the bond.
*/
	GCU_PROP_BOND_END,
/*!
*/
	GCU_PROP_BOND_ORDER,
/*!
The bond order.
*/
	GCU_PROP_BOND_TYPE, //normal, hash, wedge,...
	// Text properties
/*!
The text of a textual object.
*/
	GCU_PROP_TEXT_TEXT,
/*!
The markup of a textual object.
*/
	GCU_PROP_TEXT_MARKUP,
/*!
The alignment of a textual object.
*/
	GCU_PROP_TEXT_ALIGNMENT,
/*!
The justification of a textual object.
*/
	GCU_PROP_TEXT_JUSTIFICATION,
/*!
The position in bytes of the bonded atom in a group of atoms.
*/
	GCU_PROP_FRAGMENT_ATOM_START, // index of the start of the symbol of the bonded atom if any
/*!
The Id of the bonded atom in a group of atoms.
*/
	GCU_PROP_FRAGMENT_ATOM_ID,
	// Arrows properties
/*!
The 2D coordinates of the start and end points of the arrow in the order xstart, ystart,
xend, yend, separated by white spaces.
*/
	GCU_PROP_ARROW_COORDS,
/*!
The Id of the object representing the reactants.
*/
	GCU_PROP_ARROW_START_ID,
/*!
The Id of the object representing the products.
*/
	GCU_PROP_ARROW_END_ID,
/*!
The type of a reaction arrow ("double" for a reversible arrow).
*/
	GCU_PROP_REACTION_ARROW_TYPE,
};

#endif	//	GCU_OBJ_PROPS_H
