(module
  (type (;0;) (func))
  (type (;1;) (func (param i32 i32 i32) (result i32)))
  (type (;2;) (func (param i32 i32)))
  (type (;3;) (func (param i32 i32) (result i32)))
  (type (;4;) (func (param i32)))
  (type (;5;) (func (param f64) (result i32)))
  (type (;6;) (func (param i32) (result i32)))
  (type (;7;) (func (param i32 i32 i32)))
  (type (;8;) (func (result i32)))
  (type (;9;) (func (param i32 f64 i32)))
  (type (;10;) (func (param i32 i32 i32 i32 i32) (result i32)))
  (func $__wasm_call_ctors (type 0))
  (func $initialize (type 4) (param i32)
    block  ;; label = @1
      local.get 0
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      call_indirect (type 0)
    end)
  (func $terminate (type 0))
  (func $strlen (type 6) (param i32) (result i32)
    (local i32 i32 i32)
    i32.const 0
    local.set 1
    loop  ;; label = @1
      local.get 0
      local.get 1
      i32.add
      local.set 2
      local.get 1
      i32.const 1
      i32.add
      local.tee 3
      local.set 1
      local.get 2
      i32.load8_u
      br_if 0 (;@1;)
    end
    local.get 3
    i32.const -1
    i32.add)
  (func $startsWith (type 3) (param i32 i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        local.get 0
        i32.load8_u
        local.tee 2
        br_if 0 (;@2;)
        i32.const 0
        local.set 3
        br 1 (;@1;)
      end
      local.get 0
      i32.const 1
      i32.add
      local.set 4
      i32.const 0
      local.set 0
      loop  ;; label = @2
        block  ;; label = @3
          local.get 1
          local.get 0
          i32.add
          i32.load8_u
          local.tee 3
          br_if 0 (;@3;)
          local.get 0
          local.set 3
          br 2 (;@1;)
        end
        block  ;; label = @3
          local.get 2
          i32.const 255
          i32.and
          local.get 3
          i32.eq
          br_if 0 (;@3;)
          i32.const 0
          return
        end
        local.get 4
        local.get 0
        i32.add
        local.set 2
        local.get 0
        i32.const 1
        i32.add
        local.tee 3
        local.set 0
        local.get 2
        i32.load8_u
        local.tee 2
        br_if 0 (;@2;)
      end
    end
    local.get 1
    local.get 3
    i32.add
    i32.load8_u
    i32.eqz)
  (func $equals (type 3) (param i32 i32) (result i32)
    (local i32 i32 i32)
    local.get 1
    i32.load8_u
    local.tee 2
    i32.eqz
    local.set 3
    block  ;; label = @1
      local.get 0
      i32.load8_u
      local.tee 4
      i32.eqz
      br_if 0 (;@1;)
      local.get 1
      i32.const 1
      i32.add
      local.set 1
      local.get 0
      i32.const 1
      i32.add
      local.set 0
      block  ;; label = @2
        loop  ;; label = @3
          local.get 3
          i32.const 1
          i32.and
          br_if 1 (;@2;)
          local.get 4
          i32.const 255
          i32.and
          local.get 2
          i32.const 255
          i32.and
          i32.ne
          br_if 1 (;@2;)
          local.get 0
          i32.load8_u
          local.set 4
          local.get 1
          i32.load8_u
          local.tee 2
          i32.eqz
          local.set 3
          local.get 1
          i32.const 1
          i32.add
          local.set 1
          local.get 0
          i32.const 1
          i32.add
          local.set 0
          local.get 4
          i32.eqz
          br_if 2 (;@1;)
          br 0 (;@3;)
        end
      end
      i32.const 0
      return
    end
    local.get 3)
  (func $copy (type 3) (param i32 i32) (result i32)
    (local i32 i32)
    i32.const 0
    local.set 2
    loop  ;; label = @1
      local.get 0
      local.get 2
      i32.add
      local.get 1
      local.get 2
      i32.add
      i32.load8_u
      local.tee 3
      i32.store8
      local.get 2
      i32.const 1
      i32.add
      local.set 2
      local.get 3
      br_if 0 (;@1;)
    end
    local.get 0)
  (func $parseInteger (type 6) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 48
    i32.sub
    local.tee 1
    global.set 0
    i32.const -1
    local.set 2
    loop  ;; label = @1
      local.get 0
      local.get 2
      i32.add
      local.set 3
      local.get 2
      i32.const 1
      i32.add
      local.tee 4
      local.set 2
      local.get 3
      i32.const 1
      i32.add
      i32.load8_u
      br_if 0 (;@1;)
    end
    block  ;; label = @1
      block  ;; label = @2
        local.get 4
        i32.const 12
        i32.lt_u
        br_if 0 (;@2;)
        local.get 1
        local.get 0
        i32.store offset=4
        i32.const 0
        local.set 2
        local.get 1
        i32.const 0
        i32.load offset=2676
        i32.store
        i32.const 1052
        i32.const 1076
        local.get 1
        i32.const 0
        i32.load offset=1524
        call_indirect (type 1)
        drop
        br 1 (;@1;)
      end
      i32.const 0
      local.set 2
      local.get 4
      local.get 0
      i32.load8_u
      i32.const 45
      i32.eq
      local.tee 5
      i32.sub
      i32.eqz
      br_if 0 (;@1;)
      i32.const -1
      i32.const 0
      local.get 5
      select
      local.set 3
      block  ;; label = @2
        block  ;; label = @3
          local.get 5
          br_if 0 (;@3;)
          local.get 3
          local.get 4
          i32.add
          local.set 6
          local.get 0
          local.get 3
          local.get 5
          i32.add
          i32.add
          local.get 4
          i32.add
          i32.const -1
          i32.add
          local.set 3
          i32.const 1
          local.set 4
          i32.const 0
          local.set 2
          loop  ;; label = @4
            local.get 3
            i32.load8_u
            i32.const -48
            i32.add
            i32.const 255
            i32.and
            local.tee 5
            i32.const 10
            i32.ge_u
            br_if 2 (;@2;)
            local.get 3
            i32.const -1
            i32.add
            local.set 3
            local.get 4
            local.get 5
            i32.mul
            local.get 2
            i32.add
            local.set 2
            local.get 4
            i32.const 10
            i32.mul
            local.set 4
            local.get 6
            i32.const -1
            i32.add
            local.tee 6
            br_if 0 (;@4;)
            br 3 (;@1;)
          end
        end
        local.get 0
        local.get 5
        i32.add
        local.get 4
        i32.add
        local.set 7
        i32.const 0
        local.set 2
        i32.const -1
        local.set 6
        loop  ;; label = @3
          local.get 7
          local.get 3
          i32.add
          i32.const -1
          i32.add
          i32.load8_u
          i32.const -48
          i32.add
          i32.const 255
          i32.and
          local.tee 5
          i32.const 9
          i32.gt_u
          br_if 1 (;@2;)
          block  ;; label = @4
            local.get 6
            local.get 5
            i32.mul
            local.tee 5
            i32.const 0
            i32.gt_s
            br_if 0 (;@4;)
            local.get 5
            local.get 2
            i32.add
            local.set 2
            local.get 6
            i32.const 10
            i32.mul
            local.set 6
            local.get 4
            local.get 3
            i32.add
            local.set 5
            local.get 3
            i32.const -1
            i32.add
            local.set 3
            local.get 5
            i32.const 1
            i32.ne
            br_if 1 (;@3;)
            br 3 (;@1;)
          end
        end
        local.get 1
        local.get 0
        i32.store offset=36
        i32.const 0
        local.set 2
        local.get 1
        i32.const 0
        i32.load offset=2676
        i32.store offset=32
        i32.const 1052
        i32.const 1187
        local.get 1
        i32.const 32
        i32.add
        i32.const 0
        i32.load offset=1524
        call_indirect (type 1)
        drop
        br 1 (;@1;)
      end
      local.get 1
      local.get 0
      i32.store offset=20
      i32.const 0
      local.set 2
      local.get 1
      i32.const 0
      i32.load offset=2676
      i32.store offset=16
      i32.const 1052
      i32.const 1138
      local.get 1
      i32.const 16
      i32.add
      i32.const 0
      i32.load offset=1524
      call_indirect (type 1)
      drop
    end
    local.get 1
    i32.const 48
    i32.add
    global.set 0
    local.get 2)
  (func $jump (type 3) (param i32 i32) (result i32)
    block  ;; label = @1
      loop  ;; label = @2
        local.get 0
        local.get 1
        i32.ge_s
        br_if 1 (;@1;)
        local.get 0
        i32.const 1
        i32.shl
        local.set 0
        br 0 (;@2;)
      end
    end
    local.get 0)
  (func $getTime (type 4) (param i32)
    local.get 0
    local.get 0
    i32.const 8
    i32.add
    i32.const 0
    i32.load offset=1968
    call_indirect (type 2))
  (func $initialize.1 (type 6) (param i32) (result i32)
    i32.const 4
    local.get 0
    i32.const 0
    i32.load offset=1424
    call_indirect (type 3)
    drop
    local.get 0
    i32.const 0
    i32.const 0
    i32.load offset=1440
    call_indirect (type 3)
    drop
    local.get 0)
  (func $destroy (type 4) (param i32)
    local.get 0
    i32.const 0
    i32.load offset=1432
    call_indirect (type 4))
  (func $destroyAndFree (type 4) (param i32)
    local.get 0
    i32.const 0
    i32.load offset=1432
    call_indirect (type 4)
    local.get 0
    i32.const 0
    i32.load offset=1944
    call_indirect (type 4))
  (func $vAppend (type 1) (param i32 i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i64 i64 f64)
    global.get 0
    i32.const 96
    i32.sub
    local.tee 3
    global.set 0
    local.get 0
    local.get 3
    i32.const 95
    i32.add
    i32.const 0
    i32.load offset=1444
    call_indirect (type 3)
    drop
    local.get 3
    i32.const 64
    i32.add
    i32.const -2
    i32.add
    local.set 4
    i32.const 0
    i32.load offset=1976
    local.set 5
    i32.const 0
    i32.load offset=1972
    local.set 6
    i32.const 0
    i32.load offset=1440
    local.set 7
    i32.const 0
    local.set 8
    loop (result i32)  ;; label = @1
      local.get 8
      i32.const 1
      i32.add
      local.set 9
      block  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            local.get 1
            local.get 8
            i32.add
            i32.load8_u
            local.tee 10
            i32.const 37
            i32.eq
            br_if 0 (;@4;)
            local.get 10
            i32.eqz
            br_if 1 (;@3;)
            br 2 (;@2;)
          end
          local.get 8
          i32.const 2
          i32.add
          local.set 11
          i32.const 37
          local.set 10
          block  ;; label = @4
            local.get 1
            local.get 9
            i32.add
            i32.load8_s
            local.tee 9
            i32.const 37
            i32.ne
            br_if 0 (;@4;)
            local.get 11
            local.set 9
            br 2 (;@2;)
          end
          block  ;; label = @4
            local.get 9
            i32.const -99
            i32.add
            local.tee 10
            i32.const 16
            i32.gt_u
            br_if 0 (;@4;)
            block  ;; label = @5
              block  ;; label = @6
                block  ;; label = @7
                  block  ;; label = @8
                    block  ;; label = @9
                      local.get 10
                      br_table 1 (;@8;) 2 (;@7;) 5 (;@4;) 4 (;@5;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 3 (;@6;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 5 (;@4;) 0 (;@9;) 1 (;@8;)
                    end
                    local.get 2
                    i32.load
                    local.set 9
                    local.get 11
                    local.set 8
                    local.get 2
                    i32.const 4
                    i32.add
                    local.tee 12
                    local.set 2
                    local.get 9
                    i32.load8_u
                    local.tee 10
                    i32.eqz
                    br_if 7 (;@1;)
                    local.get 9
                    i32.const 1
                    i32.add
                    local.set 8
                    loop  ;; label = @9
                      local.get 0
                      local.get 10
                      i32.const 24
                      i32.shl
                      i32.const 24
                      i32.shr_s
                      local.get 7
                      call_indirect (type 3)
                      drop
                      local.get 8
                      i32.load8_u
                      local.set 10
                      local.get 8
                      i32.const 1
                      i32.add
                      local.set 8
                      local.get 10
                      br_if 0 (;@9;)
                    end
                    local.get 11
                    local.set 8
                    local.get 12
                    local.set 2
                    br 7 (;@1;)
                  end
                  local.get 0
                  local.get 2
                  i32.load8_s
                  local.get 7
                  call_indirect (type 3)
                  drop
                  local.get 2
                  i32.const 4
                  i32.add
                  local.set 2
                  local.get 11
                  local.set 8
                  br 6 (;@1;)
                end
                block  ;; label = @7
                  local.get 2
                  i32.load
                  local.tee 8
                  i32.const -1
                  i32.gt_s
                  br_if 0 (;@7;)
                  local.get 0
                  i32.const 45
                  local.get 7
                  call_indirect (type 3)
                  drop
                  i32.const 0
                  local.get 8
                  i32.sub
                  local.set 8
                end
                local.get 2
                i32.const 4
                i32.add
                local.set 2
                i32.const 0
                local.set 10
                i32.const 1
                local.set 12
                loop  ;; label = @7
                  local.get 3
                  i32.const 64
                  i32.add
                  local.get 10
                  i32.add
                  local.get 8
                  local.get 8
                  i32.const 10
                  i32.div_u
                  local.tee 13
                  i32.const 10
                  i32.mul
                  i32.sub
                  local.tee 14
                  i32.store8
                  local.get 12
                  local.tee 9
                  i32.const 1
                  i32.add
                  local.set 12
                  local.get 10
                  i32.const 1
                  i32.add
                  local.set 10
                  local.get 8
                  i32.const 9
                  i32.gt_u
                  local.set 15
                  local.get 13
                  local.set 8
                  local.get 15
                  br_if 0 (;@7;)
                end
                local.get 0
                local.get 14
                i32.const 24
                i32.shl
                i32.const 24
                i32.shr_s
                i32.const 48
                i32.or
                local.get 7
                call_indirect (type 3)
                drop
                block  ;; label = @7
                  local.get 10
                  i32.const 1
                  i32.eq
                  br_if 0 (;@7;)
                  loop  ;; label = @8
                    local.get 0
                    local.get 4
                    local.get 9
                    i32.add
                    i32.load8_u
                    i32.const 24
                    i32.shl
                    i32.const 805306368
                    i32.add
                    i32.const 24
                    i32.shr_s
                    local.get 7
                    call_indirect (type 3)
                    drop
                    local.get 9
                    i32.const -1
                    i32.add
                    local.tee 9
                    i32.const 1
                    i32.ne
                    br_if 0 (;@8;)
                  end
                end
                local.get 11
                local.set 8
                br 5 (;@1;)
              end
              block  ;; label = @6
                local.get 1
                local.get 11
                i32.add
                i32.load8_u
                i32.const 100
                i32.eq
                br_if 0 (;@6;)
                local.get 3
                i32.const 52
                i32.add
                local.get 1
                i32.store
                local.get 3
                i32.const 48
                i32.add
                i32.const 0
                i32.load offset=2676
                local.tee 8
                i32.store
                local.get 3
                i32.const 0
                i32.load offset=2664
                local.tee 10
                i32.store offset=44
                local.get 3
                local.get 8
                i32.store offset=40
                local.get 3
                local.get 10
                i32.store offset=36
                local.get 3
                local.get 8
                i32.store offset=32
                i32.const 1288
                i32.const 1304
                local.get 3
                i32.const 32
                i32.add
                i32.const 0
                i32.load offset=1524
                call_indirect (type 1)
                drop
                br 3 (;@3;)
              end
              block  ;; label = @6
                local.get 2
                i32.const 7
                i32.add
                i32.const -8
                i32.and
                local.tee 10
                i64.load
                local.tee 16
                i64.const -1
                i64.gt_s
                br_if 0 (;@6;)
                local.get 0
                i32.const 45
                local.get 7
                call_indirect (type 3)
                drop
                i64.const 0
                local.get 16
                i64.sub
                local.set 16
              end
              local.get 8
              i32.const 3
              i32.add
              local.set 8
              local.get 10
              i32.const 8
              i32.add
              local.set 2
              i32.const 0
              local.set 10
              i32.const 1
              local.set 11
              loop  ;; label = @6
                local.get 3
                i32.const 64
                i32.add
                local.get 10
                i32.add
                local.get 16
                local.get 16
                i64.const 10
                i64.div_u
                local.tee 17
                i64.const 10
                i64.mul
                i64.sub
                i32.wrap_i64
                local.tee 13
                i32.store8
                local.get 11
                local.tee 9
                i32.const 1
                i32.add
                local.set 11
                local.get 10
                i32.const 1
                i32.add
                local.set 10
                local.get 16
                i64.const 9
                i64.gt_u
                local.set 12
                local.get 17
                local.set 16
                local.get 12
                br_if 0 (;@6;)
              end
              local.get 0
              local.get 13
              i32.const 24
              i32.shl
              i32.const 24
              i32.shr_s
              i32.const 48
              i32.or
              local.get 7
              call_indirect (type 3)
              drop
              local.get 10
              i32.const 1
              i32.eq
              br_if 4 (;@1;)
              loop  ;; label = @6
                local.get 0
                local.get 4
                local.get 9
                i32.add
                i32.load8_u
                i32.const 24
                i32.shl
                i32.const 805306368
                i32.add
                i32.const 24
                i32.shr_s
                local.get 7
                call_indirect (type 3)
                drop
                local.get 9
                i32.const -1
                i32.add
                local.tee 9
                i32.const 1
                i32.ne
                br_if 0 (;@6;)
                br 5 (;@1;)
              end
            end
            local.get 2
            i32.const 7
            i32.add
            i32.const -8
            i32.and
            local.tee 8
            i32.const 8
            i32.add
            local.set 2
            block  ;; label = @5
              local.get 8
              f64.load
              local.tee 18
              local.get 6
              call_indirect (type 5)
              i32.eqz
              br_if 0 (;@5;)
              local.get 0
              i32.const 110
              local.get 7
              call_indirect (type 3)
              drop
              local.get 0
              i32.const 97
              local.get 7
              call_indirect (type 3)
              drop
              local.get 0
              i32.const 110
              local.get 7
              call_indirect (type 3)
              drop
              local.get 11
              local.set 8
              br 4 (;@1;)
            end
            block  ;; label = @5
              local.get 18
              f64.const 0x0p+0 (;=0;)
              f64.lt
              i32.const 1
              i32.xor
              br_if 0 (;@5;)
              local.get 0
              i32.const 45
              local.get 7
              call_indirect (type 3)
              drop
              f64.const 0x0p+0 (;=0;)
              local.get 18
              f64.sub
              local.set 18
            end
            block  ;; label = @5
              local.get 18
              local.get 5
              call_indirect (type 5)
              i32.eqz
              br_if 0 (;@5;)
              local.get 0
              i32.const 105
              local.get 7
              call_indirect (type 3)
              drop
              local.get 0
              i32.const 110
              local.get 7
              call_indirect (type 3)
              drop
              local.get 0
              i32.const 102
              local.get 7
              call_indirect (type 3)
              drop
              local.get 11
              local.set 8
              br 4 (;@1;)
            end
            block  ;; label = @5
              block  ;; label = @6
                local.get 18
                f64.const 0x1.158e46p+63 (;=1e+19;)
                f64.ge
                i32.const 1
                i32.xor
                br_if 0 (;@6;)
                local.get 18
                f64.const 0x1.54fdd7f73bf3cp+850 (;=1e+256;)
                f64.ge
                local.tee 8
                i32.const 8
                i32.shl
                local.tee 10
                i32.const 128
                i32.or
                local.get 10
                local.get 18
                f64.const 0x1.8062864ac6f43p-851 (;=1e-256;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.27748f9301d32p+425 (;=1e+128;)
                f64.ge
                local.tee 8
                select
                local.tee 10
                i32.const 64
                i32.or
                local.get 10
                local.get 18
                f64.const 0x1.bba08cf8c979dp-426 (;=1e-128;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.84f03e93ff9f5p+212 (;=1e+64;)
                f64.ge
                local.tee 8
                select
                local.tee 10
                i32.const 32
                i32.or
                local.get 10
                local.get 18
                f64.const 0x1.50ffd44f4a73dp-213 (;=1e-64;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.3b8b5b5056e17p+106 (;=1e+32;)
                f64.ge
                local.tee 8
                select
                local.tee 10
                i32.const 16
                i32.add
                local.get 10
                local.get 18
                f64.const 0x1.9f623d5a8a733p-107 (;=1e-32;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.1c37937e08p+53 (;=1e+16;)
                f64.ge
                local.tee 8
                select
                local.tee 10
                i32.const 8
                i32.add
                local.get 10
                local.get 18
                f64.const 0x1.cd2b297d889bcp-54 (;=1e-16;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.7d784p+26 (;=1e+08;)
                f64.ge
                local.tee 8
                select
                local.tee 10
                i32.const 4
                i32.add
                local.get 10
                local.get 18
                f64.const 0x1.5798ee2308c3ap-27 (;=1e-08;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.388p+13 (;=10000;)
                f64.ge
                local.tee 8
                select
                local.tee 10
                i32.const 2
                i32.add
                local.get 10
                local.get 18
                f64.const 0x1.a36e2eb1c432dp-14 (;=0.0001;)
                f64.mul
                local.get 18
                local.get 8
                select
                local.tee 18
                f64.const 0x1.9p+6 (;=100;)
                f64.ge
                local.tee 9
                select
                local.set 8
                local.get 18
                f64.const 0x1.47ae147ae147bp-7 (;=0.01;)
                f64.mul
                local.get 18
                local.get 9
                select
                local.tee 18
                f64.const 0x1.4p+3 (;=10;)
                f64.ge
                i32.const 1
                i32.xor
                br_if 1 (;@5;)
                local.get 18
                f64.const 0x1.999999999999ap-4 (;=0.1;)
                f64.mul
                local.set 18
                local.get 8
                i32.const 1
                i32.add
                local.set 8
                br 1 (;@5;)
              end
              i32.const 0
              local.set 8
              local.get 18
              f64.const 0x1.4f8b58p-17 (;=1e-05;)
              f64.le
              i32.const 1
              i32.xor
              br_if 0 (;@5;)
              i32.const -256
              i32.const 0
              local.get 18
              f64.const 0x1.8062864ac6f43p-851 (;=1e-256;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -128
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.54fdd7f73bf3cp+850 (;=1e+256;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.bba08cf8c979dp-426 (;=1e-128;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -64
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.27748f9301d32p+425 (;=1e+128;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.50ffd44f4a73dp-213 (;=1e-64;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -32
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.84f03e93ff9f5p+212 (;=1e+64;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.039d66589688p-103 (;=1e-31;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -16
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.3b8b5b5056e17p+106 (;=1e+32;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.203af9ee75616p-50 (;=1e-15;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -8
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.1c37937e08p+53 (;=1e+16;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.ad7f29abcaf48p-24 (;=1e-07;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -4
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.7d784p+26 (;=1e+08;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.0624dd2f1a9fcp-10 (;=0.001;)
              f64.lt
              local.tee 8
              select
              local.tee 10
              i32.const -2
              i32.add
              local.get 10
              local.get 18
              f64.const 0x1.388p+13 (;=10000;)
              f64.mul
              local.get 18
              local.get 8
              select
              local.tee 18
              f64.const 0x1.999999999999ap-4 (;=0.1;)
              f64.lt
              local.tee 9
              select
              local.set 8
              local.get 18
              f64.const 0x1.9p+6 (;=100;)
              f64.mul
              local.get 18
              local.get 9
              select
              local.tee 18
              f64.const 0x1p+0 (;=1;)
              f64.lt
              i32.const 1
              i32.xor
              br_if 0 (;@5;)
              local.get 18
              f64.const 0x1.4p+3 (;=10;)
              f64.mul
              local.set 18
              local.get 8
              i32.const -1
              i32.add
              local.set 8
            end
            local.get 0
            local.get 18
            local.get 8
            call $appendDoubleToByteVector_HandleTextFormatting
            local.get 11
            local.set 8
            br 3 (;@1;)
          end
          local.get 3
          i32.const 16
          i32.add
          local.get 1
          i32.store
          local.get 3
          local.get 9
          i32.store offset=4
          local.get 3
          i32.const 0
          i32.load offset=2676
          local.tee 8
          i32.store offset=12
          local.get 3
          i32.const 0
          i32.load offset=2664
          i32.store offset=8
          local.get 3
          local.get 8
          i32.store
          i32.const 1288
          i32.const 1360
          local.get 3
          i32.const 0
          i32.load offset=1524
          call_indirect (type 1)
          drop
        end
        local.get 0
        i32.const 0
        local.get 7
        call_indirect (type 3)
        drop
        local.get 3
        i32.const 96
        i32.add
        global.set 0
        local.get 0
        return
      end
      local.get 0
      local.get 10
      i32.const 24
      i32.shl
      i32.const 24
      i32.shr_s
      local.get 7
      call_indirect (type 3)
      drop
      local.get 9
      local.set 8
      br 0 (;@1;)
    end)
  (func $appendDoubleToByteVector_HandleTextFormatting (type 9) (param i32 f64 i32)
    (local i32 i32 i64 i64 i64 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    local.set 4
    local.get 3
    global.set 0
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        f64.const 0x1p+64 (;=1.84467e+19;)
        f64.lt
        local.get 1
        f64.const 0x0p+0 (;=0;)
        f64.ge
        i32.and
        i32.eqz
        br_if 0 (;@2;)
        local.get 1
        i64.trunc_f64_u
        local.set 5
        br 1 (;@1;)
      end
      i64.const 0
      local.set 5
    end
    block  ;; label = @1
      block  ;; label = @2
        local.get 1
        local.get 5
        f64.convert_i64_u
        f64.sub
        f64.const 0x1.7d784p+26 (;=1e+08;)
        f64.mul
        local.tee 1
        f64.const 0x1p+64 (;=1.84467e+19;)
        f64.lt
        local.get 1
        f64.const 0x0p+0 (;=0;)
        f64.ge
        i32.and
        i32.eqz
        br_if 0 (;@2;)
        local.get 1
        i64.trunc_f64_u
        local.set 6
        br 1 (;@1;)
      end
      i64.const 0
      local.set 6
    end
    block  ;; label = @1
      block  ;; label = @2
        local.get 6
        i64.const 10
        i64.rem_u
        local.tee 7
        i32.wrap_i64
        i32.const 5
        i32.lt_u
        br_if 0 (;@2;)
        i64.const 0
        local.get 6
        local.get 7
        i64.sub
        i64.const 10
        i64.add
        local.tee 6
        local.get 6
        i64.const 100000000
        i64.eq
        local.tee 8
        select
        local.set 6
        local.get 8
        i64.extend_i32_u
        local.get 5
        i64.add
        local.set 5
        br 1 (;@1;)
      end
      local.get 6
      local.get 7
      i64.sub
      local.set 6
    end
    local.get 3
    local.set 9
    local.get 3
    i32.const -32
    i32.add
    local.tee 10
    global.set 0
    i32.const 0
    local.set 3
    loop  ;; label = @1
      local.get 10
      local.get 3
      i32.add
      local.get 5
      local.get 5
      i64.const 10
      i64.div_u
      local.tee 7
      i64.const 10
      i64.mul
      i64.sub
      i32.wrap_i64
      local.tee 11
      i32.store8
      local.get 3
      i32.const 1
      i32.add
      local.set 3
      local.get 5
      i64.const 9
      i64.gt_u
      local.set 8
      local.get 7
      local.set 5
      local.get 8
      br_if 0 (;@1;)
    end
    local.get 0
    local.get 11
    i32.const 24
    i32.shl
    i32.const 24
    i32.shr_s
    i32.const 48
    i32.or
    i32.const 0
    i32.load offset=1440
    local.tee 11
    call_indirect (type 3)
    drop
    block  ;; label = @1
      local.get 3
      i32.const 1
      i32.eq
      br_if 0 (;@1;)
      local.get 10
      i32.const -2
      i32.add
      local.set 8
      loop  ;; label = @2
        local.get 0
        local.get 8
        local.get 3
        i32.add
        i32.load8_u
        i32.const 24
        i32.shl
        i32.const 805306368
        i32.add
        i32.const 24
        i32.shr_s
        local.get 11
        call_indirect (type 3)
        drop
        local.get 3
        i32.const -1
        i32.add
        local.tee 3
        i32.const 1
        i32.ne
        br_if 0 (;@2;)
      end
    end
    local.get 9
    local.set 3
    block  ;; label = @1
      local.get 6
      i64.eqz
      br_if 0 (;@1;)
      local.get 0
      i32.const 46
      local.get 11
      call_indirect (type 3)
      drop
      local.get 3
      local.set 12
      local.get 3
      local.tee 3
      i32.const -16
      i32.add
      local.tee 10
      global.set 0
      local.get 3
      i32.const -12
      i32.add
      i32.const 0
      i32.store align=1
      local.get 3
      i32.const -15
      i32.add
      i32.const 0
      i32.store align=1
      local.get 10
      local.set 3
      loop  ;; label = @2
        local.get 3
        local.get 6
        local.get 6
        i64.const 10
        i64.div_u
        local.tee 5
        i64.const 10
        i64.mul
        i64.sub
        i64.store8
        local.get 3
        i32.const 1
        i32.add
        local.set 3
        local.get 6
        i64.const 9
        i64.gt_u
        local.set 8
        local.get 5
        local.set 6
        local.get 8
        br_if 0 (;@2;)
      end
      i32.const 0
      local.set 3
      loop  ;; label = @2
        local.get 10
        local.get 3
        i32.add
        local.set 8
        local.get 3
        i32.const 1
        i32.add
        local.tee 9
        local.set 3
        local.get 8
        i32.load8_u
        i32.eqz
        br_if 0 (;@2;)
      end
      i32.const 9
      local.set 3
      block  ;; label = @2
        local.get 9
        i32.const 9
        i32.eq
        br_if 0 (;@2;)
        loop  ;; label = @3
          local.get 0
          local.get 10
          local.get 3
          i32.add
          i32.const -2
          i32.add
          i32.load8_u
          i32.const 24
          i32.shl
          i32.const 805306368
          i32.add
          i32.const 24
          i32.shr_s
          local.get 11
          call_indirect (type 3)
          drop
          local.get 9
          local.get 3
          i32.const -1
          i32.add
          local.tee 3
          i32.ne
          br_if 0 (;@3;)
        end
      end
      local.get 12
      drop
    end
    block  ;; label = @1
      local.get 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      i32.const 101
      local.get 11
      call_indirect (type 3)
      drop
      block  ;; label = @2
        local.get 2
        i32.const -1
        i32.gt_s
        br_if 0 (;@2;)
        local.get 0
        i32.const 45
        local.get 11
        call_indirect (type 3)
        drop
        i32.const 0
        local.get 2
        i32.sub
        local.set 2
      end
      local.get 2
      i64.extend_i32_u
      local.set 5
      i32.const 0
      local.set 3
      loop  ;; label = @2
        local.get 4
        i32.const 13
        i32.add
        local.get 3
        i32.add
        local.get 5
        local.get 5
        i64.const 10
        i64.div_u
        local.tee 6
        i64.const 10
        i64.mul
        i64.sub
        i32.wrap_i64
        local.tee 10
        i32.store8
        local.get 3
        i32.const 1
        i32.add
        local.set 3
        local.get 5
        i64.const 9
        i64.gt_u
        local.set 8
        local.get 6
        local.set 5
        local.get 8
        br_if 0 (;@2;)
      end
      local.get 0
      local.get 10
      i32.const 24
      i32.shl
      i32.const 24
      i32.shr_s
      i32.const 48
      i32.or
      local.get 11
      call_indirect (type 3)
      drop
      local.get 3
      i32.const 1
      i32.eq
      br_if 0 (;@1;)
      local.get 4
      i32.const 11
      i32.add
      local.set 8
      loop  ;; label = @2
        local.get 0
        local.get 8
        local.get 3
        i32.add
        i32.load8_u
        i32.const 24
        i32.shl
        i32.const 805306368
        i32.add
        i32.const 24
        i32.shr_s
        local.get 11
        call_indirect (type 3)
        drop
        local.get 3
        i32.const -1
        i32.add
        local.tee 3
        i32.const 1
        i32.ne
        br_if 0 (;@2;)
      end
    end
    local.get 4
    i32.const 16
    i32.add
    global.set 0)
  (func $append (type 1) (param i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    global.set 0
    local.get 3
    local.get 2
    i32.store offset=12
    local.get 0
    local.get 1
    local.get 2
    call $vAppend
    local.set 2
    local.get 3
    i32.const 16
    i32.add
    global.set 0
    local.get 2)
  (func $set (type 1) (param i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    global.set 0
    local.get 0
    i32.const 0
    i32.load offset=1436
    call_indirect (type 6)
    drop
    local.get 0
    i32.const 0
    i32.const 0
    i32.load offset=1440
    call_indirect (type 3)
    drop
    local.get 3
    local.get 2
    i32.store offset=12
    local.get 0
    local.get 1
    local.get 2
    call $vAppend
    local.set 0
    local.get 3
    i32.const 16
    i32.add
    global.set 0
    local.get 0)
  (func $get (type 6) (param i32) (result i32)
    local.get 0
    i32.load offset=8)
  (func $create (type 3) (param i32 i32) (result i32)
    (local i32 i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 2
    global.set 0
    i32.const 4
    i32.const 12
    i32.const 0
    i32.load offset=1940
    call_indirect (type 6)
    local.tee 3
    i32.const 0
    i32.load offset=1424
    call_indirect (type 3)
    drop
    local.get 3
    i32.const 0
    i32.const 0
    i32.load offset=1440
    call_indirect (type 3)
    drop
    local.get 2
    local.get 1
    i32.store offset=12
    local.get 3
    local.get 0
    local.get 1
    call $vAppend
    local.set 1
    local.get 2
    i32.const 16
    i32.add
    global.set 0
    local.get 1)
  (func $replace (type 1) (param i32 i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 32
    i32.sub
    local.tee 3
    global.set 0
    i32.const 0
    local.set 4
    i32.const 1415
    i32.const 0
    call $create
    local.set 5
    block  ;; label = @1
      local.get 0
      i32.load8_u
      local.tee 6
      i32.eqz
      br_if 0 (;@1;)
      local.get 1
      i32.const 1
      i32.add
      local.set 7
      loop  ;; label = @2
        block  ;; label = @3
          block  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                local.get 1
                i32.load8_u
                local.tee 8
                br_if 0 (;@6;)
                i32.const 0
                local.set 9
                br 1 (;@5;)
              end
              local.get 0
              local.get 4
              i32.add
              local.set 10
              i32.const 0
              local.set 11
              loop  ;; label = @6
                local.get 10
                local.get 11
                i32.add
                i32.load8_u
                local.tee 9
                i32.eqz
                br_if 2 (;@4;)
                local.get 8
                i32.const 255
                i32.and
                local.get 9
                i32.ne
                br_if 2 (;@4;)
                local.get 7
                local.get 11
                i32.add
                local.set 8
                local.get 11
                i32.const 1
                i32.add
                local.tee 9
                local.set 11
                local.get 8
                i32.load8_u
                local.tee 8
                br_if 0 (;@6;)
              end
            end
            local.get 3
            local.get 2
            i32.store
            local.get 5
            i32.const 1416
            local.get 3
            call $append
            drop
            local.get 9
            local.get 4
            i32.add
            local.set 4
            br 1 (;@3;)
          end
          local.get 3
          local.get 6
          i32.const 24
          i32.shl
          i32.const 24
          i32.shr_s
          i32.store offset=16
          local.get 5
          i32.const 1419
          local.get 3
          i32.const 16
          i32.add
          call $append
          drop
          local.get 4
          i32.const 1
          i32.add
          local.set 4
        end
        local.get 0
        local.get 4
        i32.add
        i32.load8_u
        local.tee 6
        br_if 0 (;@2;)
      end
    end
    local.get 3
    i32.const 32
    i32.add
    global.set 0
    local.get 5)
  (func $length (type 6) (param i32) (result i32)
    local.get 0
    i32.load offset=4
    i32.const -1
    i32.add)
  (func $create.1 (type 3) (param i32 i32) (result i32)
    local.get 1
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 1
    i32.const 0
    i32.store offset=4
    local.get 1
    i32.const 4
    local.get 0
    local.get 0
    i32.const -1
    i32.add
    i32.const 3
    i32.lt_u
    select
    local.tee 0
    i32.store
    block  ;; label = @1
      local.get 0
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 1
      local.get 0
      i32.const 0
      i32.load offset=1940
      call_indirect (type 6)
      i32.store offset=8
    end
    local.get 1)
  (func $createInHeap (type 6) (param i32) (result i32)
    (local i32 i32)
    i32.const 12
    i32.const 0
    i32.load offset=1940
    local.tee 1
    call_indirect (type 6)
    local.tee 2
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 2
    i32.const 0
    i32.store offset=4
    local.get 2
    i32.const 4
    local.get 0
    local.get 0
    i32.const -1
    i32.add
    i32.const 3
    i32.lt_u
    select
    local.tee 0
    i32.store
    block  ;; label = @1
      local.get 0
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 2
      local.get 0
      local.get 1
      call_indirect (type 6)
      i32.store offset=8
    end
    local.get 2)
  (func $destroy.1 (type 4) (param i32)
    local.get 0
    i32.load offset=8
    i32.const 0
    i32.load offset=1944
    call_indirect (type 4)
    local.get 0
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop)
  (func $clear (type 6) (param i32) (result i32)
    local.get 0
    i32.const 0
    i32.store offset=4
    local.get 0)
  (func $pushBack (type 3) (param i32 i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          local.get 0
          i32.load offset=4
          local.tee 2
          local.get 0
          i32.load
          i32.eq
          br_if 0 (;@3;)
          local.get 0
          i32.load offset=8
          local.set 2
          br 1 (;@2;)
        end
        i32.const 0
        local.set 3
        i32.const 0
        i32.load offset=1940
        local.set 4
        block  ;; label = @3
          local.get 2
          br_if 0 (;@3;)
          local.get 0
          i32.const 4
          local.get 4
          call_indirect (type 6)
          local.tee 2
          i32.store offset=8
          local.get 2
          i32.eqz
          br_if 2 (;@1;)
          local.get 0
          i32.const 4
          i32.store
          br 1 (;@2;)
        end
        local.get 2
        i32.const 1
        i32.shl
        local.get 4
        call_indirect (type 6)
        local.tee 2
        i32.eqz
        br_if 1 (;@1;)
        local.get 2
        local.get 0
        i32.load offset=8
        local.get 0
        i32.load
        i32.const 0
        i32.load offset=1952
        call_indirect (type 1)
        drop
        local.get 0
        i32.load offset=8
        i32.const 0
        i32.load offset=1944
        call_indirect (type 4)
        local.get 0
        local.get 2
        i32.store offset=8
        local.get 0
        local.get 0
        i32.load
        i32.const 1
        i32.shl
        i32.store
      end
      i32.const 1
      local.set 3
      local.get 0
      local.get 0
      i32.load offset=4
      local.tee 4
      i32.const 1
      i32.add
      i32.store offset=4
      local.get 2
      local.get 4
      i32.add
      local.get 1
      i32.store8
    end
    local.get 3)
  (func $popBack (type 3) (param i32 i32) (result i32)
    (local i32)
    block  ;; label = @1
      local.get 0
      i32.load offset=4
      local.tee 2
      br_if 0 (;@1;)
      i32.const 0
      return
    end
    local.get 0
    local.get 2
    i32.const -1
    i32.add
    local.tee 2
    i32.store offset=4
    local.get 1
    local.get 0
    i32.load offset=8
    local.get 2
    i32.add
    i32.load8_u
    i32.store8
    i32.const 1)
  (func $pushBack32Bit (type 3) (param i32 i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          local.get 0
          i32.load offset=4
          i32.const 3
          i32.add
          local.get 0
          i32.load
          local.tee 2
          i32.ge_s
          br_if 0 (;@3;)
          local.get 0
          i32.load offset=8
          local.set 2
          br 1 (;@2;)
        end
        i32.const 0
        local.set 3
        i32.const 0
        i32.load offset=1940
        local.set 4
        block  ;; label = @3
          local.get 2
          br_if 0 (;@3;)
          local.get 0
          i32.const 4
          local.get 4
          call_indirect (type 6)
          local.tee 2
          i32.store offset=8
          local.get 2
          i32.eqz
          br_if 2 (;@1;)
          local.get 0
          i32.const 4
          i32.store
          br 1 (;@2;)
        end
        local.get 2
        i32.const 1
        i32.shl
        local.get 4
        call_indirect (type 6)
        local.tee 2
        i32.eqz
        br_if 1 (;@1;)
        local.get 2
        local.get 0
        i32.load offset=8
        local.get 0
        i32.load
        i32.const 0
        i32.load offset=1952
        call_indirect (type 1)
        drop
        local.get 0
        i32.load offset=8
        i32.const 0
        i32.load offset=1944
        call_indirect (type 4)
        local.get 0
        local.get 2
        i32.store offset=8
        local.get 0
        local.get 0
        i32.load
        i32.const 1
        i32.shl
        i32.store
      end
      local.get 2
      local.get 0
      i32.load offset=4
      i32.add
      local.get 1
      i32.store
      local.get 0
      local.get 0
      i32.load offset=4
      i32.const 4
      i32.add
      i32.store offset=4
      i32.const 1
      local.set 3
    end
    local.get 3)
  (func $popBack32Bit (type 3) (param i32 i32) (result i32)
    (local i32 i32)
    i32.const 0
    local.set 2
    block  ;; label = @1
      local.get 0
      i32.load offset=4
      local.tee 3
      i32.const 4
      i32.lt_s
      br_if 0 (;@1;)
      local.get 0
      local.get 3
      i32.const -4
      i32.add
      local.tee 2
      i32.store offset=4
      local.get 1
      local.get 0
      i32.load offset=8
      local.get 2
      i32.add
      i32.load
      i32.store
      i32.const 1
      local.set 2
    end
    local.get 2)
  (func $get.1 (type 3) (param i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 2
    global.set 0
    block  ;; label = @1
      block  ;; label = @2
        local.get 0
        i32.load offset=4
        local.get 1
        i32.gt_s
        br_if 0 (;@2;)
        local.get 2
        local.get 1
        i32.store
        i32.const 0
        local.set 1
        i32.const 1468
        i32.const 1486
        local.get 2
        i32.const 0
        i32.load offset=1524
        call_indirect (type 1)
        drop
        br 1 (;@1;)
      end
      local.get 0
      i32.load offset=8
      local.get 1
      i32.add
      i32.load8_u
      local.set 1
    end
    local.get 2
    i32.const 16
    i32.add
    global.set 0
    local.get 1
    i32.const 24
    i32.shl
    i32.const 24
    i32.shr_s)
  (func $set.1 (type 1) (param i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    global.set 0
    block  ;; label = @1
      block  ;; label = @2
        local.get 0
        i32.load offset=4
        local.get 1
        i32.gt_s
        br_if 0 (;@2;)
        local.get 3
        local.get 1
        i32.store
        i32.const 0
        local.set 1
        i32.const 1468
        i32.const 1486
        local.get 3
        i32.const 0
        i32.load offset=1524
        call_indirect (type 1)
        drop
        br 1 (;@1;)
      end
      local.get 0
      i32.load offset=8
      local.get 1
      i32.add
      local.get 2
      i32.store8
      i32.const 1
      local.set 1
    end
    local.get 3
    i32.const 16
    i32.add
    global.set 0
    local.get 1)
  (func $size (type 6) (param i32) (result i32)
    local.get 0
    i32.load offset=4)
  (func $pushError (type 1) (param i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    global.set 0
    local.get 3
    local.get 2
    i32.store offset=12
    local.get 0
    local.get 1
    local.get 2
    call $vPushError
    local.set 2
    local.get 3
    i32.const 16
    i32.add
    global.set 0
    local.get 2)
  (func $vPushError (type 1) (param i32 i32 i32) (result i32)
    (local i32 i32 i32)
    i32.const 0
    local.set 3
    block  ;; label = @1
      i32.const 0
      i32.load offset=2724
      br_if 0 (;@1;)
      i32.const 0
      i32.const 0
      i32.const 336
      i32.const 0
      i32.load offset=1692
      call_indirect (type 3)
      i32.store offset=2724
    end
    i32.const 1544
    i32.const 0
    i32.const 0
    i32.load offset=1276
    call_indirect (type 3)
    local.tee 4
    local.get 1
    local.get 2
    i32.const 0
    i32.load offset=1260
    call_indirect (type 1)
    drop
    i32.const 0
    i32.load offset=2724
    i32.const 0
    i32.load offset=1704
    call_indirect (type 6)
    local.set 2
    block  ;; label = @1
      local.get 0
      i32.eqz
      br_if 0 (;@1;)
      i32.const 0
      local.set 3
      loop  ;; label = @2
        local.get 0
        local.get 3
        i32.add
        i32.load8_u
        local.tee 1
        i32.eqz
        br_if 1 (;@1;)
        local.get 2
        local.get 3
        i32.add
        local.get 1
        i32.store8
        local.get 3
        i32.const 1
        i32.add
        local.tee 3
        i32.const 64
        i32.ne
        br_if 0 (;@2;)
      end
      i32.const 1545
      i32.const 1552
      i32.const 0
      call $pushAndPrintError
      drop
      i32.const 63
      local.set 3
    end
    local.get 2
    local.get 3
    i32.add
    i32.const 0
    i32.store8
    local.get 2
    i32.const 64
    i32.add
    local.set 1
    local.get 4
    i32.const 0
    i32.load offset=1284
    call_indirect (type 6)
    local.set 3
    local.get 4
    i32.const 0
    i32.load offset=1272
    call_indirect (type 6)
    local.set 0
    i32.const 0
    i32.load offset=1952
    local.set 5
    block  ;; label = @1
      block  ;; label = @2
        local.get 3
        i32.const 256
        i32.lt_s
        br_if 0 (;@2;)
        local.get 1
        local.get 0
        i32.const 255
        local.get 5
        call_indirect (type 1)
        drop
        local.get 2
        i32.const 0
        i32.store8 offset=319
        i32.const 1545
        i32.const 1586
        i32.const 0
        call $pushAndPrintError
        drop
        br 1 (;@1;)
      end
      local.get 1
      local.get 0
      local.get 3
      i32.const 1
      i32.add
      local.get 5
      call_indirect (type 1)
      drop
    end
    local.get 4
    i32.const 0
    i32.load offset=1256
    call_indirect (type 4)
    local.get 2
    i32.const 320
    i32.add
    i32.const 0
    i32.load offset=1244
    call_indirect (type 4)
    local.get 2)
  (func $pushAndPrintError (type 1) (param i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    global.set 0
    local.get 3
    local.get 2
    i32.store offset=12
    local.get 3
    local.get 0
    local.get 1
    local.get 2
    call $vPushError
    local.tee 2
    i32.const 64
    i32.add
    i32.store
    local.get 0
    i32.const 1624
    local.get 3
    i32.const 0
    i32.load offset=1964
    call_indirect (type 7)
    local.get 3
    i32.const 16
    i32.add
    global.set 0
    local.get 2)
  (func $popErrors (type 6) (param i32) (result i32)
    (local i32 i32 i32 i32 i32)
    i32.const 0
    local.set 1
    block  ;; label = @1
      i32.const 0
      i32.load offset=2724
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      i32.const 0
      local.set 1
      local.get 2
      i32.const 0
      i32.load offset=1720
      call_indirect (type 6)
      local.get 0
      i32.sub
      local.tee 2
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 2
      i32.const 336
      i32.const 0
      i32.load offset=1692
      call_indirect (type 3)
      local.set 1
      i32.const 0
      i32.load offset=1716
      local.set 3
      i32.const 0
      i32.load offset=1712
      local.set 4
      i32.const 0
      i32.load offset=1704
      local.set 5
      i32.const 0
      local.set 0
      loop  ;; label = @2
        local.get 1
        local.get 5
        call_indirect (type 6)
        drop
        i32.const 0
        i32.load offset=2724
        local.get 1
        local.get 0
        local.get 3
        call_indirect (type 3)
        local.get 4
        call_indirect (type 3)
        drop
        local.get 2
        local.get 0
        i32.const 1
        i32.add
        local.tee 0
        i32.ne
        br_if 0 (;@2;)
      end
    end
    local.get 1)
  (func $destroyAndFreeErrors (type 4) (param i32)
    local.get 0
    i32.const 0
    i32.load offset=1696
    call_indirect (type 4)
    local.get 0
    i32.const 0
    i32.load offset=1944
    call_indirect (type 4))
  (func $popDestroyAndFreeErrors (type 6) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32)
    i32.const 0
    local.set 1
    block  ;; label = @1
      i32.const 0
      i32.load offset=2724
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      i32.const 0
      local.set 1
      local.get 2
      i32.const 0
      i32.load offset=1720
      local.tee 3
      call_indirect (type 6)
      local.get 0
      i32.sub
      local.tee 2
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 2
      i32.const 336
      i32.const 0
      i32.load offset=1692
      call_indirect (type 3)
      local.set 0
      i32.const 0
      i32.load offset=1716
      local.set 4
      i32.const 0
      i32.load offset=1712
      local.set 5
      i32.const 0
      i32.load offset=1704
      local.set 6
      i32.const 0
      local.set 1
      loop  ;; label = @2
        local.get 0
        local.get 6
        call_indirect (type 6)
        drop
        i32.const 0
        i32.load offset=2724
        local.get 0
        local.get 1
        local.get 4
        call_indirect (type 3)
        local.get 5
        call_indirect (type 3)
        drop
        local.get 2
        local.get 1
        i32.const 1
        i32.add
        local.tee 1
        i32.ne
        br_if 0 (;@2;)
      end
      block  ;; label = @2
        local.get 0
        br_if 0 (;@2;)
        i32.const 0
        return
      end
      local.get 0
      local.get 3
      call_indirect (type 6)
      local.set 1
      local.get 0
      i32.const 0
      i32.load offset=1696
      call_indirect (type 4)
      local.get 0
      i32.const 0
      i32.load offset=1944
      call_indirect (type 4)
    end
    local.get 1)
  (func $terminate.1 (type 0)
    (local i32)
    block  ;; label = @1
      i32.const 0
      i32.load offset=2724
      local.tee 0
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      i32.const 0
      i32.load offset=1700
      call_indirect (type 4)
      i32.const 0
      i32.const 0
      i32.store offset=2724
    end)
  (func $observeErrors (type 8) (result i32)
    (local i32 i32)
    i32.const 0
    local.set 0
    block  ;; label = @1
      i32.const 0
      i32.load offset=2724
      local.tee 1
      i32.eqz
      br_if 0 (;@1;)
      local.get 1
      i32.const 0
      i32.load offset=1720
      call_indirect (type 6)
      local.set 0
    end
    local.get 0)
  (func $logAndTerminate (type 8) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 384
    i32.sub
    local.tee 0
    global.set 0
    i32.const 0
    local.set 1
    block  ;; label = @1
      i32.const 0
      i32.load offset=2724
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      i32.const 0
      local.set 1
      block  ;; label = @2
        local.get 2
        i32.const 0
        i32.load offset=1720
        local.tee 3
        call_indirect (type 6)
        local.tee 4
        i32.const 1
        i32.lt_s
        br_if 0 (;@2;)
        local.get 4
        i32.const 336
        i32.const 0
        i32.load offset=1692
        call_indirect (type 3)
        local.set 5
        i32.const 0
        i32.load offset=1716
        local.set 1
        i32.const 0
        i32.load offset=1712
        local.set 6
        i32.const 0
        i32.load offset=1704
        local.set 7
        i32.const 0
        local.set 2
        loop  ;; label = @3
          local.get 5
          local.get 7
          call_indirect (type 6)
          drop
          i32.const 0
          i32.load offset=2724
          local.get 5
          local.get 2
          local.get 1
          call_indirect (type 3)
          local.get 6
          call_indirect (type 3)
          drop
          local.get 4
          local.get 2
          i32.const 1
          i32.add
          local.tee 2
          i32.ne
          br_if 0 (;@3;)
        end
        block  ;; label = @3
          local.get 5
          br_if 0 (;@3;)
          i32.const 0
          local.set 1
          br 1 (;@2;)
        end
        local.get 0
        local.get 5
        local.get 3
        call_indirect (type 6)
        local.tee 1
        i32.store offset=36
        local.get 0
        i32.const 0
        i32.load offset=2676
        i32.store offset=32
        i32.const 1627
        i32.const 1644
        local.get 0
        i32.const 32
        i32.add
        i32.const 0
        i32.load offset=1960
        local.tee 2
        call_indirect (type 7)
        block  ;; label = @3
          local.get 5
          local.get 0
          i32.const 48
          i32.add
          local.get 6
          call_indirect (type 3)
          i32.eqz
          br_if 0 (;@3;)
          local.get 0
          i32.const 112
          i32.add
          local.set 4
          loop  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                local.get 0
                i32.load8_u offset=48
                i32.eqz
                br_if 0 (;@6;)
                local.get 0
                local.get 4
                i32.store offset=20
                local.get 0
                local.get 0
                i32.const 48
                i32.add
                i32.store offset=16
                i32.const 0
                i32.const 1673
                local.get 0
                i32.const 16
                i32.add
                local.get 2
                call_indirect (type 7)
                br 1 (;@5;)
              end
              local.get 0
              local.get 4
              i32.store
              i32.const 0
              i32.const 1682
              local.get 0
              local.get 2
              call_indirect (type 7)
            end
            local.get 5
            local.get 0
            i32.const 48
            i32.add
            local.get 6
            call_indirect (type 3)
            br_if 0 (;@4;)
          end
        end
        local.get 5
        i32.const 0
        i32.load offset=1696
        call_indirect (type 4)
        local.get 5
        i32.const 0
        i32.load offset=1944
        call_indirect (type 4)
      end
      i32.const 0
      i32.load offset=2724
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 2
      i32.const 0
      i32.load offset=1700
      call_indirect (type 4)
      i32.const 0
      i32.const 0
      i32.store offset=2724
    end
    local.get 0
    i32.const 384
    i32.add
    global.set 0
    local.get 1)
  (func $create.2 (type 1) (param i32 i32 i32) (result i32)
    block  ;; label = @1
      local.get 1
      br_if 0 (;@1;)
      i32.const 0
      return
    end
    local.get 2
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 2
    i32.const 0
    i32.store offset=8
    local.get 2
    local.get 1
    i32.store offset=4
    local.get 2
    local.get 0
    i32.store
    block  ;; label = @1
      local.get 0
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 2
      local.get 1
      local.get 0
      i32.mul
      i32.const 0
      i32.load offset=1940
      call_indirect (type 6)
      i32.store offset=12
    end
    local.get 2)
  (func $createInHeap.1 (type 3) (param i32 i32) (result i32)
    (local i32 i32)
    block  ;; label = @1
      local.get 1
      br_if 0 (;@1;)
      i32.const 0
      return
    end
    i32.const 16
    i32.const 0
    i32.load offset=1940
    local.tee 2
    call_indirect (type 6)
    local.tee 3
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 3
    i32.const 0
    i32.store offset=8
    local.get 3
    local.get 1
    i32.store offset=4
    local.get 3
    local.get 0
    i32.store
    block  ;; label = @1
      local.get 0
      i32.const 1
      i32.lt_s
      br_if 0 (;@1;)
      local.get 3
      local.get 1
      local.get 0
      i32.mul
      local.get 2
      call_indirect (type 6)
      i32.store offset=12
    end
    local.get 3)
  (func $destroy.2 (type 4) (param i32)
    local.get 0
    i32.load offset=12
    i32.const 0
    i32.load offset=1944
    call_indirect (type 4)
    local.get 0
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop)
  (func $destroyAndFree.1 (type 4) (param i32)
    (local i32)
    local.get 0
    i32.load offset=12
    i32.const 0
    i32.load offset=1944
    local.tee 1
    call_indirect (type 4)
    local.get 0
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 0
    local.get 1
    call_indirect (type 4))
  (func $emplaceBack (type 6) (param i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          local.get 0
          i32.load offset=8
          local.tee 1
          local.get 0
          i32.load
          i32.eq
          br_if 0 (;@3;)
          local.get 0
          i32.load offset=12
          local.set 1
          br 1 (;@2;)
        end
        block  ;; label = @3
          local.get 1
          br_if 0 (;@3;)
          i32.const 0
          local.set 2
          local.get 0
          local.get 0
          i32.load offset=4
          i32.const 0
          i32.load offset=1940
          call_indirect (type 6)
          local.tee 1
          i32.store offset=12
          local.get 1
          i32.eqz
          br_if 2 (;@1;)
          local.get 0
          i32.const 1
          i32.store
          br 1 (;@2;)
        end
        i32.const 0
        local.set 2
        local.get 0
        i32.load offset=4
        local.get 1
        i32.mul
        local.tee 3
        i32.const 1
        i32.shl
        i32.const 0
        i32.load offset=1940
        call_indirect (type 6)
        local.tee 1
        i32.eqz
        br_if 1 (;@1;)
        local.get 1
        local.get 0
        i32.load offset=12
        local.get 3
        i32.const 0
        i32.load offset=1952
        call_indirect (type 1)
        drop
        local.get 0
        i32.load offset=12
        i32.const 0
        i32.load offset=1944
        call_indirect (type 4)
        local.get 0
        local.get 1
        i32.store offset=12
        local.get 0
        local.get 0
        i32.load
        i32.const 1
        i32.shl
        i32.store
      end
      local.get 0
      local.get 0
      i32.load offset=8
      local.tee 2
      i32.const 1
      i32.add
      i32.store offset=8
      local.get 2
      local.get 0
      i32.load offset=4
      i32.mul
      local.get 1
      i32.add
      local.set 2
    end
    local.get 2)
  (func $pushBack.1 (type 3) (param i32 i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          local.get 0
          i32.load offset=8
          local.tee 2
          local.get 0
          i32.load
          i32.eq
          br_if 0 (;@3;)
          local.get 0
          i32.load offset=12
          local.set 2
          br 1 (;@2;)
        end
        block  ;; label = @3
          local.get 2
          br_if 0 (;@3;)
          i32.const 0
          local.set 3
          local.get 0
          local.get 0
          i32.load offset=4
          i32.const 0
          i32.load offset=1940
          call_indirect (type 6)
          local.tee 2
          i32.store offset=12
          local.get 2
          i32.eqz
          br_if 2 (;@1;)
          local.get 0
          i32.const 1
          i32.store
          br 1 (;@2;)
        end
        i32.const 0
        local.set 3
        local.get 0
        i32.load offset=4
        local.get 2
        i32.mul
        local.tee 4
        i32.const 1
        i32.shl
        i32.const 0
        i32.load offset=1940
        call_indirect (type 6)
        local.tee 2
        i32.eqz
        br_if 1 (;@1;)
        local.get 2
        local.get 0
        i32.load offset=12
        local.get 4
        i32.const 0
        i32.load offset=1952
        call_indirect (type 1)
        drop
        local.get 0
        i32.load offset=12
        i32.const 0
        i32.load offset=1944
        call_indirect (type 4)
        local.get 0
        local.get 2
        i32.store offset=12
        local.get 0
        local.get 0
        i32.load
        i32.const 1
        i32.shl
        i32.store
      end
      local.get 0
      i32.load offset=4
      local.tee 3
      local.get 0
      i32.load offset=8
      i32.mul
      local.get 2
      i32.add
      local.get 1
      local.get 3
      i32.const 0
      i32.load offset=1952
      call_indirect (type 1)
      drop
      i32.const 1
      local.set 3
      local.get 0
      local.get 0
      i32.load offset=8
      i32.const 1
      i32.add
      i32.store offset=8
    end
    local.get 3)
  (func $popBack.1 (type 3) (param i32 i32) (result i32)
    (local i32 i32)
    block  ;; label = @1
      local.get 0
      i32.load offset=8
      local.tee 2
      br_if 0 (;@1;)
      i32.const 0
      return
    end
    local.get 0
    local.get 2
    i32.const -1
    i32.add
    local.tee 2
    i32.store offset=8
    local.get 1
    local.get 0
    i32.load offset=4
    local.tee 3
    local.get 2
    i32.mul
    local.get 0
    i32.load offset=12
    i32.add
    local.get 3
    i32.const 0
    i32.load offset=1952
    call_indirect (type 1)
    drop
    i32.const 1)
  (func $get.2 (type 3) (param i32 i32) (result i32)
    (local i32)
    i32.const 0
    local.set 2
    block  ;; label = @1
      local.get 0
      i32.load offset=8
      local.get 1
      i32.le_s
      br_if 0 (;@1;)
      local.get 0
      i32.load offset=4
      local.get 1
      i32.mul
      local.get 0
      i32.load offset=12
      i32.add
      local.set 2
    end
    local.get 2)
  (func $size.1 (type 6) (param i32) (result i32)
    local.get 0
    i32.load offset=8)
  (func $NWM_createReferenceWasmMachine (type 6) (param i32) (result i32)
    local.get 0
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 0
    i32.const 49
    i32.store offset=8
    local.get 0
    i32.const 50
    i32.store offset=4
    local.get 0
    i32.const 1
    i32.store
    local.get 0)
  (func $parseWatCode (type 3) (param i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 544
    i32.sub
    local.tee 2
    global.set 0
    i32.const 0
    local.set 3
    local.get 2
    i32.const 0
    i32.store offset=28
    block  ;; label = @1
      block  ;; label = @2
        i32.const 1724
        local.get 1
        local.get 2
        i32.const 28
        i32.add
        i32.const 1755
        local.get 2
        i32.const 32
        i32.add
        call $getToken
        br_if 0 (;@2;)
        local.get 2
        i32.const 0
        i32.store offset=28
        br 1 (;@1;)
      end
      block  ;; label = @2
        block  ;; label = @3
          i32.const 1724
          local.get 1
          local.get 2
          i32.const 28
          i32.add
          i32.const 1757
          local.get 2
          i32.const 32
          i32.add
          call $getToken
          i32.eqz
          br_if 0 (;@3;)
          local.get 2
          i32.load offset=28
          local.set 4
          i32.const 0
          local.set 3
          i32.const 1724
          local.get 1
          local.get 2
          i32.const 28
          i32.add
          i32.const 0
          local.get 2
          i32.const 32
          i32.add
          call $getToken
          i32.eqz
          br_if 1 (;@2;)
          i32.const 0
          i32.load offset=1516
          local.set 5
          i32.const 0
          i32.load offset=1040
          local.set 6
          loop  ;; label = @4
            block  ;; label = @5
              local.get 2
              i32.const 32
              i32.add
              i32.const 1764
              local.get 6
              call_indirect (type 3)
              i32.eqz
              br_if 0 (;@5;)
              i32.const 1
              local.set 3
              br 4 (;@1;)
            end
            block  ;; label = @5
              local.get 2
              i32.const 32
              i32.add
              i32.const 1755
              local.get 6
              call_indirect (type 3)
              i32.eqz
              br_if 0 (;@5;)
              local.get 5
              call_indirect (type 8)
              local.set 7
              local.get 2
              local.get 4
              i32.store offset=28
              block  ;; label = @6
                block  ;; label = @7
                  i32.const 1904
                  local.get 1
                  local.get 2
                  i32.const 28
                  i32.add
                  i32.const 1755
                  local.get 2
                  i32.const 288
                  i32.add
                  call $getToken
                  i32.eqz
                  br_if 0 (;@7;)
                  i32.const 1904
                  local.get 1
                  local.get 2
                  i32.const 28
                  i32.add
                  i32.const 1933
                  local.get 2
                  i32.const 288
                  i32.add
                  call $getToken
                  i32.eqz
                  br_if 0 (;@7;)
                  i32.const 1904
                  local.get 1
                  local.get 2
                  i32.const 28
                  i32.add
                  i32.const 1755
                  local.get 2
                  i32.const 288
                  i32.add
                  call $getToken
                  i32.eqz
                  br_if 0 (;@7;)
                  i32.const 1904
                  local.get 1
                  local.get 2
                  i32.const 28
                  i32.add
                  i32.const 1938
                  local.get 2
                  i32.const 288
                  i32.add
                  call $getToken
                  i32.eqz
                  br_if 0 (;@7;)
                  i32.const 1904
                  local.get 1
                  local.get 2
                  i32.const 28
                  i32.add
                  i32.const 0
                  local.get 2
                  i32.const 288
                  i32.add
                  call $getToken
                  br_if 1 (;@6;)
                end
                local.get 2
                local.get 4
                i32.store offset=28
                local.get 5
                call_indirect (type 8)
                local.get 7
                i32.le_s
                br_if 0 (;@6;)
                i32.const 0
                local.set 3
                br 5 (;@1;)
              end
              local.get 2
              i32.load offset=28
              local.set 4
              i32.const 1724
              local.get 1
              local.get 2
              i32.const 28
              i32.add
              i32.const 0
              local.get 2
              i32.const 32
              i32.add
              call $getToken
              br_if 1 (;@4;)
              br 3 (;@2;)
            end
          end
          i32.const 0
          local.set 3
          local.get 2
          i32.const 24
          i32.add
          i32.const 0
          i32.load offset=2664
          local.tee 1
          i32.store
          local.get 2
          i32.const 16
          i32.add
          i32.const 0
          i32.load offset=2676
          local.tee 4
          i32.store
          local.get 2
          i32.const 20
          i32.add
          local.get 2
          i32.const 32
          i32.add
          i32.store
          local.get 2
          local.get 1
          i32.store offset=12
          local.get 2
          local.get 4
          i32.store offset=8
          local.get 2
          local.get 1
          i32.store offset=4
          local.get 2
          local.get 4
          i32.store
          i32.const 1724
          i32.const 1766
          local.get 2
          i32.const 0
          i32.load offset=1524
          call_indirect (type 1)
          drop
          br 2 (;@1;)
        end
        i32.const 0
        local.set 3
      end
      local.get 2
      local.get 3
      i32.store offset=28
    end
    local.get 2
    i32.const 544
    i32.add
    global.set 0
    local.get 3)
  (func $NWM_destroyReferenceMachine (type 4) (param i32)
    local.get 0
    i32.const 0
    i32.store)
  (func $getToken (type 10) (param i32 i32 i32 i32 i32) (result i32)
    (local i32 i32 i32 i32)
    global.get 0
    i32.const 64
    i32.sub
    local.tee 5
    global.set 0
    local.get 2
    i32.load
    local.set 6
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          loop  ;; label = @4
            local.get 1
            local.get 6
            i32.add
            i32.load8_u
            local.tee 7
            i32.const 59
            i32.gt_u
            br_if 2 (;@2;)
            block  ;; label = @5
              block  ;; label = @6
                local.get 7
                br_table 1 (;@5;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 0 (;@6;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 3 (;@3;) 3 (;@3;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 3 (;@3;) 1 (;@5;)
              end
              local.get 2
              local.get 6
              i32.const 1
              i32.add
              local.tee 6
              i32.store
              br 1 (;@4;)
            end
          end
          local.get 4
          i32.const 1812
          i32.const 0
          i32.load offset=1044
          call_indirect (type 3)
          drop
          i32.const 1
          local.set 7
          br 2 (;@1;)
        end
        local.get 4
        i32.const 0
        i32.store8 offset=1
        local.get 4
        local.get 7
        i32.store8
        i32.const 1
        local.set 7
        local.get 2
        local.get 2
        i32.load
        i32.const 1
        i32.add
        i32.store
        br 1 (;@1;)
      end
      i32.const 0
      local.set 6
      block  ;; label = @2
        block  ;; label = @3
          loop  ;; label = @4
            block  ;; label = @5
              local.get 7
              i32.const 255
              i32.and
              local.tee 8
              i32.const 59
              i32.gt_u
              br_if 0 (;@5;)
              local.get 8
              br_table 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 2 (;@3;)
            end
            local.get 4
            local.get 6
            i32.add
            local.get 7
            i32.store8
            block  ;; label = @5
              local.get 6
              i32.const 255
              i32.ne
              br_if 0 (;@5;)
              local.get 5
              local.get 4
              i32.store offset=48
              i32.const 0
              local.set 7
              local.get 0
              i32.const 1816
              local.get 5
              i32.const 48
              i32.add
              i32.const 0
              i32.load offset=1524
              call_indirect (type 1)
              drop
              i32.const 256
              local.set 6
              br 3 (;@2;)
            end
            local.get 6
            i32.const 1
            i32.add
            local.set 6
            local.get 2
            local.get 2
            i32.load
            i32.const 1
            i32.add
            local.tee 7
            i32.store
            local.get 1
            local.get 7
            i32.add
            i32.load8_u
            local.set 7
            br 0 (;@4;)
          end
        end
        i32.const 1
        local.set 7
      end
      local.get 4
      local.get 6
      i32.add
      i32.const 0
      i32.store8
    end
    block  ;; label = @1
      block  ;; label = @2
        local.get 3
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 6
        local.get 4
        local.get 3
        i32.const 0
        i32.load offset=1040
        call_indirect (type 3)
        br_if 0 (;@2;)
        local.get 5
        i32.const 36
        i32.add
        i32.const 0
        i32.load offset=2664
        local.tee 7
        i32.store
        local.get 5
        i32.const 32
        i32.add
        local.get 4
        i32.store
        local.get 5
        local.get 3
        i32.store offset=20
        local.get 5
        i32.const 0
        i32.load offset=2676
        local.tee 2
        i32.store offset=28
        local.get 5
        local.get 7
        i32.store offset=24
        local.get 5
        local.get 2
        i32.store offset=16
        local.get 0
        i32.const 1850
        local.get 5
        i32.const 16
        i32.add
        i32.const 0
        i32.load offset=1524
        call_indirect (type 1)
        drop
        br 1 (;@1;)
      end
      block  ;; label = @2
        local.get 7
        br_if 0 (;@2;)
        i32.const 0
        local.set 6
        br 1 (;@1;)
      end
      local.get 5
      local.get 4
      i32.store offset=4
      local.get 5
      i32.const 0
      i32.load offset=2676
      i32.store
      local.get 0
      i32.const 1886
      local.get 5
      i32.const 0
      i32.load offset=1956
      call_indirect (type 7)
      i32.const 1
      local.set 6
    end
    local.get 5
    i32.const 64
    i32.add
    global.set 0
    local.get 6)
  (func $NWM_createReferenceWasmMachineInHeap (type 8) (result i32)
    (local i32)
    i32.const 12
    i32.const 0
    i32.load offset=1940
    call_indirect (type 6)
    local.tee 0
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1948
    call_indirect (type 1)
    drop
    local.get 0
    i32.const 49
    i32.store offset=8
    local.get 0
    i32.const 50
    i32.store offset=4
    local.get 0
    i32.const 1
    i32.store
    local.get 0)
  (func $main (type 0)
    i32.const 51
    i32.const 0
    i32.load offset=1024
    call_indirect (type 4)
    i32.const 0
    i32.load offset=1028
    call_indirect (type 0))
  (func $nMalloc (type 6) (param i32) (result i32)
    i32.const 0)
  (func $nFree (type 4) (param i32))
  (func $nMemset (type 1) (param i32 i32 i32) (result i32)
    i32.const 0)
  (func $nMemcpy (type 1) (param i32 i32 i32) (result i32)
    i32.const 0)
  (func $nLogI (type 7) (param i32 i32 i32))
  (func $nLogW (type 7) (param i32 i32 i32))
  (func $nLogE (type 7) (param i32 i32 i32))
  (func $getTime.1 (type 2) (param i32 i32))
  (func $isNaN (type 5) (param f64) (result i32)
    i32.const 0)
  (func $isInf (type 5) (param f64) (result i32)
    i32.const 0)
  (func $NMain (type 0)
    (local i32 i32 i32)
    global.get 0
    i32.const 32
    i32.sub
    local.tee 0
    global.set 0
    local.get 0
    i32.const 24
    i32.add
    i32.const 0
    i32.load8_u offset=2692
    i32.store8
    local.get 0
    i32.const 0
    i64.load offset=2684 align=1
    i64.store offset=16
    i32.const 0
    i32.load offset=1240
    call_indirect (type 8)
    local.tee 1
    local.get 0
    i32.const 16
    i32.add
    local.get 1
    i32.load offset=8
    call_indirect (type 3)
    local.set 2
    local.get 1
    local.get 1
    i32.load offset=4
    call_indirect (type 4)
    local.get 1
    i32.const 0
    i32.load offset=1944
    call_indirect (type 4)
    local.get 0
    i32.const 2712
    i32.const 2717
    local.get 2
    i32.const 1
    i32.eq
    select
    i32.store
    i32.const 2693
    i32.const 2708
    local.get 0
    i32.const 0
    i32.load offset=1956
    call_indirect (type 7)
    i32.const 0
    i32.load offset=1540
    call_indirect (type 8)
    drop
    local.get 0
    i32.const 32
    i32.add
    global.set 0)
  (table (;0;) 62 62 funcref)
  (memory (;0;) 2)
  (global (;0;) (mut i32) (i32.const 68272))
  (global (;1;) i32 (i32.const 1024))
  (global (;2;) i32 (i32.const 2436))
  (global (;3;) i32 (i32.const 1512))
  (global (;4;) i32 (i32.const 1032))
  (global (;5;) i32 (i32.const 1236))
  (global (;6;) i32 (i32.const 1940))
  (global (;7;) i32 (i32.const 1244))
  (global (;8;) i32 (i32.const 1424))
  (global (;9;) i32 (i32.const 1248))
  (global (;10;) i32 (i32.const 1688))
  (global (;11;) i32 (i32.const 1024))
  (global (;12;) i32 (i32.const 2728))
  (global (;13;) i32 (i32.const 1024))
  (global (;14;) i32 (i32.const 68272))
  (global (;15;) i32 (i32.const 0))
  (global (;16;) i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "NSystem" (global 1))
  (export "NTerminalColor" (global 2))
  (export "NError" (global 3))
  (export "NCString" (global 4))
  (export "jump" (func $jump))
  (export "NWM" (global 5))
  (export "NWM_createReferenceWasmMachine" (func $NWM_createReferenceWasmMachine))
  (export "NWM_createReferenceWasmMachineInHeap" (func $NWM_createReferenceWasmMachineInHeap))
  (export "NSystemUtils" (global 6))
  (export "NTime" (global 7))
  (export "NByteVector" (global 8))
  (export "NString" (global 9))
  (export "NVector" (global 10))
  (export "main" (func $main))
  (export "NMain" (func $NMain))
  (export "__dso_handle" (global 11))
  (export "__data_end" (global 12))
  (export "__global_base" (global 13))
  (export "__heap_base" (global 14))
  (export "__memory_base" (global 15))
  (export "__table_base" (global 16))
  (elem (;0;) (i32.const 1) func $initialize $terminate $strlen $startsWith $equals $copy $parseInteger $NWM_createReferenceWasmMachine $NWM_createReferenceWasmMachineInHeap $getTime $initialize.1 $destroy $destroyAndFree $vAppend $append $set $get $create $replace $length $create.1 $createInHeap $destroy.1 $clear $pushBack $popBack $pushBack32Bit $popBack32Bit $get.1 $set.1 $size $terminate.1 $observeErrors $pushError $pushAndPrintError $popErrors $destroyAndFreeErrors $popDestroyAndFreeErrors $logAndTerminate $create.2 $createInHeap.1 $destroy.2 $destroyAndFree.1 $emplaceBack $pushBack.1 $popBack.1 $get.2 $size.1 $parseWatCode $NWM_destroyReferenceMachine $NMain $nMalloc $nFree $nMemset $nMemcpy $nLogI $nLogW $nLogE $getTime.1 $isNaN $isInf)
  (data (;0;) (i32.const 1024) "\01\00\00\00\02\00\00\00\03\00\00\00\04\00\00\00\05\00\00\00\06\00\00\00\07\00\00\00NCString.parseInteger()\00Integer length can't exceed 10 digits and a sign. Found: %s%s\00Only digits from 0 to 9 are allowed. Found: %s%s\00Value too small to fit in a 32 bit integer: %s%s\00\08\00\00\00\09\00\00\00\0a\00\00\00\0b\00\00\00\0c\00\00\00\0d\00\00\00\0e\00\00\00\0f\00\00\00\10\00\00\00\11\00\00\00\12\00\00\00\13\00\00\00\14\00\00\00NString.vAppend\00Expected \22%sd%s\22 after \22%s%%l%s\22 in format string: %s%s\00Unexpected sequence: \22%s%%%c%s\22 in format string: %s%s\00\00%s\00%c\00\00\00\15\00\00\00\16\00\00\00\17\00\00\00\18\00\00\00\19\00\00\00\1a\00\00\00\1b\00\00\00\1c\00\00\00\1d\00\00\00\1e\00\00\00\1f\00\00\00NByteVector.get()\00Index out of bound: %d\00\00\00\00 \00\00\00!\00\00\00\22\00\00\00#\00\00\00$\00\00\00%\00\00\00&\00\00\00'\00\00\00\00NError\00Error tag exceeded maximum length\00Error message exceeded maximum length\00%s\00Unhandled errors\00%sUnhandled errors count: %d\00  %s: %s\00  %s\00\00(\00\00\00)\00\00\00*\00\00\00+\00\00\00,\00\00\00-\00\00\00.\00\00\00/\00\00\000\00\00\00ReferenceMachine.parseModule()\00(\00module\00)\00Expected: \22%s)%s\22 or \22%s(%s\22, found: \22%s%s%s\22\00EOF\00TOKEN exceeded maximum length: %s\00Expected: \22%s%s%s\22, found: \22%s%s%s\22\00found token: %s%s\00ReferenceMachine.parseType()\00type\00;\004\00\00\005\00\00\006\00\00\007\00\00\008\00\00\009\00\00\00:\00\00\00;\00\00\00<\00\00\00=\00\00\00\1b[0m\00\1b[0;30m\00\1b[0;31m\00\1b[0;32m\00\1b[0;33m\00\1b[0;34m\00\1b[0;35m\00\1b[0;36m\00\1b[0;37m\00\1b[1;30m\00\1b[1;31m\00\1b[1;32m\00\1b[1;33m\00\1b[1;34m\00\1b[1;35m\00\1b[1;36m\00\1b[1;37m\00\1b[4;30m\00\1b[4;31m\00\1b[4;32m\00\1b[4;33m\00\1b[4;34m\00\1b[4;35m\00\1b[4;36m\00\1b[4;37m\00\1b[40m\00\1b[41m\00\1b[42m\00\1b[43m\00\1b[44m\00\1b[45m\00\1b[46m\00\1b[47m\00\1b[0;90m\00\1b[0;91m\00\1b[0;92m\00\1b[0;93m\00\1b[0;94m\00\1b[0;95m\00\1b[0;96m\00\1b[0;97m\00\1b[1;90m\00\1b[1;91m\00\1b[1;92m\00\1b[1;93m\00\1b[1;94m\00\1b[1;95m\00\1b[1;96m\00\1b[1;97m\00\1b[0;100m\00\1b[0;101m\00\1b[0;102m\00\1b[0;103m\00\1b[0;104m\00\1b[0;105m\00\1b[0;106m\00\1b[0;107m\00\1bNOMoneSD\00\00\bc\07\00\00\c1\07\00\00\c9\07\00\00\d1\07\00\00\d9\07\00\00\e1\07\00\00\e9\07\00\00\f1\07\00\00\f9\07\00\00\01\08\00\00\09\08\00\00\11\08\00\00\19\08\00\00!\08\00\00)\08\00\001\08\00\009\08\00\00A\08\00\00I\08\00\00Q\08\00\00Y\08\00\00a\08\00\00i\08\00\00q\08\00\00y\08\00\00\81\08\00\00\87\08\00\00\8d\08\00\00\93\08\00\00\99\08\00\00\9f\08\00\00\a5\08\00\00\ab\08\00\00\b1\08\00\00\b9\08\00\00\c1\08\00\00\c9\08\00\00\d1\08\00\00\d9\08\00\00\e1\08\00\00\e9\08\00\00\f1\08\00\00\f9\08\00\00\01\09\00\00\09\09\00\00\11\09\00\00\19\09\00\00!\09\00\00)\09\00\001\09\00\00:\09\00\00C\09\00\00L\09\00\00U\09\00\00^\09\00\00g\09\00\00p\09\00\00y\09\00\00\c9\07\00\00\d9\07\00\00\09\09\00\00\f9\08\00\00(module)\00Parsing result\00%s\0a\00True\00False\00"))
