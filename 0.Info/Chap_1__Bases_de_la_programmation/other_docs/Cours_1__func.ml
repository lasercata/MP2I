(*List.exist*)
let rec exists (f:'a -> bool) (l:'a list) : bool =
  match l with
  | [] -> false
  | t::q -> f(t) || exists f q;;


 (*List.for_all*)
let rec for_all (f:'a -> bool) (l:'a list) : bool =
  match l with
  | [] -> false
  | t::q -> f(t) && for_all f q;;


(*List.filter*)
let rec filter (f:'a -> bool) (l:'a) : 'a list = 
  match l with
  | [] -> []
  | t::q -> begin match f [t] with
            | true -> [t]::filter f q
            | false -> filter f q
            end;;

(*List.map*)
let rec map (f:'a -> 'b) (l:'a list):'b list =
  match l with
  | [] -> []
  | t::q -> f t::map f q;;
