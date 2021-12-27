package com.nomone.wasm_machine;

import com.nomone.std_lib.StdLibNatives;

public class WasmMachineNatives {

    //static { System.loadLibrary("NOMoneStdLib"); }

    public static void start() { StdLibNatives.start(); }

    public static int createReferenceMachine() { return nativeCreateReferenceMachine(); }
    public static void destroyMachine(int machineIndex) { nativeDestroyMachine(machineIndex); }
    public static boolean parseWatCode(int machineIndex, String watCode) { return nativeParseWatCode(machineIndex, watCode); }

    private static native boolean nativeParseWatCode(int machineIndex, String watCode);
    private static native int nativeCreateReferenceMachine();
    private static native void nativeDestroyMachine(int machineIndex);
}
