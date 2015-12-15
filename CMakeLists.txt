CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

set ( APP_NAME Qt5OpenGLExamples) 
project(${APP_NAME})

find_package(OpenGL REQUIRED)

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/rpavlik-cmake-modules-fe2273")

include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP
 
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	MESSAGE ("-- Host system is 64 bits  CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P} --")
	if(MSVC)
		set (CMAKE_PREFIX_PATH 
			"c:\\Qt\\5.4\\msvc2013_64_opengl"
			#"c:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Lib\\"
			)
		set (QTDIR "c:/Qt/5.4/msvc2013_64_opengl")
	endif(MSVC)
	set ( MACHINEBIT	64)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	MESSAGE ("-- Host system is 32 bits  CMAKE_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P} --")
	if(MSVC)
		set (CMAKE_PREFIX_PATH 
			"c:\\Qt\\5.4\\msvc2013_opengl"
			#"c:\\Program Files\\Microsoft SDKs\\Windows\\v7.1\\Lib\\"
			)
		set (QTDIR "c:/Qt/5.4/msvc2013_opengl")
	endif(MSVC)
	set ( MACHINEBIT	32)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

if(MSVC)
# Instruct CMake where to find cmake related stuffs
SET( APP_BIN_DIR "${PROJECT_SOURCE_DIR}/../binvc-${MACHINEBIT}")
ELSE(MSVC)
SET( APP_BIN_DIR "${PROJECT_SOURCE_DIR}/../bin-${MACHINEBIT}")
endif(MSVC)


set(EXTERNAL_ROOT ${CMAKE_SOURCE_DIR}/external)

set (RUN_ENV "PATH=${QTDIR}/bin;%PATH%")

#set(PROJECT_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/../${APP_BIN_DIR})
MESSAGE ("-- APP_BIN_DIR: ${APP_BIN_DIR}")

#####################################################################################
#
#	Find common libraries 
#
#####################################################################################
# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5OpenGL REQUIRED)

 

#####################################################################################
#
#	Additional dependencies
#
#####################################################################################
if ( WIN32 )
	include_directories(
	  #${EXTERNAL_ROOT}/glfw3/include/win32
	  #${EXTERNAL_ROOT}/win32-specific/gles/include/OGLES
	  ${EXTERNAL_ROOT}/glfw-3.1.2/include/GLFW/
	  #${EXTERNAL_ROOT}/glm-0.9.7.1/
	  ${EXTERNAL_ROOT}/glew-1.13.0/include/
	)
endif()

set(EXTERNAL_LINK
	#glew32
	glfw
	GLEW_1130
)

add_subdirectory(circles-in-a-cube)
add_subdirectory(helloGL2)
add_subdirectory(testGraphicview)
add_subdirectory(testQGLWdget)
add_subdirectory(testQGLWdget_glew)
add_subdirectory(testQwindow)
add_subdirectory(testQwindow-glew)
