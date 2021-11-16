CXX=g++
CXXFLAGS=-std=c++14 -g -MMD 
OBJECTS=main.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=cc3k

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}
-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
