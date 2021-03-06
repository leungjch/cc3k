CXX= g++
CXXFLAGS=-std=c++14 -g -MMD 
OBJECTS=main.o cc3k.o floor.o level.o defaultlevel.o stairway.o cell.o entity.o character.o \
		utils/message.o utils/color.o \
		player/player.o  player/drow.o player/goblin.o player/shade.o player/troll.o player/vampire.o \
		treasure/gold.o treasure/smallPile.o treasure/normalPile.o treasure/merchantHoard.o treasure/dragonHoard.o \
		item.o potion/potion.o potion/restoreHealth.o potion/poisonHealth.o potion/woundAtk.o potion/boostAtk.o potion/boostDef.o potion/woundDef.o \
		display/subject.o display/textObserver.o display/graphicalObserver.o display/window.o \
		enemy/enemy.o enemy/orc.o enemy/dragon.o enemy/merchant.o enemy/elf.o enemy/halfling.o enemy/dwarf.o enemy/human.o \
		enemy/pathfinder.o

DEPENDS=${OBJECTS:.o=.d}
EXEC=cc3k


${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11 -lXpm
-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
