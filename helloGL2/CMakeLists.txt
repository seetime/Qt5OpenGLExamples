CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

set ( APP_NAME helloGL2) 
project(${APP_NAME})

SET(${APP_NAME}_HEADERS 
            mainwindow.h window.h glwidget.h logo.h)
SET(${APP_NAME}_SOURCES main.cpp 
						mainwindow.cpp window.cpp glwidget.cpp logo.cpp)


# Find the QtWidgets library
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5OpenGL REQUIRED)

include_directories( ../common/ )
 
SET(QT5_RCCS )
SET(QT5_UIS )

QT5_ADD_RESOURCES ( ${APP_NAME}_RCCS ${QT5_RCCS} )
QT5_WRAP_UI(UIS_HDRS
    ${QT5_UIS}
)

SET( SHADER_SOURCES 
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
  #add_custom_command(TARGET ${APP_NAME} PRE_BUILD
  #                   COMMAND ${CMAKE_COMMAND} -E copy_directory
  #                       ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR})

  #add_custom_command(TARGET ${APP_NAME} POST_BUILD
  #                   COMMAND ${CMAKE_COMMAND} -E copy_if_different 
  #                   ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.glsl 
  #					 ${CMAKE_CURRENT_BINARY_DIR})
endif(WIN32 AND MSVC)