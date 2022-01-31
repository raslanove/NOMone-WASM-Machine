(module
  (type (;0;) (func))
  (func $__wasm_call_ctors (type 0))
  (func $main (type 0)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i64 i64)
    global.get 0
    local.set 0
    i32.const 32
    local.set 1
    local.get 0
    local.get 1
    i32.sub
    local.set 2
    local.get 2
    local.set 3
    i32.const 16
    local.set 4
    local.get 3
    local.get 4
    i32.add
    local.set 5
    i32.const 0
    local.set 6
    local.get 6
    i32.load8_u offset=1040
    local.set 7
    local.get 5
    local.get 7
    i32.store8
    i32.const 8
    local.set 8
    local.get 3
    local.get 8
    i32.add
    local.set 9
    local.get 6
    i64.load offset=1032
    local.set 10
    local.get 9
    local.get 10
    i64.store
    local.get 6
    i64.load offset=1024
    local.set 11
    local.get 3
    local.get 11
    i64.store
    return)
  (table (;0;) 1 1 funcref)
  (memory (;0;) 2)
  (global (;0;) (mut i32) (i32.const 66592))
  (global (;1;) i32 (i32.const 1024))
  (global (;2;) i32 (i32.const 1041))
  (global (;3;) i32 (i32.const 1024))
  (global (;4;) i32 (i32.const 66592))
  (global (;5;) i32 (i32.const 0))
  (global (;6;) i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "main" (func $main))
  (export "__dso_handle" (global 1))
  (export "__data_end" (global 2))
  (export "__global_base" (global 3))
  (export "__heap_base" (global 4))
  (export "__memory_base" (global 5))
  (export "__table_base" (global 6))
  (data (;0;) (i32.const 1024) "besm Allah \5c\5c :)\00"))
