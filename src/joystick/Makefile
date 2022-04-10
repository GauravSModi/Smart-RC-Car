TARGET = js
OUTFILE = js
OUTDIR = $(HOME)/cmpt433/public/project
MODULES = network_js.cpp

CROSS_COMPILE = arm-linux-gnueabihf-
CC_CPP = $(CROSS_COMPILE)g++
CFLAGS = -Wall -g -pthread -Werror

all: app

app:
	$(CC_CPP) $(CFLAGS) $(MODULES) $(TARGET).cpp -o $(OUTFILE)
	cp $(TARGET) $(OUTDIR)
	
clean:
	rm $(TARGET)
