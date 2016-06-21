# - Find Mosquitto library
# Find the mosquitto library includes and libraries
#
# Input variables
#  MQTT_MOSQUITTO_DIR - Directory with mosquitto installation.
# 
# Output is provided using the following variables
#  MOSQUITTO_FOUND - Found mosquitto library
#  MOSQUITTO_LIBRARIES - Libraries to link against.
#  MOSQUITTO_INCLUDE_DIRS - Directories that contain header files.
#  MOSQUITTO_LIBRARY_DIRS - Directories that contain libraries.

if ("${MQTT_MOSQUITTO_DIR}" STREQUAL "")
    find_path(MOSQUITTO_INCLUDE_DIR NAMES mosquitto.h)
    find_library(MOSQUITTO_LIBRARY NAMES mosquitto)
else ()
    find_path(MOSQUITTO_INCLUDE_DIR NAMES mosquitto.h PATHS "${MQTT_MOSQUITTO_ROOT_DIR}" PATH_SUFFIXES include)
    find_library(MOSQUITTO_LIBRARY NAMES mosquitto PATHS "${MQTT_MOSQUITTO_ROOT_DIR}" PATH_SUFFIXES lib)
endif ()

if (MOSQUITTO_LIBRARY)
    get_filename_component (MOSQUITTO_LIBRARY_DIR ${MOSQUITTO_LIBRARY} DIRECTORY)
endif ()

find_package(PackageHandleStandardArgs REQUIRED)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MOSQUITTO REQUIRED_VARS 
    MOSQUITTO_INCLUDE_DIR MOSQUITTO_LIBRARY MOSQUITTO_LIBRARY_DIR)

if(MOSQUITTO_FOUND)
  set(MOSQUITTO_LIBRARIES ${MOSQUITTO_LIBRARY})
  set(MOSQUITTO_INCLUDE_DIRS ${MOSQUITTO_INCLUDE_DIR})
  set(MOSQUITTO_LIBRARY_DIRS ${MOSQUITTO_LIBRARY_DIR})
endif()

