# tftp_common

A simple header-only Trivial File Transfer Protocol (TFTP) packets parsing and serialization library.

![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue)
[![Build and test status](https://img.shields.io/github/actions/workflow/status/eoan-ermine/tftp_common/build_and_test.yml)](https://github.com/eoan-ermine/tftp_common/actions/workflows/build_and_test.yml)
[![Docs build and deployment status](https://img.shields.io/github/actions/workflow/status/eoan-ermine/tftp_common/build_docs.yml?label=docs)](https://github.com/eoan-ermine/tftp_common/actions/workflows/build_docs.yml)
[![Link to doxygen documentation](https://img.shields.io/badge/-doxygen%20documentation-blue)](https://eoanermine.com/tftp_common/)

## Supported platforms

|               |                          |               |               |
|---------------|--------------------------|---------------|---------------|
| **Platform name** | Windows | Linux | MacOS |
| **Minimum required version** | 98 | --- | --- |

## Dependencies

| Dependency name         | Minimum required version | Ubuntu 22.04                         |
|-------------------------|--------------------------|--------------------------------------|
| C++                     | C++17                    | sudo apt-get install build-essential |
| Doxygen (optional)      | ---                      | sudo apt-get install doxygen         |

## Quick start

1. Download and install CMake. Version 3.12.0 is the minimum required.
2. Open a shell. Your development tools must be reachable from this shell through the PATH environment variable.
3. Create a build directory, go to this directory:
```shell
mkdir build
cd build
```
4. Execute this command in the shell replacing `path/to/tftp_common/source/root` with the path to the root of your tftp_common source tree:
```shell
cmake path/to/tftp_common/source/root
```
5. After CMake has finished running, proceed to use IDE project files, or start the build from the build directory:
```shell
cmake --build .
```
The --build option tells cmake to invoke the underlying build tool (make, ninja, xcodebuild, msbuild, etc.)

The underlying build tool can be invoked directly, of course, but the --build option is portable.

6. After tftp_common has finished building, install it from the build directory:
```shell
cmake --build . --target install
```

The --target option with install parameter in addition to the --build option tells cmake to build the install target.

## Options and variables

Variables customize how the build will be generated. Options are boolean variables, with possible values ON/OFF. Options and variables are defined on the CMake command line like this:

```shell
cmake -DVARIABLE=value path/to/tftp_common/source
```

## CMake variables

* `BUILD_DOCS: BOOL`

Adds doxygen build target as a dependency of the default build target. Defaults to OFF.

* `BUILD_TESTS: BOOL`

Adds test build targets as a dependencies of the default build target. Defaults to OFF.

* `BUILD_EXAMPLES: BOOL`

Adds examples build targets as a dependencies of the default build target. Defaults to OFF.
