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

(* File lexer.mll*)
{
        open Parser;;
        exception IllegalChar
}

rule token = parse
[' ' '\t' '\n']   {token lexbuf} (* ignore whitespaces *)
| ['0'-'9']+      { INT(int_of_string(Lexing.lexeme lexbuf)) }
| ['A'-'Z']       { CAPITAL(Lexing.lexeme lexbuf) }
| ['a'-'z']+      { MINOR(Lexing.lexeme lexbuf) }
| '+'             { PLUS }
| '-'             { MINUS }
| '('             { LPAREN }
| ')'             { RPAREN }
| '['             { LBRACKET }
| ']'             { RBRACKET }
| "->"            { ARROW }
| _               { raise IllegalChar; }
| eof             { EOF }

