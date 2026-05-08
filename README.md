# beman.str_split: Stand-alone, easy-to-use string split utilities

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/str_split/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/bemanproject/str_split/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/str_split/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/str_split?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

`beman.str_split` is (... TODO: description).

**Implements**: `std::todo` proposed in [TODO (PnnnnRr)](https://wg21.link/PnnnnRr).

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.str_split` is licensed under the Apache License v2.0 with LLVM Exceptions.

## Usage

TODO

Full runnable examples can be found in [`examples/`](examples/).

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++23 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option `BEMAN_STR_SPLIT_BUILD_TESTS` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler   | Version | C++ Standards | Standard Library  |
|------------|---------|---------------|-------------------|
| GCC        | 16-13   | C++26-C++17   | libstdc++         |
| GCC        | 12-11   | C++23-C++17   | libstdc++         |
| Clang      | 22-19   | C++26-C++17   | libstdc++, libc++ |
| Clang      | 18      | C++26-C++17   | libc++            |
| Clang      | 18      | C++23-C++17   | libstdc++         |
| Clang      | 17      | C++26-C++17   | libc++            |
| Clang      | 17      | C++20, C++17  | libstdc++         |
| AppleClang | latest  | C++26-C++17   | libc++            |
| MSVC       | latest  | C++23         | MSVC STL          |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

## Integrate beman.str_split into your project

### Build

You can build str_split using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.str_split without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

#### Vcpkg

The preferred way to install str_split is via vcpkg. To do so, after installing vcpkg
itself, you need to add support for the Beman project's [vcpkg
registry](https://github.com/bemanproject/vcpkg-registry) by configuring a
`vcpkg-configuration.json` file (which str_split [provides](vcpkg-configuration.json)).

Then, simply run `vcpkg install beman-str-split`.

#### Manual

To install beman.str_split globally after building with the `gcc-release` preset, you can
run:

```bash
sudo cmake --install build/gcc-release
```

Alternatively, to install to a prefix, for example `/opt/beman`, you can run:

```bash
sudo cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure:

```txt
/opt/beman
├── include
│   └── beman
│       └── str_split
│           ├── str_split.hpp
│           └── ...
└── lib
    └── cmake
        └── beman.str_split
            ├── beman.str_split-config-version.cmake
            ├── beman.str_split-config.cmake
            └── beman.str_split-targets.cmake
```

### CMake Configuration

If you installed beman.str_split to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.str_split` package to define the `beman::str_split` CMake
target:

```cmake
find_package(beman.str_split REQUIRED)
```

You will then need to add `beman::str_split` to the link libraries of any libraries or
executables that include `beman.str_split` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::str_split)
```

### Using beman.str_split

To use `beman.str_split` in your C++ project,
include an appropriate `beman.str_split` header from your source code.

```c++
#include <beman/str_split/str_split.hpp>
```

> [!NOTE]
>
> `beman.str_split` headers are to be included with the `beman/str_split/` prefix.
> Altering include search paths to spell the include target another way (e.g.
> `#include <str_split.hpp>`) is unsupported.
