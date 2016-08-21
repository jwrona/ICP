CONFIG += core gui
TEMPLATE += app

HEADERS += basis.h \
           player.h \
           gui.h \
           myConnect.h \
           saveGame.h \
           loadFile.h

SOURCES += basis.cpp \
           checkers.cpp \
           player.cpp \
           gui.cpp \
           myConnect.cpp \
           saveGame.cpp \
           loadFile.cpp
           
QT       += network
QT       += xml

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -pedantic -Wall -Wextra -Winit-self -Wcast-align -Wcast-qual -Wmissing-include-dirs -Wshadow
#QMAKE_CXXFLAGS += -Werror -Weffc++
