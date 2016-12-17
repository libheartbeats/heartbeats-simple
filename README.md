# Simple Heartbeats

Provides simplified performance, accuracy, and power tracking.

For the original libraries that support shared memory and manage memory and energy readers automatically, see [heartbeats](https://github.com/libheartbeats/heartbeats).

Also see [heartbeats-simple-classic](https://github.com/libheartbeats/heartbeats-simple-classic), which wraps this library and [energymon](https://github.com/energymon/energymon) to provide an interface similar to the original heartbeats design.

## Building

This project uses CMake.

To build, run:

``` sh
mkdir _build
cd _build
cmake ..
make
```

## Installing

To install, run with proper privileges:

``` sh
make install
```

On Linux, installation typically places libraries in `/usr/local/lib` and
header files in `/usr/local/include`.

## Uninstalling

Install must be run before uninstalling in order to have a manifest.

To uninstall, run with proper privileges:

``` sh
make uninstall
```

## Project Source

Find this and related project sources at the [libheartbeats organization on GitHub](https://github.com/libheartbeats).  
This project originates at: https://github.com/libheartbeats/heartbeats-simple

Bug reports and pull requests for bug fixes and enhancements are welcome.
