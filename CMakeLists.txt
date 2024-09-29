cmake_minimum_required(VERSION 3.20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

project(webrtc_camera)

message(STATUS "this is test")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(BUILD_SHARED_LIBS OFF)

#set(QT_BIN_DIR C:/Qt/Qt5.14.2/5.14.2/msvc2017_64/bin)
#set(Qt5_DIR C:/Qt/Qt5.14.2/5.14.2/msvc2017_64/lib/cmake/Qt5)

set(CMAKE_BUILD_TYPE Debug)

if (CMAKE_CL_64) 
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(BUILD_TYPE "x64-Debug")
    else()
        set(BUILD_TYPE "x64-Release")
    endif()
else()
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(BUILD_TYPE "x86-Debug")
    else()
        set(BUILD_TYPE "x86-Release")
    endif()
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

set(SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/out/build/${BUILD_TYPE})
message(STATUS ${CMAKE_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/bin)

set(SDL_PATH ${SDK_DIR}/3rd/SDL2-2.0.14)
set(YUV_PATH ${SDK_DIR}/3rd/libyuv)

#set(JPEG_DIR ${SDK_DIR}/3rd/libjpeg-turbo64/lib/cmake/libjpeg-turbo)

set(JPEG_SYS_PATH  ${SDK_DIR}/3rd/libjpeg-turbo64)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	add_definitions(-DWIN32)
    add_definitions(-D_WIN32)
    add_definitions(-DWEBRTC_WIN)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    add_definitions(-DHAVE_JPEG)
    add_definitions(-D_DEBUG)
    
endif()

set(JPEG_FOUND TRUE)
set(JPEG_INCLUDE_DIR ${JPEG_SYS_PATH}/include)
set(JPEG_LIBRARY ${JPEG_SYS_PATH}/lib/jpeg-static.lib)

#message("this is test")
#message(STATUS ${SDL_PATH})

include_directories(${SDK_DIR})
include_directories(${SDL_PATH}/include)
include_directories(${YUV_PATH}/include)
#include_directories(${JPEG_PATH}/include)
include_directories(${JPEG_SYS_PATH}/include)



link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
link_directories(${SDL_PATH}/lib/x64)
link_directories(${JPEG_SYS_PATH}/lib)

set(OTHER_LIBS 
yuv.lib
SDL2.lib
jpeg.lib
turbojpeg.lib
)


file(GLOB SRCS 
./*.cpp
./*.cc
./*.h
common/*.h
common/*.hpp
common/*.cc
core/*.h
core/*.cc
video_frame/*.h
video_frame/*.cc
windows/*.h
windows/*.cc
test/*.h
test/*.cpp
)

#find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME}
${OTHER_LIBS}
)

add_subdirectory(./3rd/libyuv)
#add_subdirectory(./3rd/libjpeg-turbo-2.1.3)

#if (EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations/qt_zh_CN.qm)
#    message("file exists")
#else ()
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
#        COMMAND ${QT_BIN_DIR}/windeployqt ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#endif()