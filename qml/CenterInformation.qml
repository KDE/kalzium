/***************************************************************************
 *   Copyright (C) 2011   by Etienne Rebetez etienne.rebetez@oberwallis.ch *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

 import Qt 4.7

 Rectangle {

	Text {

	    text: "<sub>"atomicNumber"</sub>" atomName
	    width: parent.width
	    font.pointSize: 24; font.bold: true
	}

	Text {

	    text: atomicNumber
	    width: parent.width
	    font.pointSize: 20; font.bold: true
	}

 }
