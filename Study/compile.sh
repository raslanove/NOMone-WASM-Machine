clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -o 1.wasm 1.c
wasm2wat 1.wasm -o 1.wat
