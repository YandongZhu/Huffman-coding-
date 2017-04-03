CXX = g++-5
CXXFLAGS = -std=c++14 -Wall -Wextra -Wno-unused-parameter -MMD -g
OBJECTS = HEncode.o HDecode.o
DEPENDS = ${OBJECTS:.o=.d}

all: HEncode HDecode

HEncode: HEncode.o
	${CXX} ${CXXFLAGS} HEncode.o -o HEncode

HDecode: HDecode.o
	${CXX} ${CXXFLAGS} HDecode.o -o HDecode

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} HDecode HEncode

