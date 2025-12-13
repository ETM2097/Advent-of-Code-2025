all: AoC8_P1 AoC8_P2

AoC8_P1: src/AoC8_P1.cpp src/Graph.cpp src/UnionFind.cpp
	mkdir -p programs
	g++ -o programs/AoC8_P1 src/AoC8_P1.cpp src/Graph.cpp src/UnionFind.cpp 

AoC8_P2: src/AoC8_P2.cpp src/Graph.cpp src/UnionFind.cpp
	mkdir -p programs
	g++ -o programs/AoC8_P2 src/AoC8_P2.cpp src/Graph.cpp src/UnionFind.cpp 

clean:
	rm -rf programs