(module
  (type (;0;) (func))
  (func $__wasm_call_ctors (type 0))
  (func $main (type 0)
    (local i32 i32 i32 i32)
    global.get 0
    local.set 0
    i32.const 16
    local.set 1
    local.get 0
    local.get 1
    i32.sub
    local.set 2
    i32.const 8
    local.set 3
    local.get 2
    local.get 3
    i32.store offset=12
    return)
  (table (;0;) 1 1 funcref)
  (memory (;0;) 2)
  (global (;0;) (mut i32) (i32.const 66560))
  (global (;1;) i32 (i32.const 1024))
  (global (;2;) i32 (i32.const 1024))
  (global (;3;) i32 (i32.const 1024))
  (global (;4;) i32 (i32.const 66560))
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
  (export "__table_base" (global 6)))
