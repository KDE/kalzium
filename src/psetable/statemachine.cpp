/**********************************************************************
 Statemachine

 Copyright (C) 2010      by Etienne Rebetez, etienne.rebetez@oberwallis.ch

 This is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This software is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 02110-1301, USA.

 **********************************************************************/

#include "statemachine.h"

StateSwitcher::StateSwitcher(QStateMachine *machine)
        : QState(machine)
{
}

void StateSwitcher::addState(QState *state, QAbstractAnimation *animation, const int &id)
{
    StateSwitchTransition *trans = new StateSwitchTransition(id);
    trans->setTargetState(state);
    addTransition(trans);
    trans->addAnimation(animation);
}


void StateSwitcher::switchToState(const int &n)
{
    machine()->postEvent(new StateSwitchEvent(n));
}
