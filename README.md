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

## Linking

If your project uses CMake, you can find the `HeartbeatsSimple` package and
link with the `heartbeats-simple` library target in that namespace, e.g.:

``` cmake
find_package(HeartbeatsSimple REQUIRED)
target_link_libraries(foo PRIVATE HeartbeatsSimple::heartbeats-simple)
```

Otherwise, get flag and linker information with `pkg-config`, e.g.:

``` sh
pkg-config --cflags heartbeats-simple
pkg-config --libs heartbeats-simple
```

## Example

The following is a simple heartbeats example, ignoring error checks and
additional `#include` statements, which are platform-specific.

```C
#include <heartbeats-simple.h>

void do_application_loop(uint64_t iterations, uint64_t window_len) {
  heartbeat_context hb;
  heartbeat_record* window_buf = malloc(window_len * sizeof(heartbeat_record));
  int fd = open("heartbeat.log", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  heartbeat_init(&hb, window_len, window_buf, fd, NULL);
  hb_ctx_log_header(&hb);

  for (uint64_t i = 0; i < iterations; i++) {
    uint64_t start_time = get_time_in_ns();
    do_work();
    uint64_t end_time = get_time_in_ns();
    heartbeat(&hb, i, 1, start_time, end_time);
  }

  hb_ctx_log_window_buffer(&hb); // flush remaining window data to log file
  close(fd);
  free(window_buf);
}
```

## Project Source

Find this and related project sources at the [libheartbeats organization on GitHub](https://github.com/libheartbeats).  
This project originates at: https://github.com/libheartbeats/heartbeats-simple

Bug reports and pull requests for bug fixes and enhancements are welcome.
