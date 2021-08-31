clear
rm HelloWasm.wasm
rm HelloWasm.wat
make
echo
echo
wasm2wat HelloWasm.wasm -o HelloWasm.wat

