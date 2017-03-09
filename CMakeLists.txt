project(RobotSwarmAPI)
cmake_minimum_required(VERSION 2.8)

# enabling automocing
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# linking sources and includes
aux_source_directory(. SRC_LIST)
add_subdirectory(${RobotSwarmAPI_SOURCE_DIR}/src)
FILE(GLOB_RECURSE INCLUDE_ALL "include/*.h")
include_directories(${RobotSwarmAPI_SOURCE_DIR}/include)
link_directories(${RobotSwarmAPI_SOURCE_DIR}/src)
add_definitions(-std=c++11)

# building and linking with API library
add_executable(${PROJECT_NAME}_run ${SRC_LIST} ${INCLUDE_ALL})
target_link_libraries(${PROJECT_NAME}_run ${PROJECT_NAME})

# provides .moc integrity (fix vtable errors)
qt5_use_modules(${PROJECT_NAME}_run Core WebSockets Network DBus)
qt5_wrap_cpp(${SRC_LIST} ${INCLUDE_ALL})
