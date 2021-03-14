package com.nomone.wasm_machine;

public class WasmMachineNatives {

    static {
        System.loadLibrary("WasmMachine");
    }

    public static int createReferenceMachine() { return nativeCreateReferenceMachine(); }
    public static void destroyMachine(int machineIndex) { nativeDestroyMachine(machineIndex); }
    public static String compileWasmToBites(int machineIndex, String watCode) { return nativeCompileWasmToBites(machineIndex, watCode); }

    private static native String nativeCompileWasmToBites(int machineIndex, String watCode);
    private static native int nativeCreateReferenceMachine();
    private static native void nativeDestroyMachine(int machineIndex);
}
