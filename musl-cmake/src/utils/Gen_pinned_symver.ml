(* ocaml str.cma Gen_pinned_symver.ml *)

let for_list xs f = List.iter f xs
let for_seq xs f = Seq.iter f xs

type version = Version of (int * int * int option)

let glibc_2_3_2 = Version (2, 3, Some 2)

module VersionOrd = struct
  type t = version
  let compare (Version (x, y, z)) (Version (a, b, c)) =
    match (z, c) with
    | _ when x < a -> -1
    | _ when x > a -> 1
    | _ when y < b -> -1
    | _ when y > b -> 1
    | (None, Some _) -> -1
    | (Some _, None) -> 1
    | (Some z, Some c) when z < c -> -1
    | (Some z, Some c) when z > c -> 1
    | _ -> 0
end

module VersionSet = Set.Make(VersionOrd)

let no_overrides () : ((string * string), version) Hashtbl.t = Hashtbl.create 0

let pthread_syms = [
  "pthread_create";
  "pthread_exit";
  "pthread_join";
  "pthread_tryjoin_np";
  "pthread_timedjoin_np";
  "pthread_clockjoin_np";
  "pthread_detach";
  "pthread_self";
  "pthread_equal";
  "pthread_attr_init";
  "pthread_attr_destroy";
  "pthread_attr_getdetachstate";
  "pthread_attr_setdetachstate";
  "pthread_attr_getguardsize";
  "pthread_attr_setguardsize";
  "pthread_attr_getschedparam";
  "pthread_attr_setschedparam";
  "pthread_attr_getschedpolicy";
  "pthread_attr_setschedpolicy";
  "pthread_attr_getinheritsched";
  "pthread_attr_setinheritsched";
  "pthread_attr_getscope";
  "pthread_attr_setscope";
  "pthread_attr_getstackaddr";
  "pthread_attr_setstackaddr";
  "pthread_attr_getstacksize";
  "pthread_attr_setstacksize";
  "pthread_attr_getstack";
  "pthread_attr_setstack";
  "pthread_attr_setaffinity_np";
  "pthread_attr_getaffinity_np";
  "pthread_getattr_default_np";
  "pthread_attr_setsigmask_np";
  "pthread_attr_getsigmask_np";
  "pthread_setattr_default_np";
  "pthread_getattr_np";
  "pthread_setschedparam";
  "pthread_getschedparam";
  "pthread_setschedprio";
  "pthread_getname_np";
  "pthread_setname_np";
  "pthread_yield";
  "pthread_setaffinity_np";
  "pthread_getaffinity_np";
  "pthread_once";
  "pthread_setcancelstate";
  "pthread_setcanceltype";
  "pthread_cancel";
  "pthread_testcancel";
  "__pthread_register_cancel";
  "__pthread_unregister_cancel";
  "__pthread_register_cancel_defer";
  "__pthread_unregister_cancel_restore";
  "__pthread_unwind_next";
  "pthread_mutex_init";
  "pthread_mutex_destroy";
  "pthread_mutex_trylock";
  "pthread_mutex_lock";
  "pthread_mutex_timedlock";
  "pthread_mutex_clocklock";
  "pthread_mutex_unlock";
  "pthread_mutex_getprioceiling";
  "pthread_mutex_setprioceiling";
  "pthread_mutexattr_init";
  "pthread_mutexattr_destroy";
  "pthread_mutexattr_getpshared";
  "pthread_mutexattr_setpshared";
  "pthread_mutexattr_getprotocol";
  "pthread_mutexattr_setprotocol";
  "pthread_mutexattr_getprioceiling";
  "pthread_mutexattr_setprioceiling";
  "pthread_mutexattr_getrobust";
  "pthread_mutexattr_getrobust_np";
  "pthread_mutexattr_setrobust";
  "pthread_mutexattr_setrobust_np";
  "pthread_rwlock_init";
  "pthread_rwlock_destroy";
  "pthread_rwlock_rdlock";
  "pthread_rwlock_tryrdlock";
  "pthread_rwlock_timedrdlock";
  "pthread_rwlock_clockrdlock";
  "pthread_rwlock_wrlock";
  "pthread_rwlock_trywrlock";
  "pthread_rwlock_timedwrlock";
  "pthread_rwlock_clockwrlock";
  "pthread_rwlock_unlock";
  "pthread_rwlockattr_init";
  "pthread_rwlockattr_destroy";
  "pthread_rwlockattr_getpshared";
  "pthread_rwlockattr_setpshared";
  "pthread_rwlockattr_getkind_np";
  "pthread_rwlockattr_setkind_np";
  "pthread_cond_init";
  "pthread_cond_destroy";
  "pthread_cond_signal";
  "pthread_cond_broadcast";
  "pthread_cond_wait";
  "pthread_cond_timedwait";
  "pthread_cond_clockwait";
  "pthread_condattr_init";
  "pthread_condattr_destroy";
  "pthread_condattr_getpshared";
  "pthread_condattr_setpshared";
  "pthread_condattr_getclock";
  "pthread_condattr_setclock";
  "pthread_spin_init";
  "pthread_spin_destroy";
  "pthread_spin_lock";
  "pthread_spin_trylock";
  "pthread_spin_unlock";
  "pthread_barrier_init";
  "pthread_barrier_destroy";
  "pthread_barrier_wait";
  "pthread_barrierattr_init";
  "pthread_barrierattr_destroy";
  "pthread_barrierattr_getpshared";
  "pthread_barrierattr_setpshared";
  "pthread_key_create";
  "pthread_key_delete";
  "pthread_getspecific";
  "pthread_setspecific";
  "pthread_getcpuclockid";
  "pthread_atfork";
]

let x86_64__glibc_2_3_2 = [("x86_64/64", glibc_2_3_2)]

let pthread_overrides =
  let overrides = [
    ("pthread_cond_broadcast", x86_64__glibc_2_3_2);
    ("pthread_cond_destroy", x86_64__glibc_2_3_2);
    ("pthread_cond_init", x86_64__glibc_2_3_2);
    ("pthread_cond_signal", x86_64__glibc_2_3_2);
    ("pthread_cond_timedwait", x86_64__glibc_2_3_2);
    ("pthread_cond_wait", x86_64__glibc_2_3_2);
  ] in
  let ht = Hashtbl.create 16 in
  for_list overrides (fun (sym, arch_overrides) ->
    for_list arch_overrides (fun (arch, version) ->
      Hashtbl.replace ht (sym, arch) version));
  ht

let dl_syms = [
  "dlopen";
  "dlclose";
  "dlsym";
  "dlmopen";
  "dlvsym";
  "dlerror";
  "dladdr";
  "dladdr1";
  "dlinfo";
]

let unistd_syms = [
  "setegid";
  "setreuid";
  "setresgid";
  "setresuid";
  "setuid";
  "setregid";
  "setgid";
  "seteuid";
  "setgroups";
]

let malloc_syms = [
  "malloc";
  "calloc";
  "realloc";
  "free";
  "valloc";
  "memalign";
  "malloc_usable_size";
  (* aligned_alloc is stubbed and backed by posix_memalign *)
]

let stdlib_syms = [
  "posix_memalign";
  (* reallocarray is stubbed and backed by realloc *)
] @ malloc_syms

let errno_syms = [
    "__errno_location";
    "program_invocation_name";
    "program_invocation_short_name";
]

let netdb_syms = [
    "__h_errno_location";
]

let libs = [
    (pthread_syms, pthread_overrides, "pthread", {|#include "__glibc-compat-internal/pthread.h"|});
    (dl_syms, no_overrides (), "dlfcn", {|#include "__glibc-compat-internal/dlfcn.h"|});
    (unistd_syms, no_overrides (), "unistd", {|#include_next "unistd.h"|});
    (malloc_syms, no_overrides (), "malloc", {|#include_next "malloc.h"|});
    (stdlib_syms, no_overrides (), "stdlib", {|#include_next "stdlib.h"|});
    (errno_syms, no_overrides (), "errno", {|#include "__glibc-compat-internal/errno.h"|});
    (netdb_syms, no_overrides (), "netdb", {|#include_next "netdb.h"|});
]

let architectures = [
  ("x86_64/64"             , "defined(__amd64__)");
  ("i386"                  , "defined(__i386__)");
  ("aarch64"               , "defined(__aarch64__)");
  ("arm/le"                , "defined(__arm__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__");
  ("arm/be"                , "defined(__arm__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__");
  ("powerpc/powerpc64/le"  , "defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__");
  ("powerpc/powerpc64/be"  , "defined(__powerpc64__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__");
  ("powerpc/powerpc32/fpu" , "defined(__powerpc__) && !defined(__powerpc64__)");
]

let bracket open' close f =
  let x = open' () in
  try let r = f x in close x; r
  with e -> close x; raise e

let read_abilist root arch =
  let path = Printf.sprintf "%s/%s/libc.abilist" root arch in
  bracket (fun () -> open_in_bin path) close_in (fun inc ->
    let lines = ref [] in
    (try
      while true; do
        let line = input_line inc in
        lines := line :: !lines
      done
    with End_of_file -> ());
    let lines = !lines in
    List.rev lines)

let version_regexp = Str.regexp {|^[a-zA-Z]+_\([0-9]+\)\.\([0-9]+\)\(\.[0-9]+\)?$|}

exception Abort of string

let parse_version (verstring : string) : (version, string) result =
  let does_match = Str.string_match version_regexp verstring 0 in
  let parse_int str ctx =
    try int_of_string str
    with Failure _ -> raise (Abort (Printf.sprintf {|parse_version: %s: string "%s" couldn't be parsed as a base-10 decimal integer|} ctx str))
  in
  if does_match then (
    try
      let x = parse_int (Str.matched_group 1 verstring) "match-0" in
      let y = parse_int (Str.matched_group 2 verstring) "match-1" in
      let z =
        try
          let str = Str.matched_group 3 verstring in
          let str = String.sub str 1 (String.length str - 1) in
          let ver = parse_int str "match-2" in
          Some ver
        with Not_found -> None
      in
      Ok (Version (x, y, z))
    with Abort err -> Error err
  )
  else Error (Printf.sprintf {|parse_version: string "%s" doesn't match version_regexp|} verstring)

let print_version (Version (x, y, z)) : string =
  let b = Buffer.create 16 in
  Buffer.add_string b "GLIBC_";
  Buffer.add_string b (string_of_int x);
  Buffer.add_char b '.';
  Buffer.add_string b (string_of_int y);
  (match z with
  | Some z ->
    Buffer.add_char b '.';
    Buffer.add_string b (string_of_int z)
  | None -> ());
  Buffer.contents b

let space_regexp = Str.regexp " "

let split_on_space (str : string) : string list = Str.split space_regexp str

let parse_abilist (abilist : string list) : (string * version) list =
  let handle_entry (entry : string) = match split_on_space entry with
    | ver :: sym :: _ ->
      let ver = match parse_version ver with
      | Ok ver -> ver
      | Error err -> failwith (Printf.sprintf {|parse_abilist: entry = "%s"; err = %s"|} entry err)
      in
      (sym, ver)
    | _ -> failwith (Printf.sprintf {|parse_abilist: unrecognized entry; entry = "%s"|} entry)
  in
  List.map handle_entry abilist

let index_abilist (abilist : (string * version) list) : (string, VersionSet.t) Hashtbl.t =
  let ret = Hashtbl.create 16 in
  let handle_entry (sym, version) =
    let versions = Hashtbl.find_opt ret sym in
    let versions = match versions with
    | Some v -> v
    | None -> VersionSet.empty
    in
    let versions = VersionSet.add version versions in
    Hashtbl.replace ret sym versions
  in
  List.iter handle_entry abilist;
  ret

let process_group arch index syms overrides =
  let associate_sym_with_version sym =
    try
      let versions = Hashtbl.find_opt index sym in
      let versions = match versions with
      | Some versions -> versions
      | None ->
          Printf.printf "process_group: symbol %s not found in architecture %s\n" sym arch;
          raise (Abort "")
      in
      let override = Hashtbl.find_opt overrides (sym, arch) in
      let version = match override with
      | Some override ->
          if VersionSet.mem override versions
          then override
          else failwith (Printf.sprintf "process_group: version %s not found for symbol %s in architecture %s" (print_version override) sym arch)
      | None ->
          let earliest_version = VersionSet.min_elt versions in
          earliest_version
      in
      Some (sym, version)
    with
      Abort _ -> None
  in
  let pinned_versions = List.filter_map associate_sym_with_version syms in
  pinned_versions

let create_file_for_writing dest f = bracket
  (fun () -> open_out_gen [Open_binary; Open_wronly; Open_trunc; Open_creat] 0o644 dest)
  close_out f

let preproc_directives = Seq.cons ("#if", "(") (Seq.repeat ("#elif", ") ("))

let print_versions_to_file arch_pinned_sym_versions dest =
  create_file_for_writing dest (fun file ->
    let open Printf in
    let xs = Seq.zip preproc_directives (List.to_seq arch_pinned_sym_versions) in
    fprintf file "#pragma once\n";
    fprintf file "\n";
    for_seq xs (fun ((preproc_directive, brace_match), (_architecture, guard, pinned_versions)) ->
      fprintf file "%s %s // %s\n" preproc_directive guard brace_match;
      for_list pinned_versions (fun (sym, version) ->
        let version = print_version version in
        fprintf file "    __asm__(\".symver %s, %s@%s\");\n" sym sym version));
    fprintf file "#endif // )\n")

let print_wrapper_to_file dest include_fragment version_include_path =
  create_file_for_writing dest (fun file ->
    let open Printf in
    fprintf file "#pragma once\n";
    fprintf file "%s\n" include_fragment;
    fprintf file "#include \"%s\"\n" version_include_path)

let run () =
  Printexc.record_backtrace true;
  let set_str r = Arg.String (fun x -> r := Some x) in
  let dest_dir : string option ref = ref None in
  let abilist_dir : string option ref = ref None in
  let argspec = [
    ("-dest-dir", set_str dest_dir, "Destination directory for generated files.");
    ("-abilist-dir", set_str abilist_dir, "Source directory for glibc abilist files.");
  ] in
  Arg.parse argspec (fun x -> ()) (Printf.sprintf "%s -dest-dir DIR -abilist-dir DIR" Sys.argv.(0));
  let require_arg descr arg = match arg with
  | Some x -> x
  | None -> failwith (Printf.sprintf "required argument %s not specified" descr)
  in
  let dest_dir = require_arg "-dest-dir" !dest_dir in
  let abilist_dir = require_arg "-abilist-dir" !abilist_dir in
  Printf.printf "dest_dir = %s; abilist_dir = %s\n" dest_dir abilist_dir;
  let process_lib (syms, overrides, filename, include_fragment) =
    let process_architecture (arch_abilist_location, guard) =
      let abilist = read_abilist abilist_dir arch_abilist_location in
      let parsed = parse_abilist abilist in
      let indexed = index_abilist parsed in
      let pinned_versions = process_group arch_abilist_location indexed syms overrides in
      (arch_abilist_location, guard, pinned_versions)
    in
    let arch_with_pin = List.map process_architecture architectures in
    (filename, include_fragment, arch_with_pin)
  in
  let processed = List.map process_lib libs in
  for_list processed (fun (bare_filename, include_fragment, arch_with_pin) ->
      let version_filename = Printf.sprintf "__%s-versions.h" bare_filename in
      let ver_hdr_path = Printf.sprintf "%s/%s" dest_dir version_filename in
      let wrapper_path = Printf.sprintf "%s/%s.h" dest_dir bare_filename in
      print_versions_to_file arch_with_pin ver_hdr_path;
      print_wrapper_to_file wrapper_path include_fragment version_filename);
  ()

;;

run ()
