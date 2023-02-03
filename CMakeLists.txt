# 声明编译要求cmake最低版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.18.5)

#设定编译器
SET( CMAKE_CXX_COMPILER "g++")

#ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-fPIC)




# 声明一个cmake工程
PROJECT(CSGOM)

set(DobbyHome Dobby)
include_directories(
        ${DobbyHome}/include
)
OPTION(DOBBY_DEBUG OFF)
OPTION(DOBBY_GENERATE_SHARED OFF)

add_subdirectory(${DobbyHome} dobby)


#设定编译后生成的库文件所在的目录
SET( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#并且把该目录设为链接目录
LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/lib)

#设定头文件所在的目录
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include)




#生成必要的构建指令，将指定的源码编译到库中。
add_library(CSGOM SHARED
	
	src/native-start.cpp
	src/hook.cpp
	src/data.cpp
	src/UI.cpp
	src/android_native_app_glue.c
	)

#链接动态库
TARGET_LINK_LIBRARIES(CSGOM log android dobby)



# 指定库编译输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

 
