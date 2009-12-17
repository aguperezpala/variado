DEFINES += __DEBUG
INCLUDEPATH += .. ./timeline ./normalbox .
HEADERS += gtqobject.h \
	gtimequeue.h \
	../consts.h \
	../debug.h \
	timeline/timeline.h \
	normalbox/normalbox.h
SOURCES += gtqobject.cpp \
	gtimequeue.cpp \
	timeline/timeline.cpp \
	normalbox/normalbox.cpp \
	test.cpp
