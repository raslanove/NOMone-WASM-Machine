package com.nomone.wasm_machine;

public class WasmMachineNatives {

    static {
        System.loadLibrary("WasmMachine");
    }

    public static void start() {
        // Necessary to trigger running the static code. Could also use Class.forName instead of
        // calling a method.
    }

    public static int createReferenceMachine() { return nativeCreateReferenceMachine(); }
    public static void destroyMachine(int machineIndex) { nativeDestroyMachine(machineIndex); }
    public static boolean parseWatCode(int machineIndex, String watCode) { return nativeParseWatCode(machineIndex, watCode); }

    private static native boolean nativeParseWatCode(int machineIndex, String watCode);
    private static native int nativeCreateReferenceMachine();
    private static native void nativeDestroyMachine(int machineIndex);
}
