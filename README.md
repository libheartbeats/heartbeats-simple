This project is under active development.  
Things are likely to break and APIs are subject to change without warning.  
Use at your own risk!

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

To install all libraries, run:

``` sh
sudo make install
```

## Uninstalling

To remove libraries and headers installed to the system, run:

``` sh
sudo make uninstall
```
