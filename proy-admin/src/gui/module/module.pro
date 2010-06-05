FORMS = ../note/note_ui.ui ../task/task_ui.ui ../function/function_ui.ui \
module_ui.ui

CONFIG += uitools
DEFINES += __DEBUG
HEADERS += ../../task/task.h ../../note/*.h ../../function/*.h \
../../module/*.h ../../parser/*.h  ../../project/*.h ../../filemanager/*.h \
../../searcher/*.h \
../task/task_gui.h ../note/note_gui.h ../function/function_gui.h \
module_gui.h
SOURCES += ../../task/task.cpp ../../parser/*.cpp ../../note/*.cpp ../../function/*.cpp \
../../module/*.cpp ../../project/*.cpp ../../filemanager/*.cpp \
../../searcher/*.cpp \
../task/task_gui.cpp ../note/note_gui.cpp ../function/function_gui.cpp \
module_gui.cpp test.cpp
INCLUDEPATH += ../../task ../../parser ../../note ../../function \
../../module ../../parser  ../../project ../../filemanager \
../../searcher \
../task ../note ../function \