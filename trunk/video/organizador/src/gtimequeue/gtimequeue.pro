DEFINES += __DEBUG
QMAKE_CXXFLAGS += -g
INCLUDEPATH += .. ./timeline ./normalbox ./timepointer .
HEADERS += gtqobject.h \
	gtimequeue.h \
	../consts.h \
	../debug.h \
	timeline/timeline.h \
	normalbox/normalbox.h \
	timepointer/timepointer.h
	
SOURCES += gtqobject.cpp \
	gtimequeue.cpp \
	timeline/timeline.cpp \
	normalbox/normalbox.cpp \
	timepointer/timepointer.cpp \
	test.cpp
