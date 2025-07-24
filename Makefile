CXX=g++
CXXFLAGS=-std=c++17 -Wall -g -MMD
EXEC=RAIInet
CCFiles=$(wildcard src/*.cc)
OBJECTS=${CCFiles:.cc=.o}
DEPENDS=${CCFiles:.cc=.d}
LIBS:=-lboost_program_options

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${LIBS}

-include ${DEPENDS}

.PHONY: clean debug
clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

debug:
	@echo ${CCFiles}
