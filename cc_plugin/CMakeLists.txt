
function (cc_plugin_mqtt)
    set (name "mqtt")
    set (src
#        DemoPlugin.cpp
#        Protocol.cpp
        message/Connack.cpp
    )
    
#    set (hdr
#        DemoPlugin.h
#    )
    
    
#    qt5_wrap_cpp(
#        moc
#        ${hdr}
#    )
    
    add_library (${name} SHARED ${src} ${moc})
    
    install (
        TARGETS ${name}
        DESTINATION ${PLUGIN_INSTALL_DIR})
    
endfunction()

######################################################################

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(
    ${Qt5Core_INCLUDE_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
cc_plugin_mqtt ()
