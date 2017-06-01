add_subdirectory (protocol)
add_subdirectory (mosquitto_socket)

FILE(GLOB_RECURSE plugins_headers "*.h")
add_custom_target(mqtt.plugins.headers SOURCES ${plugins_headers})
