TARGET = js
OUTFILE = js
OUTDIR = $(HOME)/cmpt433/public/project
MODULES = network_js.cpp

CROSS_COMPILE = arm-linux-gnueabihf-
CC_CPP = g++
CFLAGS = -Wall -g -pthread -Werror

all: app

app:
	$(CROSS_COMPILE)$(CC_CPP) $(CFLAGS) $(MODULES) $(TARGET).cpp -o $(OUTFILE)
	cp $(TARGET) $(OUTDIR)

host:
	$(CC_CPP) $(CFLAGS) $(MODULES) $(TARGET).cpp -o $(OUTFILE)
	cp $(TARGET) $(OUTDIR)
	
clean:
	rm $(TARGET)
