#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <vector>
#define MAXNODES 1001
#define MAXCHARSPERLINE 1000
#define IN "date.in"

using namespace std;

struct graph {
	int numberOfVerticies;
	int numberOfEdges;
	vector<int> adjacencyList[MAXNODES];
	int coloring[MAXNODES];
};

graph readGraph();

void bktColoring(int k, graph g, int maxColor, int *coloring);

int evaluateColoring(graph g);

void greedyColoring(graph g, int *coloring);

void GAColoring(graph g, int *coloring, int popSize, double probabilityOfMutation, double probabilityOfCrossover, int numberOfGenerations);

void setColoring(graph &g, int *coloring);

double coloringFitness(graph g);

int maxColor(int *coloring, int numberOfVerticies);