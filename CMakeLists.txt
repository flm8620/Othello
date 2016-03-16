cmake_minimum_required(VERSION 3.3)
project(Othello)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Core_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Core_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")







set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -pedantic -std=c++11")
add_definitions(-DsrcPath="${CMAKE_CURRENT_SOURCE_DIR}")


set(SOURCE_FILES main.cpp GameState.cpp GameState.h OthelloAI.cpp OthelloAI.h GameJudge.cpp GameJudge.h genetichost.cpp tools.cpp)
set(QT_RESOURCES_FILE icons.qrc)
qt5_add_resources(RESOURCE_ADDED ${QT_RESOURCES_FILE})
add_executable(Othello ${SOURCE_FILES})
 add_executable(OthelloGui main_gui.cpp GameState.cpp GameState.h OthelloAI.cpp OthelloAI.h GameJudge.cpp GameJudge.h genetichost.cpp tools.cpp
gamewindow.cpp gamewindow.h piecebutton.cpp piecebutton.h gameworker.h gameworker.cpp ${RESOURCE_ADDED})
include_directories("${PROJECT_SOURCE_DIR}")

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(Othello ${Qt5Widgets_LIBRARIES})
target_link_libraries(Othello ${Qt5Core_LIBRARIES})

target_link_libraries(OthelloGui ${Qt5Widgets_LIBRARIES})
target_link_libraries(OthelloGui ${Qt5Core_LIBRARIES})


