

Sources = src/disc.cpp src/display2.cpp src/event.cpp src/holes.cpp src/main.cpp src/physics.cpp src/walls.cpp src/client.cpp src/server.cpp src/imageloader.cpp src/player.cpp
Include = include/header.h include/physics.h include/networking.h include/imageloader.h 
OBJECTS = disc.o display2.o event.o holes.o main.o physics.o walls.o client.o server.o imageloader.o player.o

%.o:src/%.cpp
	@g++ -g -I include/ -c $< -lglut -lGLU -lm
	
carrom : $(OBJECTS)
	@g++ -g -I include/ -o $@ $(OBJECTS) -lglut -lGLU -lm
	@./carrom
	 
clean:
	rm *.o
