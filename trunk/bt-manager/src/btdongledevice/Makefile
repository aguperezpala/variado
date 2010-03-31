#g++ -I../ -I./../btsdpsessiondata -lbluetooth -g -Wall -Wextra -c *.cpp ../btsdpsessiondata/*.cpp 

CC = g++
CFLAGS = -g -Wall -Wextra -D_DDEBUG -I../ -I./../btsdpsessiondata
LIBS = -lbluetooth 
TARGET = test
SOURCES = $(shell echo *.cpp) 
OBJECTS = $(SOURCES:%.cpp=%.o) ../btsdpsessiondata/btsdpsessiondata.o

all: $(TARGET) 

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -g -o $@ $^ $(LIBS)


-include .depend

.depend: $(SOURCES)
	$(CC) -MM -g $(SOURCES) > .depend



.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~