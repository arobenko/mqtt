install (
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/mqtt
    DESTINATION ${INC_INSTALL_DIR}
)


FILE(GLOB_RECURSE headers "include/*.h")
add_custom_target(mqtt.headers SOURCES ${headers})

