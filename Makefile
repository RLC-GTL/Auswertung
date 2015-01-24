# Makefile for RLC Auswertung mit Gewichtsstrafen

NAME = rlc-gtl-auswertung
SRCS = CcsvList.cpp CrgaParameter.cpp CrgaCar.cpp CrgaPoint.cpp CrgaDriver.cpp CrgaRace.cpp CrgaHTML.cpp Crga.cpp rga.cpp

OBJS = $(SRCS:%.cpp=%.o)

CC = g++ -c
CFLAGS = -Wall

CLNK = g++
LFLAGS = -Wall

# DOCTOOL = doxygen Doxyfile

$(NAME): $(OBJS)
	$(CLNK) $(LFLAGS) -o $@ $+
	$(DOCTOOL)

%.o: %.cpp
	$(CC) $(CFLAGS)  $<

clean:
	rm -f *~ *.o
	rm -f -R html latex
	rm -f deps.mk

depend:
	g++ -MM $(SRCS) > deps.mk

-include deps.mk

