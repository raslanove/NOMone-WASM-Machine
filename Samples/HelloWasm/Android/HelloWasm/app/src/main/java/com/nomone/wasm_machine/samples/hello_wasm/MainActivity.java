package com.nomone.wasm_machine.samples.hello_wasm;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.nomone.wasm_machine.WasmMachineNatives;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        TextView textView = findViewById(R.id.hello_textView);

        int machineIndex = WasmMachineNatives.createReferenceMachine();
        textView.setText(WasmMachineNatives.compileWasmToBites(machineIndex, "besm Allah AlRa7maan AlRa7eem :)"));
        WasmMachineNatives.destroyMachine(machineIndex);
    }
}
