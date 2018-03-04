The files in this directory will build a console application and run several
unit tests. This is a preventative action so that new code commits do not
introduce problems. It also allows the classes to be debugged easier by
removing some of the call stack levels introduced with a GUI application.
The unit tests do not cover every possible situation, but should catch any big
errors.

## Build Dependencies
Build the OTLS-Models libraries.
* See the `build` directory for instructions.

Build the GoogleTest libraries.
* See the `external` directory for instructions.

## CMake
To build the debug executable:
```
mkdir <repository_root>/build-debug
cd <repository_root>/build-debug
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DMODELS_BUILD_TESTS=True ../build/cmake
cmake --build .
```

To build the release executable:
```
mkdir <repository_root>/build-release
cd <repository_root>/build-release
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DMODELS_BUILD_TESTS=True ../build/cmake
cmake --build .
```

The executable will be located at: `build-<configuration>/test/test`

## Codeblocks
To build the test executable open the `test/test.workspace` file and select
the build configuration (debug/release). The executable will be located at:
`test/<configuration>/test`.

## Visual Studio
To build the test executable open the `test/test.sln` file and select the build
configuration (Debug/Release). Only configurations for the x64 platform are
supported. The executable will be located at: `test/<configuration>/test.exe`.
