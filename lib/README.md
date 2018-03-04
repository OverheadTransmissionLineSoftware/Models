This is the output directory for the compiled libraries.

The final binaries are located here so external apps can reference a single
directory. Multiple options are available for building the libraries. See the
`build` directory for details.

Library Naming:
* Libraries contain platform-specific prefixes (e.g. libXXXXX on unix).
* Libraries contain platform-specific suffixes (e.g. *.lib and *.a).
* Debug builds have a `d` appended to the library name.
