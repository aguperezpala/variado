DEFINES += __DEBUG
QMAKE_CXXFLAGS += -g
INCLUDEPATH += .. ./timeline ./normalbox ./timepointer ./trigger ./timemeters . \
		../timetransform
HEADERS += gtqobject.h \
	gtimequeue.h \
	../consts.h \
	../debug.h \
	../timetransform/timetransform.h \
	timeline/timeline.h \
	normalbox/normalbox.h \
	timepointer/timepointer.h \
	timemeters/timemeters.h \
	trigger/trigger.h
	
SOURCES += gtqobject.cpp \
	gtimequeue.cpp \
	../timetransform/timetransform.cpp \
	timeline/timeline.cpp \
	normalbox/normalbox.cpp \
	timepointer/timepointer.cpp \
	trigger/trigger.cpp \
	timemeters/timemeters.cpp \
	test.cpp
