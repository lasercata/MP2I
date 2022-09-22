let _ =
  let ticket = Array.make 2 0 in
  let n = ref 0 in
  let loop (id : int) : unit =
    for i = 1 to 100 do
      (* attribution du ticket *)
      let max_ticket = ref 0 in
      for j = 0 to 1 do
        max_ticket := max !max_ticket ticket.(j)
      done;
      if id = 0 then Thread.yield ();
      ticket.(id) <- 1 + !max_ticket;
      (* attente active *)
      for j = 0 to 1 do
        while ticket.(j) <> 0 && (ticket.(j), j) < (ticket.(id), id) do
          ()
        done
      done;
      (* section critique *)
      let m = !n in
      if id = 1 then
        for k = 0 to 5 do Thread.yield () done;
      n := m + 1;
      (* sortie de section critique *)
      ticket.(id) <- 0
    done
  in
  let t0 = Thread.create loop 0 and t1 = Thread.create loop 1 in
  Thread.join t0;
  Thread.join t1;
  Printf.printf "%d\n" !n
