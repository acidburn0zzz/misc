cmake_minimum_required(VERSION 2.8)

PROJECT(ctse)
FIND_PACKAGE(Qt4 REQUIRED)

SET(ctse_SOURCES character.cpp game.cpp gui.cpp main.cpp sramfile.cpp)
#SET(ctse_HEADERS character.h game.h gui.h sramfile.h strings.h struct.h)
SET(ctse_HEADERS gui.h)

QT4_WRAP_CPP(ctse_HEADERS_MOC ${ctse_HEADERS})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(ctse ${ctse_SOURCES} ${ctse_HEADERS_MOC})
TARGET_LINK_LIBRARIES(ctse ${QT_LIBRARIES})
