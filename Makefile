CXX=g++
CXXFLAGS=-std=c++23 -Wall -g -MMD
DEPFLAGS:=-MMD -MP -Iinc
EXEC=RAIInet

SRC_DIR:=src
OBJ_DIR:=build

CCFiles=$(wildcard ${SRC_DIR}/*.cc)
OBJECTS=${CCFiles:${SRC_DIR}/%.cc=${OBJ_DIR}/%.o}
DEPENDS=${OBJECTS:.o=.d}
LIBS:=-lboost_program_options

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o $@ ${LIBS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cc
	@mkdir -p ${OBJ_DIR}
	${CXX} ${CXXFLAGS} ${DEPFLAGS} -c $< -o $@
-include ${DEPENDS}

.PHONY: clean debug
clean:
	rm ${EXEC} ${OBJECTS} ${DEPENDS}

debug:
	@echo ${CCFiles}
