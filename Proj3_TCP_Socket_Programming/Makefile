all: ringmaster player

ringmaster: ringmaster.cpp function.cpp potato.h
	g++ -g -o ringmaster ringmaster.cpp function.cpp 
player: player.cpp function.cpp potato.h
	g++ -g -o player player.cpp function.cpp 

.PHONY:
	clean
clean:
	rm -rf *.o ringmaster player
