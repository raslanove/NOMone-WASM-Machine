package com.nomone.wasm_machine;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("wasm_machine");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        TextView textView = findViewById(R.id.hello_textView);
        textView.setText(compileWasmToBites());

        textView.postDelayed(() -> Log.e("sdf", "besm Allah :)"), 1000);
    }

    public native String compileWasmToBites();
}
