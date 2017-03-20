# search for source files in current dir
aux_source_directory(. lib_SRC_LIST)

# include include dir
include_directories(${RobotSwarmAPI_SOURCE_DIR}/include)

# list required packages
find_package(Qt5Core REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(Qt5Network REQUIRED)

# build API's library
add_library(${PROJECT_NAME} ${lib_SRC_LIST} ${INCLUDE_ALL})
add_definitions(-std=c++11)

# link qt5 modules to API's library
qt5_use_modules(${PROJECT_NAME} Core WebSockets Network)
