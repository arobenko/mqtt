cmake_minimum_required (VERSION 3.1)
project ("mqtt")

option (CC_MQTT_LIB_ONLY "Install only MQTT protocol library, no other applications/plugings are built." OFF)
option (CC_MQTT_FULL_SOLUTION "Build and install full solution, including CommsChampion sources." OFF)
option (CC_MQTT_NO_WARN_AS_ERR "Do NOT treat warning as error" OFF)
option (CC_NO_MOSQUITTO_SOCKET "Exclude build of mosquitto socket" OFF)

if (NOT CMAKE_CXX_STANDARD)
    set (CMAKE_CXX_STANDARD 11)
endif()

if ((CMAKE_COMPILER_IS_GNUCC) OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    set (extra_flags_list
        "-Wall" "-Wextra" "-Wcast-align" "-Wcast-qual" "-Wctor-dtor-privacy"
        "-Wmissing-include-dirs"
        "-Woverloaded-virtual" "-Wredundant-decls" "-Wshadow" "-Wundef" "-Wunused"
        "-Wno-unknown-pragmas" "-fdiagnostics-show-option"
    )

    if (CMAKE_COMPILER_IS_GNUCC)
        list (APPEND extra_flags_list
            "-Wnoexcept" "-Wlogical-op" "-Wstrict-null-sentinel"
        )
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND extra_flags_list "-Wno-dangling-field" "-Wno-unused-command-line-argument")
    endif ()

    if (NOT CC_MQTT_NO_WARN_AS_ERR)
        list (APPEND extra_flags_list "-Werror")
    endif ()

    string(REPLACE ";" " " extra_flags "${extra_flags_list}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${extra_flags}")
elseif (MSVC)
    add_definitions( "/wd4503" "/wd4309" "/wd4267" "-D_SCL_SECURE_NO_WARNINGS" "/bigobj")
    
    if (NOT CC_NO_WARN_AS_ERR)
        add_definitions("/WX")
    endif ()
endif ()

set (INSTALL_DIR ${CMAKE_BINARY_DIR}/install)
if (NOT "${CC_MQTT_INSTALL_DIR}" STREQUAL "")
    set (INSTALL_DIR "${CC_MQTT_INSTALL_DIR}")
endif ()

include(GNUInstallDirs)
set (INSTALL_NAME "CC_Mqtt")
set (LIB_INSTALL_DIR ${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR})
set (BIN_INSTALL_DIR ${INSTALL_DIR}/${CMAKE_INSTALL_BINDIR})
set (INC_INSTALL_DIR ${INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
set (CONFIG_INSTALL_DIR ${INSTALL_DIR}/config)
set (PLUGIN_INSTALL_DIR ${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/CommsChampion/plugin)
set (DOC_INSTALL_DIR ${DATA_INSTALL_DIR}/doc)

install (DIRECTORY cmake DESTINATION ${LIB_INSTALL_DIR}/${INSTALL_NAME})

add_subdirectory (protocol)

if (CC_MQTT_LIB_ONLY)
    return ()
endif ()   

set (CC_EXTERNAL_TGT "comms_champion_external")
if (CC_MQTT_FULL_SOLUTION)
    include(ExternalProject)
    
    set (cc_branch "v1.3")
    set (cc_main_dir "${CMAKE_BINARY_DIR}/comms_champion")
    set (cc_src_dir "${cc_main_dir}/src")
    set (cc_bin_dir "${cc_main_dir}/build")
    
    if (NOT "${CC_MQTT_QT_DIR}" STREQUAL "")
        set (extra_cc_args "-DCC_QT_DIR=${CC_MQTT_QT_DIR}")
    endif ()

    ExternalProject_Add(
        "${CC_EXTERNAL_TGT}"
        PREFIX "${cc_bin_dir}"
        STAMP_DIR "${cc_bin_dir}"    
        GIT_REPOSITORY "https://github.com/arobenko/comms_champion.git"
        GIT_TAG "${cc_branch}"
        SOURCE_DIR "${cc_src_dir}"
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} 
            -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}  
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} 
            ${extra_cc_args}
        BINARY_DIR "${cc_bin_dir}"
    )
        
    include_directories("${INSTALL_DIR}/include")
    link_directories("${INSTALL_DIR}/lib")
    set (CC_COMMS_FOUND TRUE)
    set (CC_COMMS_CHAMPION_FOUND TRUE)
    set (CC_INCLUDE_DIRS ${INC_INSTALL_DIR})
    set (CC_PLUGIN_LIBRARY_DIRS ${LIB_INSTALL_DIR})
    set (CC_PLUGIN_LIBRARIES "comms_champion")
else ()
    list (APPEND CMAKE_PREFIX_PATH "${INSTALL_DIR}")

    if (NOT "${CC_MAIN_INSTALL_DIR}" STREQUAL "")
        list (APPEND CMAKE_PREFIX_PATH "${CC_MAIN_INSTALL_DIR}")
    endif ()

    find_package(CommsChampion NO_MODULE)
    if (CC_COMMS_CHAMPION_FOUND)
        file (RELATIVE_PATH rel_plugin_install_path "${CC_ROOT_DIR}" "${CC_PLUGIN_DIR}")
        set (PLUGIN_INSTALL_DIR "${INSTALL_DIR}/${rel_plugin_install_path}")
    endif ()
endif ()

if ((UNIX) AND (NOT CC_MQTT_NO_CCACHE))
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif(CCACHE_FOUND)
endif ()    

include_directories(
    BEFORE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/protocol/include
)

add_subdirectory(cc_plugin)

