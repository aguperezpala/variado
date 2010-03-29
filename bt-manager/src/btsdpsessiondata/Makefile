CC = g++
CFLAGS = -g -Wall -Wextra -D_DDEBUG
LIBS = -lbluetooth
TARGET = test
INCLUDES = -I../ 
SOURCES = $(shell echo *.cpp) 
OBJECTS = $(SOURCES:%.cpp=%.o)

all: $(TARGET) 


$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(INCLUDES)


-include .depend

.depend: $(SOURCES)
	$(CC) -MM $(SOURCES) > .depend



.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~