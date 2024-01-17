cmake_minimum_required(VERSION 3.5)
project(iot CXX)

include(CheckIncludeFileCXX)

# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(controllers CTL_SRC)
aux_source_directory(plugins PLUGIN_SRC)

file(GLOB_RECURSE FILTER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/filters/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/filters/*.cpp
)

file(GLOB_RECURSE CONTROLLER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers/*.cpp
)

file(GLOB_RECURSE MODEL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/models/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/models/*.cpp
)

file(GLOB_RECURSE INCLUDE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
)

drogon_create_views(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/views
    ${CMAKE_CURRENT_BINARY_DIR}
)

# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${SRC_DIR}
    ${CTL_SRC}
    ${FILTER_SRC}
    ${CONTROLLER_SRC}
    ${PLUGIN_SRC}
    ${MODEL_SRC}
    ${INCLUDE_SRC}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
    pq
)
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)

install (TARGETS ${PROJECT_NAME}
    DESTINATION /var/www
)

install (FILES ${CMAKE_SOURCE_DIR}/config.json
    DESTINATION /var/www
)

install (DIRECTORY ${CMAKE_SOURCE_DIR}/public
    DESTINATION /var/www
)

install (DIRECTORY ${CMAKE_SOURCE_DIR}/private
    DESTINATION /var/www
)

install (FILES ${CMAKE_SOURCE_DIR}/install/iot.service
    DESTINATION /etc/systemd/system
)

install(
    CODE "execute_process(COMMAND sudo systemctl daemon-reload && sudo systemctl enable iot.service && sudo systemctl restart iot.service)"
)
