## Clone Submodule
Clone the external repository.
```
git submodule update --init
```

## GoogleTest
Google Test is used for the unit testing framework. See the
[website](https://github.com/google/googletest/) for details.

### Linux
Use the make tool to build the googletest libraries.
```
make -C external/googletest/googletest/make/
```

### Windows
Use Visual Studio to build the googletest libraries.
* Go to the `external/googletest/googletest/msvc` directory.
* Open `gtest.sln`.
* Create a x64 platform configuration by copying the Win32 settings. This is
  required because the Models libraries are built x64.
* Build the solution. There will be a handful of errors, but the important
  library will still be built.

