(* ocamlc -I +threads unix.cma threads.cma race_condition.ml -o prog *)
let _ =
  let n = ref 0 in
  let loop () : unit =
    for i = 1 to 100 do
      let m = !n in
      for j = 1 to 100000 do
        ()
      done;
      n := m + 1
    done
  in
  let t1 = Thread.create loop () and t2 = Thread.create loop () in
  Thread.join t1;
  Thread.join t2;
  Printf.printf "%d\n" !n
