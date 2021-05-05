CXX = g++ -std=c++14 -Wall -Werror -mwindows

LIBS = -LC:/SFML-MinGW/lib \
	   -lsfml-graphics \
	   -lsfml-window \
	   -lsfml-system

SOURCES = *.cpp

INCLUDES = -IC:/SFML-MinGW/include

all:
	$(CXX) $(SOURCES) $(INCLUDES) $(LIBS)