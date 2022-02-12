TARGET = src/main
OUTFILE = main
OUTDIR = $(HOME)/cmpt433/public/project/
MODULES = src/joystick/joystick.h src/led/led.h src/rover/rover.h src/common/utils.h

# compiler setup
CROSS_COMPILE = arm-linux-gnueabihf-
CC_CXX = $(CROSS_COMPILE)g++

# compiler flags
CXXFLAGS = -g -Wall -std=c++11 -pthread -O3 -Werror


all: app

app:
	$(CC_CXX) $(CXXFLAGS) $(MODULES) $(TARGET).cpp -o $(OUTFILE)
	cp $(OUTFILE) $(OUTDIR)

clean :
	rm -f $(OUTFILE)
	rm -f $(OUTDIR)/$(OUTFILE)