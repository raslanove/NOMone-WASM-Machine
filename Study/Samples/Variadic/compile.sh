clang --target=wasm32 -O3 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,-z,stack-size=$[0] -o result.wasm src.c
wasm2wat result.wasm -o result.wat
