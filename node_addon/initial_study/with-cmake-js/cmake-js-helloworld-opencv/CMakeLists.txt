cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0042 NEW)
set (CMAKE_CXX_STANDARD 11)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../opencv/build")
find_package(OpenCV REQUIRED) 

project (addon)
include_directories(${CMAKE_JS_INC} ${OPENCV_EXAMPLE_LIB})
file(GLOB SOURCE_FILES "cpphello.cpp")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${OpenCV_LIBS})

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# define NAPI_VERSION
add_definitions(-DNAPI_VERSION=3)