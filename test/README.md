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

## Linux
To build the test executable open the `test/test.workspace` file and select
the build configuration (debug/release).

## Windows
To build the test executable open the `test/test.sln` file and select the build
configuration (Debug/Release). Only configurations for the x64 platform are
supported.
