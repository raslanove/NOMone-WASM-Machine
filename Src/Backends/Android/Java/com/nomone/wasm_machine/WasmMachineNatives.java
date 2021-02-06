package com.nomone.wasm_machine;

public class WasmMachineNatives {

    static {
        System.loadLibrary("WasmMachine");
    }

    public static String compileWasmToBites(String watCode) {
        return nativeCompileWasmToBites(watCode);
    }

    public static native String nativeCompileWasmToBites(String watCode);
}
