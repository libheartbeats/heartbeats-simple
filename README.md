# Simple Heartbeats

This library provides simplified performance, accuracy, and power monitoring.

See
[https://github.com/libheartbeats/heartbeats](https://github.com/libheartbeats/heartbeats)
for the original libraries that support shared memory and manage memory and
energy readers automatically.

## Building

To build all libraries, run:

``` sh
make
```

## Installing

Installation places libraries in `/usr/local/lib` and header files in
`/usr/local/include/heartbeats-simple`.

To install all libraries and headers, run with proper privileges:

``` sh
make install
```

## Uninstalling

Install must be run before uninstalling in order to have a manifest.

To remove libraries and headers installed to the system, run with proper
privileges:

``` sh
make uninstall
```

## Cleaning

Note that cleaning will remove the installation manifest.
If you ran the install command and want to remove the installed files,
you must uninstall before cleaning.

The remove generated build configs and artifacts, run:

``` sh
make clean
```
