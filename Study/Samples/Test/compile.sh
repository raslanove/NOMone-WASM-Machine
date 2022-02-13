#clang --target=wasm32 -nostdlib -Wl,--no-entry -Wl,--export-all -o result.wasm src.c
#clang --target=wasm32 -O3 -flto -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--lto-O3 -Wl,-z,stack-size=$[10000] -o result.wasm src.c
clang --target=wasm32 -O0 -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,-z,stack-size=$[10000] -o result.wasm src.c

#clang \
#  --target=wasm32 \
#  -O3 \                            # Agressive optimizations
#  -flto \                          # Add metadata for link-time optimizations
#  -nostdlib \                      # Don’t try and link against a standard library.
#  -Wl,--no-entry \                 # Flags passed to the linker.
#  -Wl,--export-all \               #
#  -Wl,--lto-O3 \                   # Aggressive link-time optimizations.
#  -Wl,-z,stack-size=$[64 * 1024] \ # Set maximum stack size to 64KBytes.
#  -o result.wasm \
#  src.c

wasm2wat result.wasm -o result.wat
