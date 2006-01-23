(***************************************************************************
 *   Copyright (C) 2004 by Thomas Nagy                                     *
 *   tnagy2^8@yahoo.fr                                                     *
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
 ***************************************************************************)

open Printf;;
open Calc;;


let myeq1 = " a CH3(CH2)3COOH + b O2 -> c H2O + d CO2";;
let myeq2 = " a CH3COOH + b O2 -> c H2O + d CO2";;
let myeq3 = " a CH3(CH2)3COOH + b O2 -> invalid equation ";;
let myeq4 = "2CH4+aC2H6+cO2->eH2O+5CO2";;
let myeq5 = "a NaNO2 + b NH3 -> b NaN3 + e NaNO3";;
let myeq6 = "a HNO2 + b NH3 -> b HN3 + e HNO3";;
let myeq7 = "a H3O[+] + dCO2 + eO2 -> fH2O + dCO3[2+]";;

(* Printf.printf "%s \n" (solve_equation myeq1);;
Printf.printf "%s \n" (solve_equation myeq2);;
Printf.printf "%s \n" (solve_equation myeq3);; *)
(*
Printf.printf "%s \n" (solve_equation myeq5);;
Printf.printf "%s \n" (solve_equation myeq6);;
Printf.printf "%s \n" (solve_equation myeq6);;

Printf.printf "%s \n" (solve_equation myeq6);;*)
Printf.printf "%s \n" (solve_equation myeq7);;




