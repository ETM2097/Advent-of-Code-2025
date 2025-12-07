all: First_DP Second_DP First_Graph Second_Graph

First_DP: src/DynamicProgramming/AoC7_P1.cpp
mkdir -p programs
g++ -o programs/First_DP src/DynamicProgramming/AoC7_P1.cpp

Second_DP: src/DynamicProgramming/AoC7_P2.cpp
mkdir -p programs
g++ -o programs/Second_DP src/DynamicProgramming/AoC7_P2.cpp

First_Graph: src/Graphs/AoC7_P1_G.cpp src/Graphs/Graph.cpp
mkdir -p programs
g++ -o programs/First_Graph src/Graphs/AoC7_P1_G.cpp src/Graphs/Graph.cpp

Second_Graph: src/Graphs/AoC7_P2_G.cpp src/Graphs/Graph.cpp
mkdir -p programs
g++ -o programs/Second_Graph src/Graphs/AoC7_P2_G.cpp src/Graphs/Graph.cpp

clean:
	rm -rf programs