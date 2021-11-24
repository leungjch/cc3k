CXX=g++
CXXFLAGS=-std=c++14 -g -MMD 
OBJECTS=main.o cc3k.o floor.o utils/color.o cell.o entity.o character.o \
		player/player.o  \
		player/drow.o stairway.o item.o potion/potion.o \
		potion/restoreHealth.o potion/poisonHealth.o potion/woundAtk.o potion/boostAtk.o potion/boostDef.o potion/woundDef.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=cc3k

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}
-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
