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

        try { Thread.sleep(2000); } catch (InterruptedException e) { e.printStackTrace(); }

        int machineIndex = WasmMachineNatives.createReferenceMachine();
        boolean result = WasmMachineNatives.parseWatCode(machineIndex, "(module)");
        textView.setText("" + result);
        WasmMachineNatives.destroyMachine(machineIndex);

        try { Thread.sleep(2000); } catch (InterruptedException e) { e.printStackTrace(); }
    }
}
