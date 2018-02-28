## Clone Submodules
Clones the external repositories.
```
git submodule update --init
```

## GoogleTest
Google Test is used for the unit testing framework. See the
[website](https://github.com/google/googletest/) for details.

### Linux
Builds the googletest libraries with `make`.
```
make -C external/googletest/googletest/make/
```

### Windows
To build the googletest libraries with Visual Studio:
* Go to the `external/googletest/googletest/msvc` directory.
* Open `gtest.sln`.
* Create a x64 platform configuration by copying the Win32 settings. This is
  required because the Models libraries are built x64.
* Build the solution. There will be a handful of errors, but the important
  library will still be built.

## Style Guide
The style guide has a tool to check the code formatting. See the `check`
directory.
