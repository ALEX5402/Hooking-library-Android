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
 *
 */
 /** https://github.com/ALEX5402
 this project is for hooking native 64 bit libs from runtime android internally 
      on 7/12/2022 for educatuonal perpose only :) **/
      
#include "include/Includes.h"
#include "include/logger.h"
#include "Hooking/And64InlineHook.hpp"
#include "Buffers/fake_dlfcn.h"
#include "include/obfuscate.h"
#include "Proc/Proc.h"
#include <dlfcn.h>
//define the default lib name here
#define targetLibName OBFUSCATE("Defaultlib.so")


//focked from android modmenu macros.h

std::vector<uint64_t> offsetVector;

void hook(void *offset, void* ptr, void **orig)
{
#if defined(__aarch64__)
    A64HookFunction(offset, ptr, orig);
#else
    A64HookFunction(offset, ptr, orig);
#endif
}
namespace Alex
{
    void Hook(void *target, void *replace, void **backup);
 }
void Alex::Hook(void *target, void *replace, void **backup) {
    unsigned long page_size = sysconf(_SC_PAGESIZE);
    unsigned long size = page_size * sizeof(uintptr_t);
    void *p = (void *) ((uintptr_t) target - ((uintptr_t) target % page_size) - page_size);
    if (mprotect(p, (size_t) size, PROT_EXEC | PROT_READ | PROT_WRITE) == 0) {
        A64HookFunction(target, replace, backup);
    }
}
//for hook a specific libery with 1 methood this is most stable to use 
#define Lib1RealOffset(offset) getAbsoluteAddress("libanogs.so",offset)
#define A64HookFunction(offset, ptr, orig) A64HookFunction((void *)Lib1RealOffset(offset), (void *)ptr, (void **)&orig)

//direct hooks from offsets and simbol strings also they are obfuscated
#define HOOK(offset, ptr, orig) hook((void *)getAbsoluteAddress(targetLibName, string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)

// for str
#define HOOK_LIB(lib, offset, ptr, orig) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)

//for str
#define HOOKSYM(sym, ptr, org) hook(dlsym(dlopen(targetLibName, 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)

// extra hooking example
#define HOOK(target, hook, original) Alex::Hook((void *)(target), (void *)(hook), (void **)(original))




void (*FunctionExample)(void* _this);
void old_FunctionExample(void* _this){
    if (old_FunctionExample != NULL) {
        return;
    }
    return FunctionExample(_this);
}

bool (*_replace_simbol)(void* _this);
bool backup_simbol(void* _this){
    if (backup_simbol != NULL) {
        return false;
    }
    return _replace_simbol(_this);
}



void *hook(void *) {
    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));
    
    ////////               example by calling main funtion         ////////
    
    Alex::Hook((void *)getAbsoluteAddress("libexample.so",0x1234), (void *)FunctionExample, (void **)&old_FunctionExample);
 
    
        /////////////            example 1           ////////// 
       
        
    // hook strings to a defined lib in here its defined by ""targetLibName""
    HOOK("0x12345", FunctionExample, old_FunctionExample);
    
    
    
            /////////////            example 2            ////////// 
    
    //hook from a specific
    HOOK_LIB("libFileB.so", "0x123456", FunctionExample, old_FunctionExample);
    
        /////////////            example 3            ////////// 
        
    //hookings with default libs
    HOOKSYM("SymbolName", FunctionExample, old_FunctionExample);

    
            ///////               example 4          ///////
                                         //flags
      void *Target = dlopen_ex("libexample.so",4); //fake dlopen to get targeted lib
        while (!Target) {
            Target = dlopen_ex("libexample.so", 4); // cheak lib loaded or not
            sleep(1); //for prevent crash
        }
        //hook the targeted lib
        void *target_lib = dlsym_ex(Target, "hookinh simbols here");
        Alex::Hook((void *)target_lib, (void *)_replace_simbol, (void **)&backup_simbol);
         dlclose_ex(Target);
         
       
       return NULL;
  }
  
  
  JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);
    pthread_t t;
    pthread_create(&t, 0, hook, 0);
  
  //  return funtion_name(arguments); // impliment the funtion here which one you want to call after hooks are loaded

}
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {}

//giveme some coffie on telegram @alex5402 :)
