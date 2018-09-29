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
    id: detailview
    x:0
    y:0
    width: itemWidth
    height: itemHeight

    gradient: Gradient {
        GradientStop { position: 0.0; color: backgroundColor }
        GradientStop { position: 1.0; color: backgroundColor2 }
    }

    MouseArea { id: mouseArea; anchors.fill: parent }

    states: State {
	name: "left"; when: mouseArea.pressed == true
	PropertyChanges { target: detailview; x: -detailview.width; color: "red" }
    }

    transitions: Transition {
	from: ""; to: "left"; reversible: true
	ParallelAnimation {
	    NumberAnimation { properties: "x"; duration: 500; easing.type: Easing.InOutQuad }
	    ColorAnimation { duration: 500 }
	}
    }

    Text {
	anchors.leftMargin: 5
	anchors.left: detailview.left
	text: atomName
	font.pointSize: 24
    }

    Text {
	id: idSymbol
	anchors.horizontalCenter: detailview.horizontalCenter
	anchors.verticalCenter: detailview.verticalCenter
	text: atomicSymbol
	font.pointSize: 26;
	font.bold: true
    }

    Text {
	anchors.right: idSymbol.left
	anchors.verticalCenter: detailview.verticalCenter
	text: "<sub>" + atomicNumber + "</sub>"
	font.pointSize: 24;
    }

    Column {
      id: extraValues
      anchors.bottom: parent.bottom
      anchors.leftMargin: 20;
//       anchors.rightMargin: 30;
      anchors.bottomMargin: 10;
      width: parent.width - 20

      Text {
	  anchors.right: parent.right
	  text: electroNegativity
	  font.pointSize: 16
      }

      Text {
	  anchors.right: parent.right
	  text: atomicMass
	  font.pointSize: 16
      }

      Text {
	  text: electronConf
	  font.pointSize: 16
      }

    }

    Image {
	sourceSize.height: 150; sourceSize.width: 150
	height: 150; width: 150

	anchors.right: parent.right
	anchors.bottom: parent.bottom
	visible: showSvg == true
    //	        fillMode: Image.PreserveAspectFit
    //                 fillMode: Image.PreserveAspectCrop
	source:"../iconsets/school/" + atomicNumber + ".svg"
    }
}
