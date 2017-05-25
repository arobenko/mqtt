function (cc_protocol_plugin_common)
    set (name "${MQTT_PROTOCOL_PLUGIN_COMMON_LIB}")

    set (src
        field.cpp
    )

    add_library (${name} STATIC ${src})
    qt5_use_modules(${name} Core)

endfunction()

######################################################################

cc_protocol_plugin_common ()
