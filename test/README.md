OTLS-Models uses the Google Test framework for unit testing.
For more information, see the [website](https://code.google.com/p/googletest/).

## Build
Clone the googletest repository to the external folder.
```
git submodule update --init
```

### Linux
Use the make tool to build the googletest libraries.
```
make ../external/googletest/googletest/make/
```

### Windows
Use msvc to build the googletest libraries.
* Convert the Visual Studio project files to the latest version.
* Create a x64 platform configuration by copying the Win32 settings. This is
  required because the Models libraries are built x64.
* Change the runtime libraries for each project. This is required because the
  Models libraries are built to be compatible with a wxWidgets application.
 * Debug - Multi-threaded Debug DLL (/MDd)
 * Release - Multi-threaded DLL (/MD)
* Build the solution.
