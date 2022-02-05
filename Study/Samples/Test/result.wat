(module
  (type (;0;) (func))
  (type (;1;) (func (param i64 i32) (result i64)))
  (func $__wasm_call_ctors (type 0))
  (func $foo (type 1) (param i64 i32) (result i64)
    (local i32 i32 i32 i64 i32 i32 i64 i64 i64)
    global.get 0
    local.set 2
    i32.const 32
    local.set 3
    local.get 2
    local.get 3
    i32.sub
    local.set 4
    local.get 4
    local.get 0
    i64.store offset=24
    local.get 4
    local.get 1
    i32.store offset=20
    local.get 4
    i64.load offset=24
    local.set 5
    local.get 4
    i32.load offset=20
    local.set 6
    local.get 6
    local.set 7
    local.get 7
    i64.extend_i32_s
    local.set 8
    local.get 5
    local.get 8
    i64.add
    local.set 9
    local.get 4
    local.get 9
    i64.store offset=8
    local.get 4
    i64.load offset=8
    local.set 10
    local.get 10
    return)
  (table (;0;) 1 1 funcref)
  (memory (;0;) 1)
  (global (;0;) (mut i32) (i32.const 11024))
  (global (;1;) i32 (i32.const 1024))
  (global (;2;) i32 (i32.const 1024))
  (global (;3;) i32 (i32.const 1024))
  (global (;4;) i32 (i32.const 11024))
  (global (;5;) i32 (i32.const 0))
  (global (;6;) i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "foo" (func $foo))
  (export "__dso_handle" (global 1))
  (export "__data_end" (global 2))
  (export "__global_base" (global 3))
  (export "__heap_base" (global 4))
  (export "__memory_base" (global 5))
  (export "__table_base" (global 6)))
