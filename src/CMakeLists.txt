aux_source_directory(. lib_SRC_LIST)
include_directories(${RobotSwarmAPI_SOURCE_DIR}/include)
find_package(Qt5Core)
find_package(Qt5WebSockets)

add_library(${PROJECT_NAME} ${lib_SRC_LIST})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::WebSockets)
