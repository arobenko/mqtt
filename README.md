# Overview
[MQTT](http://mqtt.org/) is a machine-to-machine (M2M)/"Internet of Things" 
connectivity protocol. This project implements all the messages from 
the protocol specification v3.1.1. 

# MQTT Library
The library uses [COMMS library](https://github.com/arobenko/comms_champion#comms-library)
from [comms_champion](https://github.com/arobenko/comms_champion) project 
to specify all the messages resulting in clear, concise,
and very efficient code.

# CommsChampion Plugin
In addition to the library described above, this project provides a protocol
plugin for the [CommsChampion Tool](https://github.com/arobenko/comms_champion#commschampion-tool)
from the same [comms_champion](https://github.com/arobenko/comms_champion) project,
which will allow visual analysis of the mqtt binary protocol traffic.

# Licence
The source code of [MQTT Library](#mqtt-library) as well as [CommsChampion Plugin](#commschampion-plugin) 
from this repository is licensed under **GPLv3**. 
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
sources, and providing path to the install directory of the latter using **CC_INSTALL_PATH**
variable.

>$> cmake -DCMAKE_BUILD_TYPE=Release -DCC_INSTALL_PATH=/path/to/comms_champion/build/install /path/to/mqtt/sources

- Build and install.

>$> make install

After the build is complete, all the binaries, headers, libraries will reside
in **install** subdirectory of the directory chosen for build (/some/build/dir) .

In addition to built-in options/variables of CMake, such as **CMAKE_BUILD_TYPE** or
**CMAKE_TOOLCHAIN_FILE**, the following ones can be used:

- **MQTT_LIB_ONLY**=ON/OFF - Exclude compilation of all the sources, install only
**MQTT** Library. Default value is **OFF**, i.e. the plugin to CommsChampion gets built.

- **MQTT_CC_PLUGIN**=ON/OFF - Include/Exclude plugin for CommsChampion tool.
Default value is **ON**

- **MQTT_CC_PLUGIN_COPY_TO_CC_INSTALL_PATH**=ON/OFF - Install plugin for 
CommsChampion into **CC_INSTALL_PATH** as well as local installation path. 
Default value is **ON**.

For example, discard all other tools, just install the **MQTT** library:

>$> cd /path/to/mqtt/sources

>$> mkdir build && cd build

>$> cmake -DCMAKE_BUILD_TYPE=Release CC_INSTALL_PATH=/path/to/comms_champioin/sources -DMQTT_LIB_ONLY=ON ..

>$> make install 

The example above will skip build of any tool available, it will just install 
the **MQTT** library headers in **install/include** subdirectory

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**
