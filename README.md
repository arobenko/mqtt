# Overview
[MQTT](http://mqtt.org/) is a machine-to-machine (M2M)/"Internet of Things" 
connectivity protocol. This project implements all the messages from 
the protocol specification v3.1.1. 

# MQTT Library
The MQTT protocol implementation uses 
[COMMS library](https://github.com/arobenko/comms_champion#comms-library)
from [comms_champion](https://github.com/arobenko/comms_champion) project 
to specify all the messages resulting in clear, concise,
and very efficient code.

# CommsChampion Plugins
In addition to the library described above, this project provides two plugins
for the [CommsChampion Tools](https://github.com/arobenko/comms_champion#commschampion-tools)
from the same [comms_champion](https://github.com/arobenko/comms_champion) project:

- MQTT Protocol Plugin, which will allow visual analysis of the mqtt binary protocol traffic.
- MQTT (Mosquitto) Socket, which uses [mosquitto](http://mosquitto.org) library to implement data
    exchange with MQTT broker. The socket plugin allows debugging and visual
    analysis of the custom binary protocol messages sent using MQTT transport
    protocol as if they were received/sent over normal I/O socket.

# Licence
The source code of [MQTT Library](#mqtt-library) as well as souce code of
[CommsChampion Plugins](#commschampion-plugins) 
from this repository are licensed under **GPLv3**. 
It can be used in any product as long as its sources remain open and
have the same or other compatible license. As the author and full copyright
owner of this product I can provide a commercial license as well, please refer
to [Contact Information](#contact-information) below and get in touch with
me if you need one.

# How to Build
This project uses [CMake](https://cmake.org) cross-platform build system to
generate required build files native to the platform.

- Build and install all the necessary components from the 
[comms_champion](https://github.com/arobenko/comms_champion) project following
[instructions](https://github.com/arobenko/comms_champion#how-to-build).

- Create a separate build directory for this project somewhere and navigate there.

>$> mkdir /some/build/dir && cd /some/build/dir

- Generate Makefiles (or any other build environment) choosing the same build type
as when building the [comms_champion](https://github.com/arobenko/comms_champion)
sources. The build depends on the installed contents of the 
[comms_champion](https://github.com/arobenko/comms_champion) project
and requires knowledge about the location of the latter. It is recommended to
install the produced headers/binaries of this project into the same directory
as with [comms_champion](https://github.com/arobenko/comms_champion), it will cause
all the required dependencies to be found automatically. The installation
directory can be specified using **CC_MQTT_INSTALL_DIR** variable.

>$> cmake -DCMAKE_BUILD_TYPE=Release -DCC_MQTT_INSTALL_DIR=/path/to/comms_champion/build/install /path/to/mqtt/sources

If the produced headers/finaries are not desired to be mixed, the location
of the [comms_champion](https://github.com/arobenko/comms_champion)
installation path can be provided using **CC_MAIN_INSTALL_DIR** variable.

>$> cmake -DCMAKE_BUILD_TYPE=Release -DCC_MAIN_INSTALL_DIR=/path/to/comms_champion/build/install /path/to/mqtt/sources

- Build and install.

>$> make install

After the build is complete, all the binaries, headers, libraries will reside
in the chosen install directory (See description of **CC_MQTT_INTALL_DIR** variable below) .

In addition to built-in options/variables of CMake, such as **CMAKE_BUILD_TYPE** or
**CMAKE_TOOLCHAIN_FILE**, the following ones can be used:

- **CC_MQTT_LIB_ONLY**=ON/OFF - Exclude compilation of all the sources, install only
**MQTT** Library. Default value is **OFF**, i.e. the pluginS to CommsChampion get built.

- **CC_MQTT_INSTALL_DIR**=dir - Custom installation directory. If not provided defaults to
**install** subdirectory of the directory chosen for build (**${CMAKE_BINARY_DIR}/install**). 

- **CC_MAIN_INSTALL_DIR**=dir - Directory where headers and libraries of 
[comms_champion](https://github.com/arobenko/comms_champion) project. It must
be used if value of **CC_MQTT_INSTALL_DIR** doesn't specify the same location.

- **CC_MQTT_QT_DIR**=dir - Path to custom build of **QT5** if it cannot be
found in standard system directories.

- **CC_MQTT_MOSQUITTO_DIR**=dir - Path to custom build of **mosquitto**
library if it cannot be found in standard system directories.

- **CC_MQTT_FULL_SOLUTION**=ON/OFF - This option allows to build both
[comms_champion](https://github.com/arobenko/comms_champion) and this projects
in one go. When enabled it will checkout and build the 
[comms_champion](https://github.com/arobenko/comms_champion) prior to building
anything from this repository. All the headers, binaries, and libraries will
be installed in the directory specified with **CC_MQTT_INSTALL_DIR** variable. 
Default value of this option is **OFF**.

For example, discard all other tools, just install the **MQTT** library:

>$> cd /path/to/mqtt/sources

>$> mkdir build && cd build

>$> cmake -DCMAKE_BUILD_TYPE=Release -DCC_MQTT_LIB_ONLY=ON ..

>$> make install 

The example above will skip build of any tool available, it will just install 
the **MQTT** library headers in **install/include** subdirectory

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**

