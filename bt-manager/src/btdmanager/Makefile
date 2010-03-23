CC = g++
CC = g++
CFLAGS = -g -Wall -Wextra -D__DEBUG
LIBS = -lbluetooth
TARGET = test
INCLUDES = -I./../ -I../btdongledevice
SOURCES = $(shell echo *.cpp) 
OBJECTS = $(SOURCES:%.c=%.o)

all: $(TARGET) 


$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) 


-include .depend

.depend: $(SOURCES)
	$(CC) -MM $(SOURCES) > .depend



.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJECTS) .depend *~