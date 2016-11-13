# - Find Mqtt protocol definition
# Find the Mqtt included
# 
# Output is provided using the following variables
#  MQTT_FOUND - Found MQTT protocol library, use MQTT_INCLUDE_DIRS for includes
#  MQTT_INCLUDE_DIRS - Where to find all the headers

get_filename_component (MQTT_INSTALL_PATH ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

find_path(MQTT_INCLUDE_DIR NAMES mqtt/Message.h PATHS "${MQTT_INSTALL_PATH}" PATH_SUFFIXES include)

find_package(PackageHandleStandardArgs REQUIRED)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MQTT REQUIRED_VARS MQTT_INCLUDE_DIR)
    
if (MQTT_FOUND)
    set (MQTT_INCLUDE_DIRS ${MQTT_INCLUDE_DIR})
endif ()

