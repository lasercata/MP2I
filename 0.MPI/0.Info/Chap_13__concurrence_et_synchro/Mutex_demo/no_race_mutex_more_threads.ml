let _ =
  let n = ref 0 in
  let lock = Mutex.create () in
  let loop () : unit =
    for i = 1 to 100 do
      Mutex.lock lock;
      let m = !n in
      Thread.yield ();
      n := m + 1;
      Mutex.unlock lock
    done
  in
  let t = Array.init 10 (fun _ -> Thread.create loop ()) in
  Array.iter (fun th -> Thread.join th) t;
  Printf.printf "%d\n" !n
