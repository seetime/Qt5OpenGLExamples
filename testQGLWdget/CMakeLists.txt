CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

set ( APP_NAME testQGLWidget) 
project(${APP_NAME})

include(CocosBuildHelpers)


SET(${APP_NAME}_HEADERS 
            			window.h myglwidget.h glextensions.h)
SET(${APP_NAME}_SOURCES main.cpp 
						window.cpp myglwidget.cpp glextensions.cpp)


# Find the QtWidgets library
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5OpenGL REQUIRED)

include_directories( ../common/ )
 
SET(QT5_RCCS )
SET(QT5_UIS window.ui)

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

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

if ( WIN32 )
  #also copying dlls to binary directory for the executable to run
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_ROOT}/win32-specific/gles/prebuilt/glew32.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	#COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_ROOT}/win32-specific/zlib/prebuilt/zlib1.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	)
else()
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    )

endif()

# Create the launcher for VS2010
if (MSVC_IDE)
# Set ENVIRONMENT. This will make VS debugger to refer qt5 related DLLs properly
create_target_launcher(${APP_NAME} WORKING_DIRECTORY "${APP_BIN_DIR}" ENVIRONMENT  "PATH=${QTDIR}/bin\;%PATH%")
#create_target_launcher(sample1 ENVIRONMENT  ${RUN_ENV})
endif(MSVC_IDE)
set_target_properties(${APP_NAME} PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${APP_BIN_DIR}")


if(WIN32 AND MSVC)
  #add_custom_command(TARGET ${APP_NAME} PRE_BUILD
  #                   COMMAND ${CMAKE_COMMAND} -E copy_directory
  #                       ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR})

  #add_custom_command(TARGET ${APP_NAME} POST_BUILD
  #                   COMMAND ${CMAKE_COMMAND} -E copy_if_different 
  #                   "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX}" 
#					 "${APP_BIN_DIR}")
endif(WIN32 AND MSVC)