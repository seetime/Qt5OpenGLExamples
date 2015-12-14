CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

set ( APP_NAME testGraphicView) 
project(${APP_NAME})

# Below is required to set debugging enrinment under Visual studio 2010
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(CreateLaunchers)
 
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

SET(${APP_NAME}_HEADERS graphicview.h scene.h geometryengine.h)
SET(${APP_NAME}_SOURCES main.cpp 
						graphicview.cpp scene.cpp geometryengine.cpp)


if(MSVC)
# Instruct CMake where to find cmake related stuffs
#set (CMAKE_PREFIX_PATH "C:\\Qt\\5.4\\msvc2013_opengl\\")
set (CMAKE_PREFIX_PATH 
	"c:\\Qt\\5.4\\msvc2013_opengl"
	"c:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Lib\\"
	)
 
# Set QTDIR to Qt5 SDK path
#set (QTDIR "C:/Qt/Qt5.0.2/5.0.2/msvc2010_opengl")
set (QTDIR "c:/Qt/5.4/msvc2013_opengl")
set (RUN_ENV "PATH=${QTDIR}/bin")
MESSAGE ("-- RUN_ENV: ${RUN_ENV}")
endif(MSVC)
 
# Find the QtWidgets library
find_package(OpenGL REQUIRED)

find_package(Qt5Widgets)
find_package(Qt5OpenGL REQUIRED)

 
SET(QT5_RCCS textures.qrc )
SET(QT5_UIS )

QT5_ADD_RESOURCES ( ${APP_NAME}_RCCS ${QT5_RCCS} )
QT5_WRAP_UI(UIS_HDRS
    ${QT5_UIS}
)

SET( SHADER_SOURCES 
  shaders/vshader.glsl
  shaders/fshader.glsl
)
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/shaders/.*" )

# Tell CMake to create the sample1 executable
add_executable(${APP_NAME} 
	MACOSX_BUNDLE
	WIN32
	${${APP_NAME}_HEADERS} ${${APP_NAME}_SOURCES} ${${APP_NAME}_RCCS} ${UIS_HDRS} 
	${SHADER_SOURCES})
 

TARGET_LINK_LIBRARIES ( ${APP_NAME}
    ${OPENGL_LIBRARIES}
    Qt5::Widgets
	Qt5::OpenGL
)

# Create the launcher for VS2010
if (MSVC_IDE)
# Set ENVIRONMENT. This will make VS debugger to refer qt5 related DLLs properly
create_target_launcher(${APP_NAME} ENVIRONMENT  "PATH=${QTDIR}/bin")
#create_target_launcher(sample1 ENVIRONMENT  ${RUN_ENV})
endif(MSVC_IDE)


if(WIN32 AND MSVC)
  add_custom_command(TARGET ${APP_NAME} PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                         ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR})

  #add_custom_command(TARGET ${APP_NAME} POST_BUILD
  #                   COMMAND ${CMAKE_COMMAND} -E copy_if_different 
  #                   ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl 
  #					 ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32 AND MSVC)


add_subdirectory(helloGL2)