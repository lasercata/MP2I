type term =
  | Var of int
  | Lambda of int * term
  | App of term * term

type token =
  | VAR of int
  | LAMBDA
  | DOT
  | LPAR
  | RPAR
  | EOF

let lexer (s : string) : token list =
  let n = String.length s in
  let rec q_0 (i : int) : token * int =
    if i = n then EOF, i
    else
      match s.[i] with
      | '(' -> LPAR, i
      | ')' -> RPAR, i
      | '.' -> DOT, i
      | 'l' -> q_lambda (i + 1)
      | 'x' -> q_var (i + 1) (i + 1)
      | ' ' -> q_0 (i + 1)
      | _ -> failwith "Syntax error: invalid symbol"
    and q_lambda (i : int) : token * int =
      if i < n - 1 && s.[i] = 'b' && s.[i + 1] = 'd' then LAMBDA, i + 1
      else failwith "Syntax error: lbd expected"
    and q_var (i : int) (j : int) : token * int =
      if j < n && '0' <= s.[j] && s.[j] <= '9' then q_var i (j + 1)
      else if i = j then failwith "Syntax error: missing variable index"
      else VAR (int_of_string (String.sub s i (j - i))), j - 1
    and build (acc : token list) (i : int) : token list =
      match q_0 i with
      | EOF, _ -> List.rev (EOF :: acc)
      | token, i -> build (token :: acc) (i + 1)
  in
  build [] 0

let t_string : string =
  "(((lbd x0. (lbd x1. (lbd x2. (lbd x3. ((x0 x2) ((x1 x2) x3)))))) " ^
    " (lbd x0. (lbd x1. (x0 x1)))) (lbd x0. (lbd x1. (x0 x1))))"

let t_tokens : token list =
  lexer t_string

(* grammar:
   S -> T EOF
   T -> V | LPAR T T RPAR | LPAR LAMBDA V DOT T RPAR
   V -> VAR n
 *)

let rec parseS (l : token list) : term =
  match parseT l with
  | t, [EOF] -> t
  | _ -> failwith "Parsing failed"

and parseT (l : token list) : term * token list =
  match l with
  | LPAR :: LAMBDA :: l ->
     begin
       match parseV l with
       | Var n, DOT :: l ->
          begin
            match parseT l with
            | t, RPAR :: l -> Lambda (n, t), l
            | _ -> failwith "Syntax error: missing parenthesis closing lambda"
          end
       | _ -> failwith "Syntax error: missing dot in lambda"
     end
  | LPAR :: l ->
     begin
       let t1, l = parseT l in
       match parseT l with
       | t2, RPAR :: l -> App (t1, t2), l
       | _ -> failwith "Syntax error: missing parenthesis closing application"
     end
  | _ -> parseV l

and parseV (l : token list) : term * token list =
  match l with
  | VAR n :: l -> Var n, l
  | _ -> failwith "Syntax error: expected variable"

let t : term = parseS t_tokens
