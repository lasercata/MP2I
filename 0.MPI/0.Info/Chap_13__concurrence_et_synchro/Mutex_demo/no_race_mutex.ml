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
  let t1 = Thread.create loop () and t2 = Thread.create loop () in
  Thread.join t1;
  Thread.join t2;
  Printf.printf "%d\n" !n
