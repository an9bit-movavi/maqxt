cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)

project(maqxt CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt4 REQUIRED qtcore qtgui)
include(${QT_USE_FILE})

set(ext_libs)
if(APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6")
	set(CMAKE_OSX_SYSROOT "macosx10.6")
	find_library(CARBON_FRAMEWORK Carbon)
	list(APPEND ext_libs ${CARBON_FRAMEWORK})
elseif(WIN32)
elseif(UNIX)
endif()

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

include_directories(.)
add_library(${PROJECT_NAME} SHARED ${sources} ${headers})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${ext_libs})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY maqxt DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*_p.h" EXCLUDE)
