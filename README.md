# Simple Heartbeats

Provides simplified performance, accuracy, and power tracking.

See
[https://github.com/libheartbeats/heartbeats](https://github.com/libheartbeats/heartbeats)
for the original libraries that support shared memory and manage memory and
energy readers automatically.

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
