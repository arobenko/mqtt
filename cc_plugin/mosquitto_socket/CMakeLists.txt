set (THIS_PLUGIN_NAME "mosquitto_socket")

######################################################################

function (cc_plugin_mosquitto_socket)
    set (name "${THIS_PLUGIN_NAME}")
    
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
    )
    
    set (hdr
        Plugin.h
        Socket.h
        SocketConfigWidget.h
    )
    
    qt5_wrap_cpp(
        moc
        ${hdr}
    )
    
    qt5_wrap_ui(
        ui
        SocketConfigWidget.ui
    )
    
    #qt5_add_resources(resources ${CMAKE_CURRENT_SOURCE_DIR}/mosquitto_socket.qrc)
    
    add_library (${name} SHARED ${src} ${moc} ${ui})
    target_link_libraries (${name} ${CC_PLUGIN_LIBRARIES} ${MOSQUITTO_LIBRARIES})
    qt5_use_modules(${name} Network Widgets Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
    
endfunction()

######################################################################

if (NOT "${CC_MQTT_QT_DIR}" STREQUAL "")
    list (APPEND CMAKE_PREFIX_PATH ${CC_MQTT_QT_DIR})
endif ()

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

if (NOT Qt5Core_FOUND)
    message (WARNING "Can NOT compile mosquitto socket plugin due to missing QT5 Core library")
    return ()
endif ()

if (NOT Qt5Widgets_FOUND)
    message (WARNING "Can NOT compile mosquitto socket plugin due to missing QT5 Widgets library")
    return ()
endif ()

if (NOT Qt5Network_FOUND)
    message (WARNING "Can NOT compile mosquitto socket plugin due to missing QT5 Network library")
    return ()
endif ()

list (APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Mosquitto)
if (NOT MOSQUITTO_FOUND)
    message (WARNING "Mosquitto library hasn't been found, socket plugin cannot be built.")
    return()
endif ()

include_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
    ${MOSQUITTO_INCLUDE_DIRS}
)

link_directories(
    ${MOSQUITTO_LIBRARY_DIRS}
)

if (NOT CC_MQTT_FULL_SOLUTION)
    if (EXISTS "${INSTALL_DIR}/cmake/CommsChampionConfig.cmake")
        list (APPEND CMAKE_PREFIX_PATH "${INSTALL_DIR}/cmake")
    endif ()
    
    if (NOT "${CC_MAIN_INSTALL_DIR}" STREQUAL "")
        list (APPEND CMAKE_PREFIX_PATH "${CC_MAIN_INSTALL_DIR}/cmake")
    endif ()
    
    
    find_package(CommsChampion)
    
    if (NOT CC_COMMS_CHAMPION_FOUND)
        message (WARNING "Can NOT compile mqtt protocol plugin due to missing CommsChampion library")
        return ()
    endif ()
    
    include_directories(${CC_INCLUDE_DIRS})
    link_directories(${CC_PLUGIN_LIBRARY_DIRS})
endif ()   

cc_plugin_mosquitto_socket ()

if (CC_MQTT_FULL_SOLUTION)
    add_dependencies(${THIS_PLUGIN_NAME} ${CC_EXTERNAL_TGT})
endif ()
