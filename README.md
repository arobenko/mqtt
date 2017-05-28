# Overview
[MQTT](http://mqtt.org/) is a machine-to-machine (M2M)/"Internet of Things" 
connectivity protocol. This project implements all the messages from 
the protocol specification [v3.1.1](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.pdf) 
as well as [v5 (wd13)](https://www.oasis-open.org/committees/download.php/60716/mqtt-v5.0-wd13.pdf). 

# MQTT Protocol Library
The MQTT protocol implementation uses 
[COMMS library](https://github.com/arobenko/comms_champion#comms-library)
from [comms_champion](https://github.com/arobenko/comms_champion) project 
to specify all the messages for both **v3.1.1** and **v5** versions of the protocol,
resulting in clear, concise, and very efficient code.

At this stage the message definition classes are not documented yet, but will
be in the future.

# CommsChampion Plugins
In addition to the library described above, this project provides 3 plugins
for the [CommsChampion Tools](https://github.com/arobenko/comms_champion#commschampion-tools)
from the same [comms_champion](https://github.com/arobenko/comms_champion) project:

- MQTT v3.1.1 Protocol Plugin, which will allow visual analysis of the mqtt **v3.1.1** binary protocol traffic.
- MQTT v5 Protocol Plugin, which will allow visual analysis of the mqtt **v5** binary protocol traffic.
- MQTT (Mosquitto) Socket, which uses [mosquitto](http://mosquitto.org) library to implement data
    exchange with MQTT broker. The socket plugin allows debugging and visual
    analysis of the custom binary protocol messages sent using MQTT transport
    protocol as if they were received/sent over normal I/O socket.
    
There are several wiki pages, that explain how to use the main [CommsChampion](https://github.com/arobenko/comms_champion#commschampion-tools) GUI app to:

 - [analyse MQTT traffic between client and broker](https://github.com/arobenko/mqtt/wiki/Visual-Analysis-of-MQTT-Traffic-Between-Client-and-Broker)
 - [to be a pretend client](https://github.com/arobenko/mqtt/wiki/Using-CommsChampion-GUI-Tool-as-Pretend-MQTT-Client)
 - [to be a pretend broker](https://github.com/arobenko/mqtt/wiki/Using-CommsChampion-GUI-Tool-as-Pretend-MQTT-Broker)
 
 It can provide a significant help when developing new client library or broker application.

# Licence
The [MQTT Protocol Library](#mqtt-protocol-library) as well as [CommsChampion Plugin](#commschampion-plugin) 
from this repository are licensed under
the classic **GPLv3 / Commercial** dual licensing scheme. The
source code is available for anyone to use as long as the derivative work
remains open source with compatible licence. **Download** and **try** it! If it works
as expected and commercial closed source licence is required for the final
product, please send me an e-mail. As the author and full copyright owner I 
will be able to provide one. Please refer
to [Contact Information](#contact-information) below and get in touch with
me if you need one.

# How to Build
Detailed instructions on how to build and install all the components can be
found in [BUILD.md](BUILD.md) file.

# Branching Model
This repository will follow the 
[Successful Git Branching Model](http://nvie.com/posts/a-successful-git-branching-model/).

The **master** branch will always point to the latest release, the
development is performed on **develop** branch. As the result it is safe
to just clone the sources of this repository and use it without
any extra manipulations of looking for the latest stable version among the tags and
checking it out.

# Contact Information
For bug reports, feature requests, or any other question you may open an issue
here in **github** or e-mail me directly to: **arobenko@gmail.com**


