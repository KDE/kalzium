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
open Chemset;;
open Datastruct;;
open Chem;;
open Hashtbl;;

let create_equation str = 
    let lexbuf = Lexing.from_string str in
    let result = Parser.main Lexer.token lexbuf in
    result
;;

exception Not_found;;

let solve_equation (str:string) =
    let eq = new eqtable in
    try
        eq#build (create_equation str);
        try
(*          eq#print_all (); *)
            solve eq;
            eq#get_eq_sol ();

        with | _ -> begin 
            let str = (eq#get_eq_orig ())^" : No solution found" in
            (*cleanup eq;*)
            str
        end
    with | _ -> str^" : Parse Error";
;;

let _ = Callback.register "solve_equation" solve_equation;;

