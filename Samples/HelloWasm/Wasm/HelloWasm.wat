(module
  (type (;0;) (func))
  (type (;1;) (func (param i32 i32)))
  (type (;2;) (func (param i32 i32) (result i32)))
  (type (;3;) (func (param i32)))
  (type (;4;) (func (param i32 i32 i32) (result i32)))
  (type (;5;) (func (param f64) (result i32)))
  (type (;6;) (func (param i32) (result i32)))
  (type (;7;) (func (param i32 i32 i32)))
  (type (;8;) (func (result i32)))
  (type (;9;) (func (param i32 f64 i32)))
  (type (;10;) (func (param i32 i32 i32 i32) (result i32)))
  (func $__wasm_call_ctors (type 0))
  (func $initialize (type 3) (param i32)
    block  ;; label = @1
      local.get 0
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      call_indirect (type 0)
    end)
  (func $terminate (type 0))
  (func $startsWith (type 2) (param i32 i32) (result i32)
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
  (func $equals (type 2) (param i32 i32) (result i32)
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
  (func $copy (type 2) (param i32 i32) (result i32)
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
  (func $getTime (type 3) (param i32)
    local.get 0
    local.get 0
    i32.const 8
    i32.add
    i32.const 0
    i32.load offset=1764
    call_indirect (type 1))
  (func $initialize.1 (type 6) (param i32) (result i32)
    i32.const 4
    local.get 0
    i32.const 0
    i32.load offset=1248
    call_indirect (type 2)
    drop
    local.get 0
    i32.const 0
    i32.const 0
    i32.load offset=1264
    call_indirect (type 2)
    drop
    local.get 0)
  (func $destroy (type 3) (param i32)
    local.get 0
    i32.const 0
    i32.load offset=1256
    call_indirect (type 3))
  (func $destroyAndFree (type 3) (param i32)
    local.get 0
    i32.const 0
    i32.load offset=1256
    call_indirect (type 3)
    local.get 0
    i32.const 0
    i32.load offset=1740
    call_indirect (type 3))
  (func $vAppend (type 4) (param i32 i32 i32) (result i32)
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
    i32.load offset=1268
    call_indirect (type 2)
    drop
    local.get 3
    i32.const 64
    i32.add
    i32.const -2
    i32.add
    local.set 4
    i32.const 0
    i32.load offset=1772
    local.set 5
    i32.const 0
    i32.load offset=1768
    local.set 6
    i32.const 0
    i32.load offset=1264
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
                      call_indirect (type 2)
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
                  call_indirect (type 2)
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
                  call_indirect (type 2)
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
                call_indirect (type 2)
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
                    call_indirect (type 2)
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
                i32.load offset=2472
                local.tee 8
                i32.store
                local.get 3
                i32.const 0
                i32.load offset=2460
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
                i32.const 1112
                i32.const 1128
                local.get 3
                i32.const 32
                i32.add
                i32.const 0
                i32.load offset=1348
                call_indirect (type 4)
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
                call_indirect (type 2)
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
              call_indirect (type 2)
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
                call_indirect (type 2)
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
              call_indirect (type 2)
              drop
              local.get 0
              i32.const 97
              local.get 7
              call_indirect (type 2)
              drop
              local.get 0
              i32.const 110
              local.get 7
              call_indirect (type 2)
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
              call_indirect (type 2)
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
              call_indirect (type 2)
              drop
              local.get 0
              i32.const 110
              local.get 7
              call_indirect (type 2)
              drop
              local.get 0
              i32.const 102
              local.get 7
              call_indirect (type 2)
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
          i32.load offset=2472
          local.tee 8
          i32.store offset=12
          local.get 3
          i32.const 0
          i32.load offset=2460
          i32.store offset=8
          local.get 3
          local.get 8
          i32.store
          i32.const 1112
          i32.const 1184
          local.get 3
          i32.const 0
          i32.load offset=1348
          call_indirect (type 4)
          drop
        end
        local.get 0
        i32.const 0
        local.get 7
        call_indirect (type 2)
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
      call_indirect (type 2)
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
    i32.load offset=1264
    local.tee 11
    call_indirect (type 2)
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
        call_indirect (type 2)
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
      call_indirect (type 2)
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
          call_indirect (type 2)
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
      call_indirect (type 2)
      drop
      block  ;; label = @2
        local.get 2
        i32.const -1
        i32.gt_s
        br_if 0 (;@2;)
        local.get 0
        i32.const 45
        local.get 11
        call_indirect (type 2)
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
      call_indirect (type 2)
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
        call_indirect (type 2)
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
  (func $append (type 4) (param i32 i32 i32) (result i32)
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
  (func $set (type 4) (param i32 i32 i32) (result i32)
    (local i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 3
    global.set 0
    local.get 0
    i32.const 0
    i32.load offset=1260
    call_indirect (type 6)
    drop
    local.get 0
    i32.const 0
    i32.const 0
    i32.load offset=1264
    call_indirect (type 2)
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
  (func $create (type 2) (param i32 i32) (result i32)
    (local i32 i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 2
    global.set 0
    i32.const 4
    i32.const 12
    i32.const 0
    i32.load offset=1736
    call_indirect (type 6)
    local.tee 3
    i32.const 0
    i32.load offset=1248
    call_indirect (type 2)
    drop
    local.get 3
    i32.const 0
    i32.const 0
    i32.load offset=1264
    call_indirect (type 2)
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
  (func $replace (type 4) (param i32 i32 i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32)
    global.get 0
    i32.const 32
    i32.sub
    local.tee 3
    global.set 0
    i32.const 0
    local.set 4
    i32.const 1239
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
            i32.const 1240
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
          i32.const 1243
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
  (func $create.1 (type 2) (param i32 i32) (result i32)
    local.get 1
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
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
      i32.load offset=1736
      call_indirect (type 6)
      i32.store offset=8
    end
    local.get 1)
  (func $createInHeap (type 6) (param i32) (result i32)
    (local i32 i32)
    i32.const 12
    i32.const 0
    i32.load offset=1736
    local.tee 1
    call_indirect (type 6)
    local.tee 2
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
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
  (func $destroy.1 (type 3) (param i32)
    local.get 0
    i32.load offset=8
    i32.const 0
    i32.load offset=1740
    call_indirect (type 3)
    local.get 0
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
    drop)
  (func $clear (type 6) (param i32) (result i32)
    local.get 0
    i32.const 0
    i32.store offset=4
    local.get 0)
  (func $pushBack (type 2) (param i32 i32) (result i32)
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
        i32.load offset=1736
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
        i32.load offset=1748
        call_indirect (type 4)
        drop
        local.get 0
        i32.load offset=8
        i32.const 0
        i32.load offset=1740
        call_indirect (type 3)
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
  (func $popBack (type 2) (param i32 i32) (result i32)
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
  (func $pushBack32Bit (type 2) (param i32 i32) (result i32)
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
        i32.load offset=1736
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
        i32.load offset=1748
        call_indirect (type 4)
        drop
        local.get 0
        i32.load offset=8
        i32.const 0
        i32.load offset=1740
        call_indirect (type 3)
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
  (func $popBack32Bit (type 2) (param i32 i32) (result i32)
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
  (func $get.1 (type 2) (param i32 i32) (result i32)
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
        i32.const 1292
        i32.const 1310
        local.get 2
        i32.const 0
        i32.load offset=1348
        call_indirect (type 4)
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
  (func $set.1 (type 4) (param i32 i32 i32) (result i32)
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
        i32.const 1292
        i32.const 1310
        local.get 3
        i32.const 0
        i32.load offset=1348
        call_indirect (type 4)
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
  (func $pushError (type 4) (param i32 i32 i32) (result i32)
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
  (func $vPushError (type 4) (param i32 i32 i32) (result i32)
    (local i32 i32 i32)
    i32.const 0
    local.set 3
    block  ;; label = @1
      i32.const 0
      i32.load offset=2516
      br_if 0 (;@1;)
      i32.const 0
      i32.const 0
      i32.const 336
      i32.const 0
      i32.load offset=1524
      call_indirect (type 2)
      i32.store offset=2516
    end
    i32.const 1368
    i32.const 0
    i32.const 0
    i32.load offset=1100
    call_indirect (type 2)
    local.tee 4
    local.get 1
    local.get 2
    i32.const 0
    i32.load offset=1084
    call_indirect (type 4)
    drop
    i32.const 0
    i32.load offset=2516
    i32.const 0
    i32.load offset=1536
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
      i32.const 1369
      i32.const 1376
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
    i32.load offset=1108
    call_indirect (type 6)
    local.set 3
    local.get 4
    i32.const 0
    i32.load offset=1096
    call_indirect (type 6)
    local.set 0
    i32.const 0
    i32.load offset=1748
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
        call_indirect (type 4)
        drop
        local.get 2
        i32.const 0
        i32.store8 offset=319
        i32.const 1369
        i32.const 1410
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
      call_indirect (type 4)
      drop
    end
    local.get 4
    i32.const 0
    i32.load offset=1080
    call_indirect (type 3)
    local.get 2
    i32.const 320
    i32.add
    i32.const 0
    i32.load offset=1044
    call_indirect (type 3)
    local.get 2)
  (func $pushAndPrintError (type 4) (param i32 i32 i32) (result i32)
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
    i32.const 1448
    local.get 3
    i32.const 0
    i32.load offset=1760
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
      i32.load offset=2516
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      i32.const 0
      local.set 1
      local.get 2
      i32.const 0
      i32.load offset=1552
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
      i32.load offset=1524
      call_indirect (type 2)
      local.set 1
      i32.const 0
      i32.load offset=1548
      local.set 3
      i32.const 0
      i32.load offset=1544
      local.set 4
      i32.const 0
      i32.load offset=1536
      local.set 5
      i32.const 0
      local.set 0
      loop  ;; label = @2
        local.get 1
        local.get 5
        call_indirect (type 6)
        drop
        i32.const 0
        i32.load offset=2516
        local.get 1
        local.get 0
        local.get 3
        call_indirect (type 2)
        local.get 4
        call_indirect (type 2)
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
  (func $destroyAndFreeErrors (type 3) (param i32)
    local.get 0
    i32.const 0
    i32.load offset=1528
    call_indirect (type 3)
    local.get 0
    i32.const 0
    i32.load offset=1740
    call_indirect (type 3))
  (func $popDestroyAndFreeErrors (type 6) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32)
    i32.const 0
    local.set 1
    block  ;; label = @1
      i32.const 0
      i32.load offset=2516
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      i32.const 0
      local.set 1
      local.get 2
      i32.const 0
      i32.load offset=1552
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
      i32.load offset=1524
      call_indirect (type 2)
      local.set 0
      i32.const 0
      i32.load offset=1548
      local.set 4
      i32.const 0
      i32.load offset=1544
      local.set 5
      i32.const 0
      i32.load offset=1536
      local.set 6
      i32.const 0
      local.set 1
      loop  ;; label = @2
        local.get 0
        local.get 6
        call_indirect (type 6)
        drop
        i32.const 0
        i32.load offset=2516
        local.get 0
        local.get 1
        local.get 4
        call_indirect (type 2)
        local.get 5
        call_indirect (type 2)
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
      i32.load offset=1528
      call_indirect (type 3)
      local.get 0
      i32.const 0
      i32.load offset=1740
      call_indirect (type 3)
    end
    local.get 1)
  (func $terminate.1 (type 0)
    (local i32)
    block  ;; label = @1
      i32.const 0
      i32.load offset=2516
      local.tee 0
      i32.eqz
      br_if 0 (;@1;)
      local.get 0
      i32.const 0
      i32.load offset=1532
      call_indirect (type 3)
      i32.const 0
      i32.const 0
      i32.store offset=2516
    end)
  (func $observeErrors (type 8) (result i32)
    (local i32 i32)
    i32.const 0
    local.set 0
    block  ;; label = @1
      i32.const 0
      i32.load offset=2516
      local.tee 1
      i32.eqz
      br_if 0 (;@1;)
      local.get 1
      i32.const 0
      i32.load offset=1552
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
    i32.load offset=1552
    local.set 1
    block  ;; label = @1
      block  ;; label = @2
        i32.const 0
        i32.load offset=2516
        local.tee 2
        i32.eqz
        br_if 0 (;@2;)
        local.get 2
        local.get 1
        call_indirect (type 6)
        local.tee 3
        i32.const 1
        i32.lt_s
        br_if 0 (;@2;)
        local.get 3
        i32.const 336
        i32.const 0
        i32.load offset=1524
        call_indirect (type 2)
        local.set 4
        i32.const 0
        i32.load offset=1548
        local.set 5
        i32.const 0
        i32.load offset=1544
        local.set 6
        i32.const 0
        i32.load offset=1536
        local.set 7
        i32.const 0
        local.set 2
        loop  ;; label = @3
          local.get 4
          local.get 7
          call_indirect (type 6)
          drop
          i32.const 0
          i32.load offset=2516
          local.get 4
          local.get 2
          local.get 5
          call_indirect (type 2)
          local.get 6
          call_indirect (type 2)
          drop
          local.get 3
          local.get 2
          i32.const 1
          i32.add
          local.tee 2
          i32.ne
          br_if 0 (;@3;)
        end
        local.get 4
        local.get 1
        call_indirect (type 6)
        local.set 5
        local.get 4
        i32.eqz
        br_if 1 (;@1;)
        local.get 0
        local.get 4
        local.get 1
        call_indirect (type 6)
        i32.store offset=36
        local.get 0
        i32.const 0
        i32.load offset=2472
        i32.store offset=32
        i32.const 1451
        i32.const 1468
        local.get 0
        i32.const 32
        i32.add
        i32.const 0
        i32.load offset=1756
        local.tee 2
        call_indirect (type 7)
        block  ;; label = @3
          local.get 4
          local.get 0
          i32.const 48
          i32.add
          local.get 6
          call_indirect (type 2)
          i32.eqz
          br_if 0 (;@3;)
          local.get 0
          i32.const 112
          i32.add
          local.set 3
          loop  ;; label = @4
            block  ;; label = @5
              block  ;; label = @6
                local.get 0
                i32.load8_u offset=48
                i32.eqz
                br_if 0 (;@6;)
                local.get 0
                local.get 3
                i32.store offset=20
                local.get 0
                local.get 0
                i32.const 48
                i32.add
                i32.store offset=16
                i32.const 0
                i32.const 1497
                local.get 0
                i32.const 16
                i32.add
                local.get 2
                call_indirect (type 7)
                br 1 (;@5;)
              end
              local.get 0
              local.get 3
              i32.store
              i32.const 0
              i32.const 1506
              local.get 0
              local.get 2
              call_indirect (type 7)
            end
            local.get 4
            local.get 0
            i32.const 48
            i32.add
            local.get 6
            call_indirect (type 2)
            br_if 0 (;@4;)
          end
        end
        local.get 4
        i32.const 0
        i32.load offset=1528
        call_indirect (type 3)
        local.get 4
        i32.const 0
        i32.load offset=1740
        call_indirect (type 3)
        br 1 (;@1;)
      end
      i32.const 0
      local.get 1
      call_indirect (type 6)
      local.set 5
    end
    block  ;; label = @1
      i32.const 0
      i32.load offset=2516
      local.tee 2
      i32.eqz
      br_if 0 (;@1;)
      local.get 2
      i32.const 0
      i32.load offset=1532
      call_indirect (type 3)
      i32.const 0
      i32.const 0
      i32.store offset=2516
    end
    local.get 0
    i32.const 384
    i32.add
    global.set 0
    local.get 5)
  (func $jump (type 2) (param i32 i32) (result i32)
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
  (func $create.2 (type 4) (param i32 i32 i32) (result i32)
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
    i32.load offset=1744
    call_indirect (type 4)
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
      i32.load offset=1736
      call_indirect (type 6)
      i32.store offset=12
    end
    local.get 2)
  (func $createInHeap.1 (type 2) (param i32 i32) (result i32)
    (local i32 i32)
    block  ;; label = @1
      local.get 1
      br_if 0 (;@1;)
      i32.const 0
      return
    end
    i32.const 16
    i32.const 0
    i32.load offset=1736
    local.tee 2
    call_indirect (type 6)
    local.tee 3
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
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
  (func $destroy.2 (type 3) (param i32)
    local.get 0
    i32.load offset=12
    i32.const 0
    i32.load offset=1740
    call_indirect (type 3)
    local.get 0
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
    drop)
  (func $destroyAndFree.1 (type 3) (param i32)
    (local i32)
    local.get 0
    i32.load offset=12
    i32.const 0
    i32.load offset=1740
    local.tee 1
    call_indirect (type 3)
    local.get 0
    i32.const 0
    i32.const 16
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
    drop
    local.get 0
    local.get 1
    call_indirect (type 3))
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
          i32.load offset=1736
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
        i32.load offset=1736
        call_indirect (type 6)
        local.tee 1
        i32.eqz
        br_if 1 (;@1;)
        local.get 1
        local.get 0
        i32.load offset=12
        local.get 3
        i32.const 0
        i32.load offset=1748
        call_indirect (type 4)
        drop
        local.get 0
        i32.load offset=12
        i32.const 0
        i32.load offset=1740
        call_indirect (type 3)
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
  (func $pushBack.1 (type 2) (param i32 i32) (result i32)
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
          i32.load offset=1736
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
        i32.load offset=1736
        call_indirect (type 6)
        local.tee 2
        i32.eqz
        br_if 1 (;@1;)
        local.get 2
        local.get 0
        i32.load offset=12
        local.get 4
        i32.const 0
        i32.load offset=1748
        call_indirect (type 4)
        drop
        local.get 0
        i32.load offset=12
        i32.const 0
        i32.load offset=1740
        call_indirect (type 3)
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
      i32.load offset=1748
      call_indirect (type 4)
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
  (func $popBack.1 (type 2) (param i32 i32) (result i32)
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
    i32.load offset=1748
    call_indirect (type 4)
    drop
    i32.const 1)
  (func $get.2 (type 2) (param i32 i32) (result i32)
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
    i32.load offset=1744
    call_indirect (type 4)
    drop
    local.get 0
    i32.const 47
    i32.store offset=8
    local.get 0
    i32.const 48
    i32.store offset=4
    local.get 0
    i32.const 1
    i32.store
    local.get 0)
  (func $parseWatCode (type 2) (param i32 i32) (result i32)
    (local i32 i32 i32 i32)
    global.get 0
    i32.const 288
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
        local.get 1
        local.get 2
        i32.const 28
        i32.add
        i32.const 1587
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
        local.get 1
        local.get 2
        i32.const 28
        i32.add
        i32.const 1589
        local.get 2
        i32.const 32
        i32.add
        call $getToken
        br_if 0 (;@2;)
        i32.const 0
        local.set 3
        local.get 2
        i32.const 0
        i32.store offset=28
        br 1 (;@1;)
      end
      local.get 2
      i32.load offset=28
      local.set 4
      i32.const 0
      local.set 3
      block  ;; label = @2
        block  ;; label = @3
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
          br_if 0 (;@3;)
          i32.const 0
          i32.load offset=1036
          local.set 5
          loop  ;; label = @4
            block  ;; label = @5
              local.get 2
              i32.const 32
              i32.add
              i32.const 1596
              local.get 5
              call_indirect (type 2)
              i32.eqz
              br_if 0 (;@5;)
              i32.const 1
              local.set 3
              br 4 (;@1;)
            end
            local.get 2
            i32.const 32
            i32.add
            i32.const 1587
            local.get 5
            call_indirect (type 2)
            i32.eqz
            br_if 2 (;@2;)
            local.get 2
            local.get 4
            i32.store offset=28
            local.get 1
            local.get 2
            i32.const 28
            i32.add
            i32.const 0
            local.get 2
            i32.const 32
            i32.add
            call $getToken
            br_if 0 (;@4;)
          end
        end
        local.get 2
        i32.const 0
        i32.store offset=28
        br 1 (;@1;)
      end
      i32.const 0
      local.set 3
      local.get 2
      i32.const 24
      i32.add
      i32.const 0
      i32.load offset=2460
      local.tee 5
      i32.store
      local.get 2
      i32.const 16
      i32.add
      i32.const 0
      i32.load offset=2472
      local.tee 1
      i32.store
      local.get 2
      i32.const 20
      i32.add
      local.get 2
      i32.const 32
      i32.add
      i32.store
      local.get 2
      local.get 5
      i32.store offset=12
      local.get 2
      local.get 1
      i32.store offset=8
      local.get 2
      local.get 5
      i32.store offset=4
      local.get 2
      local.get 1
      i32.store
      i32.const 1556
      i32.const 1598
      local.get 2
      i32.const 0
      i32.load offset=1348
      call_indirect (type 4)
      drop
    end
    local.get 2
    i32.const 288
    i32.add
    global.set 0
    local.get 3)
  (func $NWM_destroyReferenceMachine (type 3) (param i32)
    local.get 0
    i32.const 0
    i32.store)
  (func $getToken (type 10) (param i32 i32 i32 i32) (result i32)
    (local i32 i32 i32 i32)
    global.get 0
    i32.const 64
    i32.sub
    local.tee 4
    global.set 0
    local.get 1
    i32.load
    local.set 5
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          loop  ;; label = @4
            local.get 0
            local.get 5
            i32.add
            i32.load8_u
            local.tee 6
            i32.const 59
            i32.gt_u
            br_if 2 (;@2;)
            block  ;; label = @5
              block  ;; label = @6
                local.get 6
                br_table 1 (;@5;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 0 (;@6;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 0 (;@6;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 3 (;@3;) 3 (;@3;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 4 (;@2;) 3 (;@3;) 1 (;@5;)
              end
              local.get 1
              local.get 5
              i32.const 1
              i32.add
              local.tee 5
              i32.store
              br 1 (;@4;)
            end
          end
          local.get 3
          i32.const 1644
          i32.const 0
          i32.load offset=1040
          call_indirect (type 2)
          drop
          i32.const 1
          local.set 6
          br 2 (;@1;)
        end
        local.get 3
        i32.const 0
        i32.store8 offset=1
        local.get 3
        local.get 6
        i32.store8
        i32.const 1
        local.set 6
        local.get 1
        local.get 1
        i32.load
        i32.const 1
        i32.add
        i32.store
        br 1 (;@1;)
      end
      i32.const 0
      local.set 5
      block  ;; label = @2
        block  ;; label = @3
          loop  ;; label = @4
            block  ;; label = @5
              local.get 6
              i32.const 255
              i32.and
              local.tee 7
              i32.const 59
              i32.gt_u
              br_if 0 (;@5;)
              local.get 7
              br_table 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 2 (;@3;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 0 (;@5;) 2 (;@3;) 2 (;@3;)
            end
            local.get 3
            local.get 5
            i32.add
            local.get 6
            i32.store8
            block  ;; label = @5
              local.get 5
              i32.const 255
              i32.ne
              br_if 0 (;@5;)
              local.get 4
              local.get 3
              i32.store offset=48
              i32.const 0
              local.set 6
              i32.const 1556
              i32.const 1648
              local.get 4
              i32.const 48
              i32.add
              i32.const 0
              i32.load offset=1348
              call_indirect (type 4)
              drop
              i32.const 256
              local.set 5
              br 3 (;@2;)
            end
            local.get 5
            i32.const 1
            i32.add
            local.set 5
            local.get 1
            local.get 1
            i32.load
            i32.const 1
            i32.add
            local.tee 6
            i32.store
            local.get 0
            local.get 6
            i32.add
            i32.load8_u
            local.set 6
            br 0 (;@4;)
          end
        end
        i32.const 1
        local.set 6
      end
      local.get 3
      local.get 5
      i32.add
      i32.const 0
      i32.store8
    end
    block  ;; label = @1
      block  ;; label = @2
        local.get 2
        i32.eqz
        br_if 0 (;@2;)
        i32.const 0
        local.set 5
        local.get 3
        local.get 2
        i32.const 0
        i32.load offset=1036
        call_indirect (type 2)
        br_if 0 (;@2;)
        local.get 4
        i32.const 36
        i32.add
        i32.const 0
        i32.load offset=2460
        local.tee 6
        i32.store
        local.get 4
        i32.const 32
        i32.add
        local.get 3
        i32.store
        local.get 4
        local.get 2
        i32.store offset=20
        local.get 4
        i32.const 0
        i32.load offset=2472
        local.tee 1
        i32.store offset=28
        local.get 4
        local.get 6
        i32.store offset=24
        local.get 4
        local.get 1
        i32.store offset=16
        i32.const 1556
        i32.const 1682
        local.get 4
        i32.const 16
        i32.add
        i32.const 0
        i32.load offset=1348
        call_indirect (type 4)
        drop
        br 1 (;@1;)
      end
      block  ;; label = @2
        local.get 6
        br_if 0 (;@2;)
        i32.const 0
        local.set 5
        br 1 (;@1;)
      end
      local.get 4
      local.get 3
      i32.store offset=4
      local.get 4
      i32.const 0
      i32.load offset=2472
      i32.store
      i32.const 1556
      i32.const 1718
      local.get 4
      i32.const 0
      i32.load offset=1752
      call_indirect (type 7)
      i32.const 1
      local.set 5
    end
    local.get 4
    i32.const 64
    i32.add
    global.set 0
    local.get 5)
  (func $NWM_createReferenceWasmMachineInHeap (type 8) (result i32)
    (local i32)
    i32.const 12
    i32.const 0
    i32.load offset=1736
    call_indirect (type 6)
    local.tee 0
    i32.const 0
    i32.const 12
    i32.const 0
    i32.load offset=1744
    call_indirect (type 4)
    drop
    local.get 0
    i32.const 47
    i32.store offset=8
    local.get 0
    i32.const 48
    i32.store offset=4
    local.get 0
    i32.const 1
    i32.store
    local.get 0)
  (func $main (type 0)
    i32.const 49
    i32.const 0
    i32.load offset=1024
    call_indirect (type 3)
    i32.const 0
    i32.load offset=1028
    call_indirect (type 0))
  (func $nMalloc (type 6) (param i32) (result i32)
    i32.const 0)
  (func $nFree (type 3) (param i32))
  (func $nMemset (type 4) (param i32 i32 i32) (result i32)
    i32.const 0)
  (func $nMemcpy (type 4) (param i32 i32 i32) (result i32)
    i32.const 0)
  (func $nLogI (type 7) (param i32 i32 i32))
  (func $nLogW (type 7) (param i32 i32 i32))
  (func $nLogE (type 7) (param i32 i32 i32))
  (func $getTime.1 (type 1) (param i32 i32))
  (func $isNaN (type 5) (param f64) (result i32)
    i32.const 0)
  (func $isInf (type 5) (param f64) (result i32)
    i32.const 0)
  (func $NMain (type 0)
    (local i32 i32 i32)
    global.get 0
    i32.const 16
    i32.sub
    local.tee 0
    global.set 0
    local.get 0
    i64.const 28548224099773736
    i64.store offset=8
    i32.const 0
    i32.load offset=1516
    call_indirect (type 8)
    local.tee 1
    local.get 0
    i32.const 8
    i32.add
    local.get 1
    i32.load offset=8
    call_indirect (type 2)
    local.set 2
    local.get 1
    local.get 1
    i32.load offset=4
    call_indirect (type 3)
    local.get 1
    i32.const 0
    i32.load offset=1740
    call_indirect (type 3)
    local.get 0
    i32.const 2499
    i32.const 2504
    local.get 2
    i32.const 1
    i32.eq
    select
    i32.store
    i32.const 2480
    i32.const 2495
    local.get 0
    i32.const 0
    i32.load offset=1752
    call_indirect (type 7)
    i32.const 2510
    i32.const 2511
    i32.const 0
    i32.const 0
    i32.load offset=1348
    call_indirect (type 4)
    drop
    i32.const 0
    i32.load offset=1364
    call_indirect (type 8)
    drop
    local.get 0
    i32.const 16
    i32.add
    global.set 0)
  (table (;0;) 60 60 funcref)
  (memory (;0;) 2)
  (global (;0;) (mut i32) (i32.const 68064))
  (global (;1;) i32 (i32.const 1024))
  (global (;2;) i32 (i32.const 1032))
  (global (;3;) i32 (i32.const 1736))
  (global (;4;) i32 (i32.const 1044))
  (global (;5;) i32 (i32.const 1248))
  (global (;6;) i32 (i32.const 2232))
  (global (;7;) i32 (i32.const 1336))
  (global (;8;) i32 (i32.const 1048))
  (global (;9;) i32 (i32.const 1072))
  (global (;10;) i32 (i32.const 1520))
  (global (;11;) i32 (i32.const 1512))
  (global (;12;) i32 (i32.const 1024))
  (global (;13;) i32 (i32.const 2520))
  (global (;14;) i32 (i32.const 1024))
  (global (;15;) i32 (i32.const 68064))
  (global (;16;) i32 (i32.const 0))
  (global (;17;) i32 (i32.const 1))
  (export "memory" (memory 0))
  (export "__wasm_call_ctors" (func $__wasm_call_ctors))
  (export "NSystem" (global 1))
  (export "NCString" (global 2))
  (export "NSystemUtils" (global 3))
  (export "NTime" (global 4))
  (export "NByteVector" (global 5))
  (export "NTerminalColor" (global 6))
  (export "NError" (global 7))
  (export "DEFAULT_DOUBLE_CONVERSION_SETTINGS" (global 8))
  (export "NString" (global 9))
  (export "NVector" (global 10))
  (export "jump" (func $jump))
  (export "NWM" (global 11))
  (export "NWM_createReferenceWasmMachine" (func $NWM_createReferenceWasmMachine))
  (export "NWM_createReferenceWasmMachineInHeap" (func $NWM_createReferenceWasmMachineInHeap))
  (export "main" (func $main))
  (export "NMain" (func $NMain))
  (export "__dso_handle" (global 12))
  (export "__data_end" (global 13))
  (export "__global_base" (global 14))
  (export "__heap_base" (global 15))
  (export "__memory_base" (global 16))
  (export "__table_base" (global 17))
  (elem (;0;) (i32.const 1) func $initialize $terminate $startsWith $equals $copy $getTime $initialize.1 $destroy $destroyAndFree $vAppend $append $set $get $create $replace $length $create.1 $createInHeap $destroy.1 $clear $pushBack $popBack $pushBack32Bit $popBack32Bit $get.1 $set.1 $size $terminate.1 $observeErrors $pushError $pushAndPrintError $popErrors $destroyAndFreeErrors $popDestroyAndFreeErrors $logAndTerminate $NWM_createReferenceWasmMachine $NWM_createReferenceWasmMachineInHeap $create.2 $createInHeap.1 $destroy.2 $destroyAndFree.1 $emplaceBack $pushBack.1 $popBack.1 $get.2 $size.1 $parseWatCode $NWM_destroyReferenceMachine $NMain $nMalloc $nFree $nMemset $nMemcpy $nLogI $nLogW $nLogE $getTime.1 $isNaN $isInf)
  (data (;0;) (i32.const 1024) "\01\00\00\00\02\00\00\00\03\00\00\00\04\00\00\00\05\00\00\00\06\00\00\00\13\00\00\00\07\00\00\00\ac\c5'7#\c7\0a_\00\e1\f5\05\00\00\00\00\07\00\00\00\08\00\00\00\09\00\00\00\0a\00\00\00\0b\00\00\00\0c\00\00\00\0d\00\00\00\0e\00\00\00\0f\00\00\00\10\00\00\00NString.vAppend\00Expected \22%sd%s\22 after \22%s%%l%s\22 in format string: %s%s\00Unexpected sequence: \22%s%%%c%s\22 in format string: %s%s\00\00%s\00%c\00\00\00\11\00\00\00\12\00\00\00\13\00\00\00\14\00\00\00\15\00\00\00\16\00\00\00\17\00\00\00\18\00\00\00\19\00\00\00\1a\00\00\00\1b\00\00\00NByteVector.get()\00Index out of bound: %d\00\00\00\00\1c\00\00\00\1d\00\00\00\1e\00\00\00\1f\00\00\00 \00\00\00!\00\00\00\22\00\00\00#\00\00\00\00NError\00Error tag exceeded maximum length\00Error message exceeded maximum length\00%s\00Unhandled errors\00%sUnhandled errors count: %d\00  %s: %s\00  %s\00\00$\00\00\00%\00\00\00&\00\00\00'\00\00\00(\00\00\00)\00\00\00*\00\00\00+\00\00\00,\00\00\00-\00\00\00.\00\00\00ReferenceMachine.parseModule()\00(\00module\00)\00Expected: \22%s)%s\22 or \22%s(%s\22, found: \22%s%s%s\22\00EOF\00TOKEN exceeded maximum length: %s\00Expected: \22%s%s%s\22, found: \22%s%s%s\22\00found token: %s%s\002\00\00\003\00\00\004\00\00\005\00\00\006\00\00\007\00\00\008\00\00\009\00\00\00:\00\00\00;\00\00\00\1b[0m\00\1b[0;30m\00\1b[0;31m\00\1b[0;32m\00\1b[0;33m\00\1b[0;34m\00\1b[0;35m\00\1b[0;36m\00\1b[0;37m\00\1b[1;30m\00\1b[1;31m\00\1b[1;32m\00\1b[1;33m\00\1b[1;34m\00\1b[1;35m\00\1b[1;36m\00\1b[1;37m\00\1b[4;30m\00\1b[4;31m\00\1b[4;32m\00\1b[4;33m\00\1b[4;34m\00\1b[4;35m\00\1b[4;36m\00\1b[4;37m\00\1b[40m\00\1b[41m\00\1b[42m\00\1b[43m\00\1b[44m\00\1b[45m\00\1b[46m\00\1b[47m\00\1b[0;90m\00\1b[0;91m\00\1b[0;92m\00\1b[0;93m\00\1b[0;94m\00\1b[0;95m\00\1b[0;96m\00\1b[0;97m\00\1b[1;90m\00\1b[1;91m\00\1b[1;92m\00\1b[1;93m\00\1b[1;94m\00\1b[1;95m\00\1b[1;96m\00\1b[1;97m\00\1b[0;100m\00\1b[0;101m\00\1b[0;102m\00\1b[0;103m\00\1b[0;104m\00\1b[0;105m\00\1b[0;106m\00\1b[0;107m\00\1bNOMoneSD\00\00\f0\06\00\00\f5\06\00\00\fd\06\00\00\05\07\00\00\0d\07\00\00\15\07\00\00\1d\07\00\00%\07\00\00-\07\00\005\07\00\00=\07\00\00E\07\00\00M\07\00\00U\07\00\00]\07\00\00e\07\00\00m\07\00\00u\07\00\00}\07\00\00\85\07\00\00\8d\07\00\00\95\07\00\00\9d\07\00\00\a5\07\00\00\ad\07\00\00\b5\07\00\00\bb\07\00\00\c1\07\00\00\c7\07\00\00\cd\07\00\00\d3\07\00\00\d9\07\00\00\df\07\00\00\e5\07\00\00\ed\07\00\00\f5\07\00\00\fd\07\00\00\05\08\00\00\0d\08\00\00\15\08\00\00\1d\08\00\00%\08\00\00-\08\00\005\08\00\00=\08\00\00E\08\00\00M\08\00\00U\08\00\00]\08\00\00e\08\00\00n\08\00\00w\08\00\00\80\08\00\00\89\08\00\00\92\08\00\00\9b\08\00\00\a4\08\00\00\ad\08\00\00\fd\06\00\00\0d\07\00\00=\08\00\00-\08\00\00Parsing result\00%s\0a\00True\00False\00\00hey!\00"))
