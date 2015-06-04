
function (cc_plugin_mqtt)
    set (name "mqtt")
    
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
        field/QoS.cpp
        field/PacketId.cpp
    )
    
    set (hdr
        Plugin.h
        message/Publish.h
    )
    
    
    qt5_wrap_cpp(
        moc
        ${hdr}
    )
    
    add_library (${name} SHARED ${src} ${moc})
    target_link_libraries(${name} Qt5::Widgets Qt5::Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
        
    install (
        TARGETS ${name}
        DESTINATION ${CC_INSTALL_PATH}/plugin)
    
endfunction()

######################################################################

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(
    ${Qt5Core_INCLUDE_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
cc_plugin_mqtt ()
