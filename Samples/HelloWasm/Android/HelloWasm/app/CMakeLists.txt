# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DNMAIN=1)
include_directories(
        ../../../../../Src/Includes/
        ../../../../../NOMoneCC/Src/Includes/
        ../../../../../NOMoneCC/Src/NOMoneStdLib/Includes/ )

# WASMMachine library,
FILE(GLOB                  SrcCFiles ../../../Src/*.c)
FILE(GLOB          WASMMachineCFiles ../../../../../Src/*.c)
FILE(GLOB WASMMachineAndroidCPPFiles ../../../../../Src/Backends/Android/Cpp/*.cpp)

set(SRC
        ${SrcCFiles}
        ${WASMMachineCFiles}
        ${WASMMachineAndroidCPPFiles})

add_library(WasmMachine STATIC ${SRC})

# NOMoneCC library,
FILE(GLOB                  NCCCFiles ../../../../../NOMoneCC/Src/*.c)
FILE(GLOB      StdLibAndroidCPPFiles ../../../../../NOMoneCC/Src/NOMoneStdLib/Backends/Android/Cpp/*.cpp)
FILE(GLOB         StdLibCommonCFiles ../../../../../NOMoneCC/Src/NOMoneStdLib/*.c)

set(SRC
        ${NCCCFiles}
        ${StdLibAndroidCPPFiles}
        ${StdLibCommonCFiles})

add_library(NOMoneStdLib SHARED ${SRC})

find_library(log-lib log)

target_link_libraries(
        NOMoneStdLib

        # Since WasmMachine is a static library, only the symbols used in NOMoneStdLib are pulled
        # from it. But we intend to use other symbols from the java side, hence, we instruct the
        # linker to grab the whole library,
        -Wl,-whole-archive
        WasmMachine
        -Wl,-no-whole-archive

        android
        ${log-lib})
