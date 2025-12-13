all: First_DP Second_DP First_Graph Second_Graph

First_DP: src/DynamicProgramming/AoC7_P1.cpp
	g++ -o First_DP src/DynamicProgramming/AoC7_P1.cpp

Second_DP: src/DynamicProgramming/AoC7_P2.cpp
	g++ -o Second_DP src/DynamicProgramming/AoC7_P2.cpp

First_Graph: src/Graphs/AoC_P1_G.cpp src/Graphs/Graph.cpp
	g++ -o First_Graph src/Graphs/AoC_P1_G.cpp src/Graphs/Graph.cpp

Second_Graph: src/Graphs/AoC_P2_G.cpp src/Graphs/Graph.cpp
	g++ -o Second_Graph src/Graphs/AoC_P2_G.cpp src/Graphs/Graph.cpp

clean:
	rm -rf First_DP Second_DP First_Graph Second_Graph