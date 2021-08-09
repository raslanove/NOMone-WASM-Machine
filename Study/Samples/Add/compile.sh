clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -o result.wasm src.c
wasm2wat result.wasm -o result.wat
