/***************************************************************************
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
 ***************************************************************************/

/* File parser.mly */



%{
        open Chemset
        
        (*let parse_error somestring = Printf.printf "%s \n" somestring;;
        *)
%}

%token <int> INT
%token PLUS MINUS
%token LPAREN RPAREN LBRACKET RBRACKET
%token EOF
%token <string> CAPITAL
%token <string> MINOR
%token ARROW
%start main
%type <Chemset.listitems> main
%%

main:
    equation EOF { $1 }
;


equation:
    expr ARROW expr { chem_negate $3; List.append $1 $3 }
;

expr:
    item { $1::[] }
    | item PLUS expr { $1::$3 }
;

item:
    MINOR symbols { {ikey=$1; itbl=$2; sign=1} }
    | INT symbols { {ikey=string_of_int($1); itbl=$2; sign=1} }
;

symbols:
    qte { $1 }
    | qte symbols { chem_add $1 $2 }
;

qte:
    element INT { createchem $1 $2 }
    | element { createchem $1 1 } 
    | LPAREN symbols RPAREN INT { chem_mult $2 $4 }
    | LBRACKET INT PLUS RBRACKET   { createchem "+" $2 }
    | LBRACKET INT MINUS RBRACKET  { createchem "-" $2 }
    | LBRACKET PLUS RBRACKET   { createchem "+" 1 }
    | LBRACKET MINUS RBRACKET  { createchem "-" 1 }
;

element:
    CAPITAL MINOR { $1 ^ $2 }
    | CAPITAL { $1 } 
;



