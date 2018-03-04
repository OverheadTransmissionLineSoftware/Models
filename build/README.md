This directory contains files for building the libraries.

General Notes:
* Each subdirectory contains a different set of build files.
* All final binaries are placed in the `lib` directory.

## CMake
CMake is used to generate makefiles.

For the debug configuration:
```
mkdir <repository_root>/build-debug
cd <repository_root>/build-debug
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../build/cmake
cmake --build .
```

For the release configuration:
```
mkdir <repository_root>/build-release
cd <repository_root>/build-release
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../build/cmake
cmake --build .
```

## Codeblocks
Code::Blocks is the primary IDE for Linux. These files are manually maintained
even though CMake can generate them. Codeblocks can also be used for
building on Windows, although the project files are not currently configured
for this. For more information, see the [website](http://codeblocks.org).

To build the OTLS-Models libraries open the
`build/codeblocks/otlsmodels.workspace` file and select the build configuration
(debug/release).

## Visual Studio
Visual Studio is the primary IDE for Windows. These files are manually maintained
even though CMake can generate them. The project files are set up for use with
the latest community edition version. For more information, see the
[website](https://www.visualstudio.com/en-us/products/visual-studio-community-vs.aspx).

To build the OTLS-Models libraries open the `build/msvc/otlsmodels.sln` file and
select the build configuration (Debug/Release). Only configurations for the x64
platform are supported.
