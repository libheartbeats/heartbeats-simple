# Release Notes

## [Unreleased]


## [v0.3.6] - 2017-11-13
### Added

* Multiarch support using GNU standard installation directories


## [v0.3.5] - 2017-10-10
### Fixed

* Fixed "MSYS Makefiles" generator for CMake builds on Windows


## [v0.3.4] - 2017-10-10
### Added

* VERSION and SOVERSION properties to shared object library


## [v0.3.3] - 2017-10-02
### Added

* Added AUTHORS and RELEASES.md files

### Fixed

* Fixed header documentation for accuracy function parameters
* Cleaned up warnings from stricter compile flags (mostly in tests)


## [v0.3.2] - 2017-02-06
### Added

* Added AppVeyor test configuration for Windows continuous integration

### Changed

* Headers now installed to their own subdirectory (again)
* Updated README documentation

### Removed

* Removed Android CMake toolchain file: cmake-toolchain/android.toolchain.cmake

### Fixed

* Fixed locking in Windows


## [v0.3.1] - 2016-10-20
### Changed

* Disabled forcing of fPIC - still used by default, but can be overridden


## [v0.3.0] - 2016-06-18
### Added

* Added a single header file to wrap individual headers
* Windows support

### Changed

* Collapsed separate libraries into a single library
* Use default CMake library type (static) instead of defaulting to shared object
* Headers no longer installed to own their own subdirectory


## [v0.2.2] - 2016-05-18
### Fixed

* Fixed instant performance logging


## v0.2.1 - 2016-03-03
### Added

* Initial public release


[Unreleased]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.6...HEAD
[v0.3.6]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.5...v0.3.6
[v0.3.5]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.4...v0.3.5
[v0.3.4]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.3...v0.3.4
[v0.3.3]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.2...v0.3.3
[v0.3.2]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.1...v0.3.2
[v0.3.1]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.3.0...v0.3.1
[v0.3.0]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.2.2...v0.3.0
[v0.2.2]: https://github.com/libheartbeats/heartbeats-simple/compare/v0.2.1...v0.2.2
