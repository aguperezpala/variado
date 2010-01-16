DEFINES += __DEBUG
QMAKE_CXXFLAGS += -g
INCLUDEPATH += .. ./timeline ./normalbox ./timepointer ./trigger .
HEADERS += gtqobject.h \
	gtimequeue.h \
	../consts.h \
	../debug.h \
	timeline/timeline.h \
	normalbox/normalbox.h \
	timepointer/timepointer.h \
	trigger/trigger.h
	
SOURCES += gtqobject.cpp \
	gtimequeue.cpp \
	timeline/timeline.cpp \
	normalbox/normalbox.cpp \
	timepointer/timepointer.cpp \
	trigger/trigger.cpp \
	test.cpp
