type token =
  | INT of (int)
  | PLUS
  | MINUS
  | LPAREN
  | RPAREN
  | LBRACKET
  | RBRACKET
  | EOF
  | CAPITAL of (string)
  | MINOR of (string)
  | ARROW

open Parsing;;
# 26 "parser.mly"
        open Chemset
        
        (*let parse_error somestring = Printf.printf "%s \n" somestring;;
        *)
# 21 "parser.ml"
let yytransl_const = [|
  258 (* PLUS *);
  259 (* MINUS *);
  260 (* LPAREN *);
  261 (* RPAREN *);
  262 (* LBRACKET *);
  263 (* RBRACKET *);
    0 (* EOF *);
  266 (* ARROW *);
    0|]

let yytransl_block = [|
  257 (* INT *);
  264 (* CAPITAL *);
  265 (* MINOR *);
    0|]

let yylhs = "\255\255\
\001\000\002\000\003\000\003\000\004\000\004\000\005\000\005\000\
\006\000\006\000\006\000\006\000\006\000\006\000\006\000\007\000\
\007\000\000\000"

let yylen = "\002\000\
\002\000\003\000\001\000\003\000\002\000\002\000\001\000\002\000\
\002\000\001\000\004\000\004\000\004\000\003\000\003\000\002\000\
\001\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\000\000\000\000\018\000\000\000\000\000\000\000\
\000\000\000\000\000\000\006\000\000\000\000\000\005\000\001\000\
\000\000\000\000\000\000\000\000\000\000\000\000\016\000\008\000\
\009\000\002\000\004\000\000\000\000\000\000\000\014\000\015\000\
\011\000\012\000\013\000"

let yydgoto = "\002\000\
\005\000\006\000\007\000\008\000\012\000\013\000\014\000"

let yysindex = "\255\255\
\003\255\000\000\010\255\010\255\000\000\003\000\252\254\011\255\
\010\255\018\255\008\255\000\000\010\255\021\255\000\000\000\000\
\003\255\003\255\020\255\007\255\019\255\022\255\000\000\000\000\
\000\000\000\000\000\000\026\255\023\255\024\255\000\000\000\000\
\000\000\000\000\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\005\000\
\000\000\000\000\001\000\000\000\015\000\008\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000"

let yygindex = "\000\000\
\000\000\000\000\006\000\000\000\254\255\000\000\000\000"

let yytablesize = 281
let yytable = "\001\000\
\017\000\015\000\016\000\003\000\003\000\017\000\019\000\010\000\
\029\000\030\000\024\000\004\000\018\000\009\000\007\000\010\000\
\023\000\011\000\020\000\021\000\022\000\025\000\026\000\027\000\
\028\000\031\000\033\000\000\000\032\000\034\000\035\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\017\000\017\000\000\000\017\000\017\000\017\000\000\000\
\017\000\010\000\017\000\010\000\010\000\010\000\003\000\010\000\
\007\000\010\000\000\000\007\000\000\000\000\000\000\000\000\000\
\007\000"

let yycheck = "\001\000\
\000\000\004\000\000\000\001\001\000\000\010\001\009\000\000\000\
\002\001\003\001\013\000\009\001\002\001\004\001\000\000\006\001\
\009\001\008\001\001\001\002\001\003\001\001\001\017\000\018\000\
\005\001\007\001\001\001\255\255\007\001\007\001\007\001\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\001\001\002\001\255\255\004\001\005\001\006\001\255\255\
\008\001\002\001\010\001\004\001\005\001\006\001\010\001\008\001\
\002\001\010\001\255\255\005\001\255\255\255\255\255\255\255\255\
\010\001"

let yynames_const = "\
  PLUS\000\
  MINUS\000\
  LPAREN\000\
  RPAREN\000\
  LBRACKET\000\
  RBRACKET\000\
  EOF\000\
  ARROW\000\
  "

let yynames_block = "\
  INT\000\
  CAPITAL\000\
  MINOR\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : 'equation) in
    Obj.repr(
# 44 "parser.mly"
                 ( _1 )
# 177 "parser.ml"
               : Chemset.listitems))
; (fun parser_env ->
    let _1 = (peek_val parser_env 2 : 'expr) in
    let _3 = (peek_val parser_env 0 : 'expr) in
    Obj.repr(
# 49 "parser.mly"
                    ( chem_negate _3; List.append _1 _3 )
# 185 "parser.ml"
               : 'equation))
; (fun parser_env ->
    let _1 = (peek_val parser_env 0 : 'item) in
    Obj.repr(
# 53 "parser.mly"
         ( _1::[] )
# 192 "parser.ml"
               : 'expr))
; (fun parser_env ->
    let _1 = (peek_val parser_env 2 : 'item) in
    let _3 = (peek_val parser_env 0 : 'expr) in
    Obj.repr(
# 54 "parser.mly"
                     ( _1::_3 )
# 200 "parser.ml"
               : 'expr))
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : string) in
    let _2 = (peek_val parser_env 0 : 'symbols) in
    Obj.repr(
# 58 "parser.mly"
                  ( {ikey=_1; itbl=_2; sign=1} )
# 208 "parser.ml"
               : 'item))
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : int) in
    let _2 = (peek_val parser_env 0 : 'symbols) in
    Obj.repr(
# 59 "parser.mly"
                  ( {ikey=string_of_int(_1); itbl=_2; sign=1} )
# 216 "parser.ml"
               : 'item))
; (fun parser_env ->
    let _1 = (peek_val parser_env 0 : 'qte) in
    Obj.repr(
# 63 "parser.mly"
        ( _1 )
# 223 "parser.ml"
               : 'symbols))
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : 'qte) in
    let _2 = (peek_val parser_env 0 : 'symbols) in
    Obj.repr(
# 64 "parser.mly"
                  ( chem_add _1 _2 )
# 231 "parser.ml"
               : 'symbols))
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : 'element) in
    let _2 = (peek_val parser_env 0 : int) in
    Obj.repr(
# 68 "parser.mly"
                ( createchem _1 _2 )
# 239 "parser.ml"
               : 'qte))
; (fun parser_env ->
    let _1 = (peek_val parser_env 0 : 'element) in
    Obj.repr(
# 69 "parser.mly"
              ( createchem _1 1 )
# 246 "parser.ml"
               : 'qte))
; (fun parser_env ->
    let _2 = (peek_val parser_env 2 : 'symbols) in
    let _4 = (peek_val parser_env 0 : int) in
    Obj.repr(
# 70 "parser.mly"
                                ( chem_mult _2 _4 )
# 254 "parser.ml"
               : 'qte))
; (fun parser_env ->
    let _2 = (peek_val parser_env 2 : int) in
    Obj.repr(
# 71 "parser.mly"
                                   ( createchem "+" _2 )
# 261 "parser.ml"
               : 'qte))
; (fun parser_env ->
    let _2 = (peek_val parser_env 2 : int) in
    Obj.repr(
# 72 "parser.mly"
                                   ( createchem "-" _2 )
# 268 "parser.ml"
               : 'qte))
; (fun parser_env ->
    Obj.repr(
# 73 "parser.mly"
                               ( createchem "+" 1 )
# 274 "parser.ml"
               : 'qte))
; (fun parser_env ->
    Obj.repr(
# 74 "parser.mly"
                               ( createchem "-" 1 )
# 280 "parser.ml"
               : 'qte))
; (fun parser_env ->
    let _1 = (peek_val parser_env 1 : string) in
    let _2 = (peek_val parser_env 0 : string) in
    Obj.repr(
# 78 "parser.mly"
                  ( _1 ^ _2 )
# 288 "parser.ml"
               : 'element))
; (fun parser_env ->
    let _1 = (peek_val parser_env 0 : string) in
    Obj.repr(
# 79 "parser.mly"
              ( _1 )
# 295 "parser.ml"
               : 'element))
(* Entry main *)
; (fun parser_env -> raise (YYexit (peek_val parser_env 0)))
|]
let yytables =
  { actions=yyact;
    transl_const=yytransl_const;
    transl_block=yytransl_block;
    lhs=yylhs;
    len=yylen;
    defred=yydefred;
    dgoto=yydgoto;
    sindex=yysindex;
    rindex=yyrindex;
    gindex=yygindex;
    tablesize=yytablesize;
    table=yytable;
    check=yycheck;
    error_function=parse_error;
    names_const=yynames_const;
    names_block=yynames_block }
let main (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (yyparse yytables 1 lexfun lexbuf : Chemset.listitems)
