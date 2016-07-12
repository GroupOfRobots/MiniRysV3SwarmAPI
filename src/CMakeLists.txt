aux_source_directory(. lib_SRC_LIST)
include_directories(${RobotSwarmAPI_SOURCE_DIR}/include)
add_library(${PROJECT_NAME} ${lib_SRC_LIST})
