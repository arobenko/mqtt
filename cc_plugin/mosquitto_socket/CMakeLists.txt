if (NOT MQTT_CC_MOSQUITTO_SOCKET_PLUGIN)
    return ()
endif ()
######################################################################

function (cc_plugin_mosquitto_socket)
    set (name "mosquitto_socket")
    
    if (NOT Qt5Core_FOUND)
        message(WARNING "Can NOT build ${name} due to missing Qt5Core library")
        return()
    endif ()
    
    if (NOT Qt5Widgets_FOUND)
        message(WARNING "Can NOT build ${name} due to missing Qt5Widgets library")
        return()
    endif ()
    
    if (NOT Qt5Network_FOUND)
        message(WARNING "Can NOT build ${name} due to missing Qt5Network library")
        return()
    endif ()
    
    set (meta_file "${CMAKE_CURRENT_SOURCE_DIR}/mosquitto_socket.json")
    set (stamp_file "${CMAKE_CURRENT_BINARY_DIR}/refresh_stamp.txt")
    
    set (refresh_plugin_header TRUE)
    if ((NOT EXISTS ${stamp_file}) OR (${meta_file} IS_NEWER_THAN ${stamp_file}))
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_SOURCE_DIR}/Plugin.h)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E touch ${stamp_file})
    endif ()
    
    
    set (src
        Plugin.cpp
        Socket.cpp
        SocketConfigWidget.cpp
        ConnectAction.cpp
    )
    
    set (hdr
        Plugin.h
        Socket.h
        SocketConfigWidget.h
        ConnectAction.h
    )
    
    qt5_wrap_cpp(
        moc
        ${hdr}
    )
    
    qt5_wrap_ui(
        ui
        SocketConfigWidget.ui
    )
    
    qt5_add_resources(resources ${CMAKE_CURRENT_SOURCE_DIR}/mosquitto_socket.qrc)
    
    add_library (${name} SHARED ${src} ${moc} ${ui} ${resources})
    target_link_libraries (${name} ${CC_PLUGIN_LIBRARIES} ${MOSQUITTO_LIBRARIES})
    qt5_use_modules(${name} Network Widgets Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})

    if (MQTT_CC_PLUGIN_COPY_TO_CC_INSTALL_PATH)        
        install (
            TARGETS ${name}
            DESTINATION ${CC_PLUGIN_DIR})
     endif ()
    
endfunction()

######################################################################

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Mosquitto)
if (NOT MOSQUITTO_FOUND)
    message (WARNING "Mosquitto library hasn't been found, socket plugin cannot be built.")
    return()
endif ()

include_directories(${MOSQUITTO_INCLUDE_DIRS})
link_directories(${MOSQUITTO_LIBRARY_DIRS})

if (NOT "${MQTT_QT_DIR}" STREQUAL "")
    list (APPEND CMAKE_PREFIX_PATH ${MQTT_QT_DIR})
endif ()

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

if (NOT "${MQTT_CC_INSTALL_PATH}" STREQUAL "")
    list (APPEND CMAKE_PREFIX_PATH "${MQTT_CC_INSTALL_PATH}/cmake")
endif ()

find_package(CommsChampion)

if (NOT CC_COMMS_CHAMPION_FOUND)
    message (FATAL_ERROR "CommsChampion wasn't found!. Please set MQTT_CC_INSTALL_PATH to its installation path.")
endif ()

include_directories("${CC_INCLUDE_DIRS}")
link_directories("${CC_PLUGIN_LIBRARY_DIRS}")

if (CMAKE_COMPILER_IS_GNUCC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
endif ()

cc_plugin_mosquitto_socket ()
