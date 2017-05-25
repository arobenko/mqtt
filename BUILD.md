# How to Build

This project uses [CMake](https://cmake.org) cross-platform build system to
generate required build files native to the platform.

## Available CMake Options

In addition to built-in options/variables of CMake, such as **CMAKE_BUILD_TYPE** or
**CMAKE_TOOLCHAIN_FILE**, the following ones can be used:

- **CC_MQTT_LIB_ONLY**=ON/OFF - Exclude compilation of all the sources, install only
**MQTT Protocol** Library. Default value is **OFF**, i.e. the pluginS to **CommsChampion** get built.

- **CC_MQTT_FULL_SOLUTION**=ON/OFF - This option allows to build both
[comms_champion](https://github.com/arobenko/comms_champion) and this projects
in one go. When enabled, it will checkout and build the 
[comms_champion](https://github.com/arobenko/comms_champion) prior to building
anything from this repository. All the headers, binaries, and libraries will
be installed in the directory specified with **CC_MQTT_INSTALL_DIR** variable. 
Default value of this option is **OFF**.

- **CC_MQTT_INSTALL_DIR**=dir - Custom installation directory. If not provided defaults to
**install** subdirectory of the directory chosen for build (**${CMAKE_BINARY_DIR}/install**). 

- **CC_MAIN_INSTALL_DIR**=dir - The provided **MQTT Protocol** library depends on 
artefacts created and installed by 
[comms_champion](https://github.com/arobenko/comms_champion) project. If
the latter is already built separately, the path to it needs to be provided using
this variable. If this project and 
[comms_champion](https://github.com/arobenko/comms_champion) share the same
installation directory (provided by **CC_MQTT_INSTALL_DIR**), the usage of
this variable is not necessary.

- **CC_MQTT_QT_DIR**=/path/to/qt - Path to custom build of **QT5** if it cannot be
found in standard system directories. The **QT5** is used by the
[comms_champion](https://github.com/arobenko/comms_champion) tools and it is
required to build the protocol plugin for them.

- **CC_MQTT_NO_WARN_AS_ERR**=ON/OFF - By default, all warnings are treated as
errors. Enable this option in case the compiler generates warning and fails the
compilation. Please open the issue when such scenario occurs. Default value is 
**OFF**.

## Choosing C++ Standard

Since CMake v3.1 it became possible to set version of C++ standard by setting
**CMAKE_CXX_STANDARD** variable. If no value of this variable is set in command
line arguments, default value **11** will be assigned to it. In order to use
c++14 standard in compilation, set the variable value to **14**. 

Please **NOTE**, that _clang_ compiler has problems compiling valid c++11 constructs
used in this project. Hence, the compilation will fail unless the compilation is
configured to use c++14 standard.

## Build and Install Examples

The examples below are Linux/Unix system oriented, i.e. they use **make** utility
to build the "install" target after configuration with **cmake**. 

On Windows
systems with Visual Studio compiler, the CMake utility generates Visual Studio
solution files by default. Build "install" project. It is also possible to 
generate Makefile-s on Windows by providing additional **-G "NMake Makefiles"** option
to **cmake**. In this case use **nmake** utility instead of **make**.

Please review the examples below and use appropriate option that suites your
needs. Remember to add **-DCMAKE_BUILD_TYPE=Release** option for release
builds.

### Full Solution
If you are not familiar with [comms_champion](https://github.com/arobenko/comms_champion) 
project and its tools, it is recommended to use **CC_MQTT_FULL_SOLUTION**
option to force full build with all the provided libraries, tools and plugins to
them. The "full solution" build is intended to be built and used on the 
development PC with QT5 libraries installed. 

```
$> cd /path/to/mqtt/sources
$> mkdir build && cd build
$> cmake -DCMAKE_BUILD_TYPE=Release -DCC_MQTT_FULL_SOLUTION=ON ..
$> make install 
```

### Full Solution with Clang Compiler
**NOTE**, that c++14 standard needs to be enabled for successful compilation.
```
$> cd /path/to/mqtt/sources
$> mkdir build && cd build
$> CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release -DCC_MQTT_FULL_SOLUTION=ON -DCMAKE_CXX_STANDARD=14 ..
$> make install 
```

### MQTT Protocol Library Only
If the [COMMS](https://github.com/arobenko/comms_champion#comms-library) library
already installed and used to define some other protocols, there is no need to
install it again. Use **CC_MQTT_LIB_ONLY** option to install only headers of
the MQTT Protocol library.
```
$> cd /path/to/mqtt/sources
$> mkdir build && cd build
$> cmake -CC_MQTT_LIB_ONLY=ON ..
$> make install 
```

### Building Plugins
If the [comms_champion](https://github.com/arobenko/comms_champion) tools are
built and installed separately, use **CC_MAIN_INSTALL_DIR** option to specify
its installation directory.

```
$> cd /path/to/mqtt/sources
$> mkdir build && cd build
$> cmake -DCMAKE_BUILD_TYPE=Release -CC_MAIN_INSTALL_DIR=/path/to/comms_champion/build/install ..
$> make install 
```

Note, that the example above will not install the plugin to the relevant directory
searched by [comms_champion](https://github.com/arobenko/comms_champion) tools.
The created plugin needs to be copied manually. 

It is possible to make the built plugin to be installed into the right place by
installing the artefacts to the same directory as **comms_champion** by 
using **CC_MQTT_INSTALL_DIR** variable instead.

```
$> cd /path/to/mqtt/sources
$> mkdir build && cd build
$> cmake -DCMAKE_BUILD_TYPE=Release -CC_MQTT_INSTALL_DIR=/path/to/comms_champion/build/install ..
$> make install 
```

