project(RobotSwarmAPI)
cmake_minimum_required(VERSION 2.8)
FIND_PACKAGE(OpenSSL)
add_definitions(-std=c++11 -lwebsockets -lopenssl)

FIND_PATH ( LIBWEBSOCKETS_INCLUDE_DIR libwebsockets.h
    /usr/local/include
    /usr/include
)

FIND_LIBRARY ( LIBWEBSOCKETS_LIBRARIES websockets
    /usr/local/lib
    /usr/lib
)

GET_FILENAME_COMPONENT( LIBWEBSOCKETS_LIBRARY_DIR ${LIBWEBSOCKETS_LIBRARIES} PATH )

SET ( LIBWEBSOCKETS_FOUND "NO" )
IF ( LIBWEBSOCKETS_INCLUDE_DIR )
    IF ( LIBWEBSOCKETS_LIBRARIES )
        SET ( LIBWEBSOCKETS_FOUND "YES" )
    ENDIF ( LIBWEBSOCKETS_LIBRARIES )
ENDIF ( LIBWEBSOCKETS_INCLUDE_DIR )

MARK_AS_ADVANCED(
    LIBWEBSOCKETS_LIBRARY_DIR
    LIBWEBSOCKETS_INCLUDE_DIR
    LIBWEBSOCKETS_LIBRARIES
)

#set(LWS_LIB "/home/tgalecki/libwebsockets/build/lib/libwebsockets.a")
#include_directories("/home/tgalecki/libwebsockets/lib")
include_directories(.)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} robotstatus.h)
target_link_libraries(${PROJECT_NAME} ${LWS_LIB})
