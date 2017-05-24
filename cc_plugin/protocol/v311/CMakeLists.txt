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
        message/Subscribe.cpp
        message/Unsubscribe.cpp
        message/Unsuback.cpp
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
    target_link_libraries (${name} ${MQTT_PROTOCOL_PLUGIN_COMMON_LIB} ${CC_PLUGIN_LIBRARIES})
    qt5_use_modules(${name} Core)
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})

endfunction()

######################################################################

cc_plugin_mqtt ()

if (CC_MQTT_FULL_SOLUTION)
    add_dependencies(${THIS_PLUGIN_NAME} ${CC_EXTERNAL_TGT})
endif ()
