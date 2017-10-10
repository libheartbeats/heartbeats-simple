# Release Notes

## v0.3.4 - 2017-10-10

 * Added VERSION and SOVERSION properties to shared object library

## v0.3.3 - 2017-10-02

 * Fixed header documentation for accuracy function parameters
 * Cleaned up warnings from stricter compile flags (mostly in tests)

## v0.3.2 - 2017-02-06

 * Fixed locking in Windows
 * Removed Android CMake toolchain file: cmake-toolchain/android.toolchain.cmake
 * Headers now installed to their own subdirectory (again)
 * Added AppVeyor test configuration for Windows continuous integration
 * Updated README documentation

## v0.3.1 - 2016-10-20

 * Disabled forcing of fPIC - still used by default, but can be overridden

## v0.3.0 - 2016-06-18

 * Collapsed separate libraries into a single library
 * Added a single header file to wrap individual headers
 * Added Windows support
 * Use default CMake library type (static) instead of defaulting to shared object
 * Headers no longer installed to own their own subdirectory

## v0.2.2 - 2016-05-18

 * Fixed instant performance logging

## v0.2.1 - 2016-03-03

 * Initial public release
