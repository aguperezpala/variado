FORMS = function_ui.ui
CONFIG += uitools
DEFINES += __DEBUG
HEADERS += ../../task/task.h ../../note/*.h ../../function/*.h \
../../module/*.h ../../parser/*.h  ../../project/*.h ../../filemanager/*.h \
../../searcher/*.h \
function_gui.h
SOURCES += ../../task/task.cpp ../../parser/*.cpp ../../note/*.cpp ../../function/*.cpp \
../../module/*.cpp ../../project/*.cpp ../../filemanager/*.cpp \
../../searcher/*.cpp \
function_gui.cpp test.cpp
INCLUDEPATH += ../../task ../../parser ../../note ../../function \
../../module ../../parser  ../../project ../../filemanager \
../../searcher \