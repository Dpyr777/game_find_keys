CXX = g++
CXXFLAGS = -c

all: sfml-app

sfml-app: main.o player.o enemy.o
	$(CXX) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio  

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $<

player.o: player.cpp
	$(CXX) $(CXXFLAGS) $<

enemy.o: enemy.cpp
	$(CXX) $(CXXFLAGS) $<

run: sfml-app
	./$<

