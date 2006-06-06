/***************************************************************************
 *   Copyright (C) 2006 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "rsdialog.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"

#include <klocale.h>
#include <kdebug.h>

RSDialog::RSDialog( QWidget* parent )
	: KDialog( parent , i18n( "R/S-Dialog" ), Help|Close )
{
	createRPhrases();
	createSPhrases();
}

void RSDialog::createSPhrases()
{
	sphrases << i18n("S1: Keep locked up");
	sphrases << i18n("S2: Keep out of the reach of children");
	sphrases << i18n("S3: Keep in a cool place");
	sphrases << i18n("S4: Keep away from living quarters");
	sphrases << i18n("S5: Keep contents under ... ( appropriate liquid to be specified by the manufacturer )");
	sphrases << i18n("S6: Keep under ... ( inert gas to be specified by the manufacturer )");
	sphrases << i18n("S7: Keep container tightly closed");
	sphrases << i18n("S8: Keep container dry");
	sphrases << i18n("S9: Keep container in a well-ventilated place");
	sphrases << i18n("S12: Do not keep the container sealed");
	sphrases << i18n("S13: Keep away from food, drink and animal feedingstuffs");
	sphrases << i18n("S14: Keep away from ... ( incompatible materials to be indicated by the manufacturer )");
	sphrases << i18n("S15: Keep away from heat");
	sphrases << i18n("S16: Keep away from sources of ignition - No smoking");
	sphrases << i18n("S17: Keep away from combustible material");
	sphrases << i18n("S18: Handle and open container with care");
	sphrases << i18n("S20: When using do not eat or drink");
	sphrases << i18n("S21: When using do not smoke");
	sphrases << i18n("S22: Do not breathe dust");
	sphrases << i18n("S23: Do not breathe gas/fumes/vapour/spray ( appropriate wording to be specified by the manufacturer )");
	sphrases << i18n("S24: Avoid contact with skin");
	sphrases << i18n("S25: Avoid contact with eyes");
	sphrases << i18n("S26: In case of contact with eyes, rinse immediately with plenty of water and seek medical advice");
	sphrases << i18n("S27: Take off immediately all contaminated clothing");
	sphrases << i18n("S28: After contact with skin, wash immediately with plenty of ... ( to be specified by the manufacturer )");
	sphrases << i18n("S29: Do not empty into drains");
	sphrases << i18n("S30: Never add water to this product");
	sphrases << i18n("S33: Take precautionary measures against static discharges");
	sphrases << i18n("S35: This material and its container must be disposed of in a safe way");
	sphrases << i18n("S36: Wear suitable protective clothing");
	sphrases << i18n("S37: Wear suitable gloves");
	sphrases << i18n("S38: In case of insufficient ventilation wear suitable respiratory equipment");
	sphrases << i18n("S39: Wear eye/face protection");
	sphrases << i18n("S40: To clean the floor and all objects contaminated by this material use ... ( to be specified by the manufacturer )");
	sphrases << i18n("S41: In case of fire and/or explosion do not breathe fumes");
	sphrases << i18n("S42: During fumigation/spraying wear suitable respiratory equipment ( appropriate wording to be specified by the manufacturer )");
	sphrases << i18n("S43: In case of fire use ... ( indicate in the space the precise type of fire-fighting equipment. If water increases the risk add - Never use water )");
	sphrases << i18n("S45: In case of accident or if you feel unwell seek medical advice immediately ( show the label where possible )");
	sphrases << i18n("S46: If swallowed, seek medical advice immediately and show this container or label");
	sphrases << i18n("S47: Keep at temperature not exceeding ... °C ( to be specified by the manufacturer )");
	sphrases << i18n("S48: Keep wet with ... ( appropriate material to be specified by the manufacturer )");
	sphrases << i18n("S49: Keep only in the original container");
	sphrases << i18n("S50: Do not mix with ... ( to be specified by the manufacturer )");
	sphrases << i18n("S51: Use only in well-ventilated areas");
	sphrases << i18n("S52: Not recommended for interior use on large surface areas");
	sphrases << i18n("S53: Avoid exposure - obtain special instructions before use");
	sphrases << i18n("S56: Dispose of this material and its container at hazardous or special waste collection point");
	sphrases << i18n("S57: Use appropriate containment to avoid environmental contamination");
	sphrases << i18n("S59: Refer to manufacturer/supplier for information on recovery/recycling");
	sphrases << i18n("S60: This material and its container must be disposed of as hazardous waste");
	sphrases << i18n("S61: Avoid release to the environment. Refer to special instructions/safety data sheet");
	sphrases << i18n("S62: If swallowed, do not induce vomiting: seek medical advice immediately and show this container or label");
	sphrases << i18n("S63: In case of accident by inhalation: remove casualty to fresh air and keep at rest");
	sphrases << i18n("S64: If swallowed, rinse mouth with water ( only if the person is conscious )");
}

void RSDialog::createRPhrases()
{
	//Translations can be found here: (the official ones)
	//	http://europa.eu.int/eur-lex/lex/LexUriServ/LexUriServ.do?uri=CELEX:32001L0059:EN:HTML
	rphrases << i18n("R1: Explosive when dry");
	rphrases << i18n("R2: Risk of explosion by shock, friction, fire or other sources of ignition");
	rphrases << i18n("R3: Extreme risk of explosion by shock, friction, fire or other sources of ignition");
	rphrases << i18n("R4: Forms very sensitive explosive metallic compounds");
	rphrases << i18n("R5: Heating may cause an explosion");
	rphrases << i18n("R6: Explosive with or without contact with air");
	rphrases << i18n("R7: May cause fire");
	rphrases << i18n("R8: Contact with combustible material may cause fire");
	rphrases << i18n("R9: Explosive when mixed with combustible material");
	rphrases << i18n("R10: Flammable");
	rphrases << i18n("R11: Highly flammable");
	rphrases << i18n("R12: Extremely flammable");
	rphrases << i18n("R14: Reacts violently with water");
	rphrases << i18n("R15: Contact with water liberates extremely flammable gases");
	rphrases << i18n("R16: Explosive when mixed with oxidising substances");
	rphrases << i18n("R17: Spontaneously flammable in air");
	rphrases << i18n("R18: In use, may form flammable/explosive vapour-air mixture");
	rphrases << i18n("R19: May form explosive peroxides");
	rphrases << i18n("R20: Harmful by inhalation");
	rphrases << i18n("R21: Harmful in contact with skin");
	rphrases << i18n("R22: Harmful if swallowed");
	rphrases << i18n("R23: Toxic by inhalation");
	rphrases << i18n("R24: Toxic in contact with skin");
	rphrases << i18n("R25: Toxic if swallowed");
	rphrases << i18n("R26: Very toxic by inhalation");
	rphrases << i18n("R27: Very toxic in contact with skin");
	rphrases << i18n("R28: Very toxic if swallowed");
	rphrases << i18n("R29: Contact with water liberates toxic gas.");
	rphrases << i18n("R30: Can become highly flammable in use");
	rphrases << i18n("R31: Contact with acids liberates toxic gas");
	rphrases << i18n("R32: Contact with acids liberates very toxic gas");
	rphrases << i18n("R33: Danger of cumulative effects");
	rphrases << i18n("R34: Causes burns");
	rphrases << i18n("R35: Causes severe burns");
	rphrases << i18n("R36: Irritating to eyes");
	rphrases << i18n("R37: Irritating to respiratory system");
	rphrases << i18n("R38: Irritating to skin");
	rphrases << i18n("R39: Danger of very serious irreversible effects");
	rphrases << i18n("R40: Limited evidence of a carcinogenic effect");
	rphrases << i18n("R41: Risk of serious damage to eyes");
	rphrases << i18n("R42: May cause sensitisation by inhalation");
	rphrases << i18n("R43: May cause sensitisation by skin contact");
	rphrases << i18n("R44: Risk of explosion if heated under confinement");
	rphrases << i18n("R45: May cause cancer");
	rphrases << i18n("R46: May cause heritable genetic damage");
	rphrases << i18n("R48: Danger of serious damage to health by prolonged exposure");
	rphrases << i18n("R49: May cause cancer by inhalation");
	rphrases << i18n("R50: Very toxic to aquatic organisms");
	rphrases << i18n("R51: Toxic to aquatic organisms");
	rphrases << i18n("R52: Harmful to aquatic organisms");
	rphrases << i18n("R53: May cause long-term adverse effects in the aquatic environment");
	rphrases << i18n("R54: Toxic to flora");
	rphrases << i18n("R55: Toxic to fauna");
	rphrases << i18n("R56: Toxic to soil organisms");
	rphrases << i18n("R57: Toxic to bees");
	rphrases << i18n("R58: May cause long-term adverse effects in the environment");
	rphrases << i18n("R59: Dangerous for the ozone layer");
	rphrases << i18n("R60: May impair fertility");
	rphrases << i18n("R61: May cause harm to the unborn child");
	rphrases << i18n("R62: Possible risk of impaired fertility");
	rphrases << i18n("R63: Possible risk of harm to the unborn child");
	rphrases << i18n("R64: May cause harm to breast-fed babies");
	rphrases << i18n("R65: Harmful: may cause lung damage if swallowed");
	rphrases << i18n("R66: Repeated exposure may cause skin dryness or cracking");
	rphrases << i18n("R67: Vapours may cause drowsiness and dizziness");
	rphrases << i18n("R68: Possible risk of irreversible effects");
}

#include "rsdialog.moc"
