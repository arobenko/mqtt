set (THIS_PLUGIN_NAME "mqtt_v311_protocol")

######################################################################

function (cc_plugin_mqtt)
    set (name "${THIS_PLUGIN_NAME}")
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/mqtt.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/mqtt_refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    
    set (src
        Plugin.cpp
        Protocol.cpp
        TransportMessage.cpp
        message/Connect.cpp
        message/Connack.cpp
        message/Publish.cpp
        message/Puback.cpp
        message/Pubrec.cpp
        message/Pubrel.cpp
        message/Pubcomp.cpp
        message/Subscribe.cpp
        message/Suback.cpp
        message/Unsubscribe.cpp
        message/Unsuback.cpp
        message/Pingreq.cpp
        message/Pingresp.cpp
        message/Disconnect.cpp
        field.cpp
    )
    
    set (hdr
        Plugin.h
    )
    
    
    qt5_wrap_cpp(
        moc
        ${hdr}
    )
    
    add_library (${name} SHARED ${src} ${moc})
    target_link_libraries (${name} ${CC_PLUGIN_LIBRARIES})
    qt5_use_modules(${name} Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})

endfunction()

######################################################################

if (NOT CC_COMMS_CHAMPION_FOUND)
    message (WARNING "Can NOT compile mqtt protocol plugin due to missing CommsChampion library")
    return ()
endif ()

if (NOT "${CC_MQTT_QT_DIR}" STREQUAL "")
    list (APPEND CMAKE_PREFIX_PATH ${CC_MQTT_QT_DIR})
endif ()

find_package(Qt5Core)

if (NOT Qt5Core_FOUND)
    message (WARNING "Can NOT compile mqtt protocol plugin due to missing QT5 Core library")
    return ()
endif ()

if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
endif ()

include_directories(${CC_INCLUDE_DIRS})
link_directories(${CC_PLUGIN_LIBRARY_DIRS})

cc_plugin_mqtt ()

if (CC_MQTT_FULL_SOLUTION)
    add_dependencies(${THIS_PLUGIN_NAME} ${CC_EXTERNAL_TGT})
endif ()
