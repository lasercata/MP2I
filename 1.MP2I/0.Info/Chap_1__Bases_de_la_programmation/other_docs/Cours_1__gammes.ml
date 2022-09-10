let rec exists (p : 'a -> bool) (l : 'a list) : bool =
  match l with
  | [] -> false
  | t :: _ when p t -> true
  | _ :: q -> exists p q

let rec for_all (p : 'a -> bool) (l : 'a list) : bool =
  match l with
  | [] -> true
  | t :: q -> p t && for_all p q

let rec filter (p : 'a -> bool) (l : 'a list) : 'a list =
  match l with
  | [] -> []
  | t :: q when p t -> t :: filter p q
  | _ :: q -> filter p q

let rec map (f : 'a -> 'b) (l : 'a list) : 'b list =
  match l with
  | [] -> []
  | t :: q -> f t :: map f q
