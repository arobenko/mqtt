if (NOT CC_COMMS_CHAMPION_FOUND)
    message (WARNING "Can NOT compile mqtt protocol plugins due to missing CommsChampion library")
    return ()
endif ()

if (NOT "${CC_MQTT_QT_DIR}" STREQUAL "")
    list (APPEND CMAKE_PREFIX_PATH ${CC_MQTT_QT_DIR})
endif ()

find_package(Qt5Core)

if (NOT Qt5Core_FOUND)
    message (WARNING "Can NOT compile mqtt protocol plugins due to missing QT5 Core library")
    return ()
endif ()

if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
endif ()

include_directories(${CC_INCLUDE_DIRS})
link_directories(${CC_PLUGIN_LIBRARY_DIRS})

set (MQTT_PROTOCOL_PLUGIN_COMMON_LIB "mqtt_plugin_common")

add_subdirectory(common)
add_subdirectory(v311)
add_subdirectory(v5)

FILE(GLOB_RECURSE headers "include/*.h")
add_custom_target(mqtt.cc_plugin.protocol.headers SOURCES ${headers})
