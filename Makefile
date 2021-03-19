CXX = g++ -std=c++14 -Wall -Werror

SOURCE_FILES = *.cpp

all:
	$(CXX) $(SOURCE_FILES)