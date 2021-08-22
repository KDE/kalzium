/*
    SPDX-FileCopyrightText: 2011 Etienne Rebetez <etienne.rebetez@oberwallis.ch>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
