# Makefile for Advent of Code 2025: Day 11

# Default target
all: AoC11_P1 AoC11_P2

# Build Part 1
AoC11_P1: src/AoC11_P1.cpp include/Graph.h include/HashMap.h
	mkdir -p programs
	g++ -o programs/AoC11_P1 src/AoC11_P1.cpp

# Build Part 2
AoC11_P2: src/AoC11_P2.cpp include/Graph.h include/HashMap.h
	mkdir -p programs
	g++ -o programs/AoC11_P2 src/AoC11_P2.cpp

# Clean build files
clean:
	rm -rf programs/AoC11_P1 programs/AoC11_P2
.PHONY: all clean