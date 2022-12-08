/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.alex.hooking64;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class Mainactivity extends Activity
{
///just load the lib from anywhere inside of apk it will work ifyou want i will make with this in a boolean funtion then support :)
    static{ 
    
        System.loadLibrary("hook64");
    }
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
   ///imploments your methoods here
}
