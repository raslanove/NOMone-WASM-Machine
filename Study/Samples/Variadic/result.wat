(module
  (type (;0;) (func))
  (type (;1;) (func (param i32 i32)))
  (func $__wasm_call_ctors (type 0))
  (func $variadicFunction (type 1) (param i32 i32)
    global.get 0
    i32.const 16
    i32.sub
    local.get 1
    i32.const 4
    i32.add
    i32.store offset=12
    local.get 1
    i32.load
    i32.const 123
    i32.store)
  (func $main (type 0)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 0
    global.set 0
    local.get 0
    local.get 0
    i32.const 12
    i32.add
    i32.store
    local.get 0
    local.get 0
    call $variadicFunction
    local.get 0
    i32.const 16
    i32.add
    global.set 0)
  (table (;0;) 1 1 funcref)
  (memory (;0;) 1)
  (global (;0;) (mut i32) (i32.const 1024))
  (global (;1;) i32 (i32.const 1024))
  (global (;2;) i32 (i32.const 1024))
  (global (;3;) i32 (i32.const 1024))
  (global (;4;) i32 (i32.const 1024))
  (global (;5;) i32 (i32.const 0))
  (global (;6;) i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "variadicFunction" (func $variadicFunction))
  (export "main" (func $main))
  (export "__dso_handle" (global 1))
  (export "__data_end" (global 2))
  (export "__global_base" (global 3))
  (export "__heap_base" (global 4))
  (export "__memory_base" (global 5))
  (export "__table_base" (global 6)))
