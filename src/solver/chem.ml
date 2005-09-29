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

open Facile;;
open Easy;;
open Datastruct;;

let solve (eq : eqtable) =
    let nb_molecules = eq#getsize_j () and nb_elements = eq#getsize_i () in
    let dist = Fd.array nb_molecules 1 900 in

    (* trivial constraints on domains *)
    for j = 0 to nb_molecules -1 do
        let num = try int_of_string (eq#getvar j) with _ -> -1 in
        if num > -1 then dist.(j) <- Fd.int num
    done;
    
    (* raises an exception if the problem is not solvable *)
    for i = 0 to nb_elements - 1 do
        Cstr.post (Arith.scalprod_fd (eq#getline i) dist =~ i2e 0)
    done;
    
    let goal = Goals.GlArray.labeling dist in
    if (Goals.solve goal) then Array.iteri (fun cnt i -> eq#setsol cnt (Fd.min i)) dist
    else failwith "no solution found"
;;

(* workaround for (probably) a bug in the facile library 1.0 (fixed in 1.1?) : 
 * when the constraints make a problem
 * unsolvable, an exception is raised
 * 
 * unfortunately the next problem
 * solved afterwards is not handled properly *)

let cleanup (eq : eqtable) =
(*  Printf.printf "cleaning up"; *)
    let nb_molecules = eq#getsize_j () and nb_elements = eq#getsize_i () in
    let dist = Fd.array nb_molecules 0 2 in
    let goal = Goals.GlArray.labeling dist in
    if not (Goals.solve goal) then failwith "fatal error"
;;


