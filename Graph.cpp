#include "Graph.h"

graph readGraph() {
	FILE *in = fopen(IN, "r");
	graph g;
	
	char *str = (char *)malloc(MAXCHARSPERLINE * sizeof(char));
	fgets(str, MAXCHARSPERLINE, in);
	while (str[0] == 'c') {
		fgets(str, MAXCHARSPERLINE, in);
	}
	char c;
	char s[10];

	sscanf(str, "%c %s %d %d", &c, s, &g.numberOfVerticies, &g.numberOfEdges);
	
	int node1, node2;
	for (int i = 0; i < g.numberOfEdges; i++) {
		fgets(str, MAXCHARSPERLINE, in);
		sscanf(str, "%c %d %d", &c, &node1, &node2);
		g.adjacencyList[node1].push_back(node2);
		g.adjacencyList[node2].push_back(node1);
	}
	free(str);
	
	for (int i = 0; i <= g.numberOfVerticies; i++) {
		g.coloring[i] = -1;
	}

	return g;
}

int evaluateColoring(graph g) {
	int k = 0;
	for (int i = 1; i <= g.numberOfVerticies; i++) {
		for (int j = 0; j < g.adjacencyList[i].size(); j++) {
			if (g.coloring[i] == g.coloring[g.adjacencyList[i][j]]) {
				k++;
			}
		}
	}
	return k;
}

void bktColoring(int k,graph g, int maxColor, int *coloring) {
	static bool found = 0;
	if (found == 1) {
		return;
	}
	if (k == g.numberOfVerticies + 1) {
		if (evaluateColoring(g) == 0) {
			for (int i = 1; i <= g.numberOfVerticies; i++) {
				coloring[i] = g.coloring[i];

			}
			found = 1;
		}
	
	}
	else {
		for (int i = 0; i < maxColor && found == 0; i++) {
			g.coloring[k] = i;
			bktColoring(k + 1, g, maxColor, coloring);
		}
	}
}

void greedyColoring(graph g, int *coloring) {
	bool availableColors[MAXNODES];
	g.coloring[1] = 0;
	availableColors[0] = false;
	availableColors[1] = true;
	for (int i = 2; i <= g.numberOfVerticies; i++) {
		g.coloring[i] = -1;
		availableColors[i] = true;
	}
	int color;
	for (int i = 2; i <= g.numberOfVerticies; i++) {
		for (int j = 0; j < g.adjacencyList[i].size(); j++) {
			if (g.coloring[g.adjacencyList[i][j]] != -1) {
				availableColors[g.coloring[g.adjacencyList[i][j]]] = false;
			}
		}
			
		for (int j = 0; j < g.numberOfVerticies; j++) {
			if (availableColors[j] == true) {
				color = j;
				break;
			}
		}

		g.coloring[i] = color;

		for (int j = 0; j <= g.numberOfVerticies; j++) {
			availableColors[j] = true;
		}

	}

	for (int i = 1; i <= g.numberOfVerticies; i++) {
		coloring[i] = g.coloring[i];
	}
}

int maxColor(int *coloring, int numberOfVerticies) {
	int max1 = -(1<<30);
	for (int i = 1; i <= numberOfVerticies; i++) {
		if (max1 < coloring[i]) {
			max1 = coloring[i];
		}
	}

	return max1;
}

void colorRandom(int *coloring, int numberOfVerticies) {
	int randomColor = 0;
	for (int i = 1; i <= numberOfVerticies; i++) {
		coloring[i] = 0;
	}
	
	for (int i = 1; i <= numberOfVerticies; i++) {
		randomColor = rand() % (maxColor(coloring, numberOfVerticies) + 2);
		coloring[i] = randomColor;
	}
}

int **generateRandomPopulation(int popSize, int numberOfVerticies) {
	int **population = (int **)malloc(popSize * sizeof(int *));
	for (int i = 0; i < popSize; i++) {
		population[i] = (int *)malloc((numberOfVerticies + 1) * sizeof(int));
		colorRandom(population[i], numberOfVerticies);
	}

	return population;
}

double coloringFitness(graph g) {
	int k = evaluateColoring(g);
	int maxC = maxColor(g.coloring, g.numberOfVerticies);
	if (k == 0) {
		return (double)1 + (double)((double)1 / (double)maxC);
	}
	int d = g.numberOfEdges;
	if (k == 0) {
		d = 0;
	}
	

	return (double)1 / (double)(k + maxC );
}

void setColoring(graph &g, int *coloring) {
	
	for (int i = 1; i <= g.numberOfVerticies; i++) {
		g.coloring[i] = coloring[i];
	}
	
}

double *evaluatePopulation(int **population, graph g, int popSize) {
	double *populationFitness = (double *)malloc(popSize * sizeof(double));
	for (int i = 0; i < popSize; i++) {
		
		setColoring(g, population[i]);		
		populationFitness[i] = coloringFitness(g);
	}

	return populationFitness;
}

int **selectNewPopulation(int **oldPopulation, double *populationFitness, int popSize, int numberOfVerticies) {
	double fitnessSum = 0;
	for (int i = 0; i < popSize; i++) {
		fitnessSum += populationFitness[i];
	}
	double *individualProbability = (double *)malloc(popSize * sizeof(double));
	for (int i = 0; i < popSize; i++) {
		individualProbability[i] = fitnessSum / populationFitness[i];
	}
	
	double *cumulatedProbability = (double *)malloc(popSize * sizeof(double));
	cumulatedProbability[0] = 0;
	for (int i = 0; i < popSize; i++) {
		cumulatedProbability[i + 1] = cumulatedProbability[i] + individualProbability[i];
	}
	
	free(individualProbability);

	int **newPopulation = (int **)malloc(sizeof(int *)* popSize);
	for (int i = 0; i < popSize; i++) {
		newPopulation[i] = (int *)malloc(sizeof(int)* (numberOfVerticies + 1));
	}
	double randomProbability;
	for (int i = 0; i < popSize; i++) {
		randomProbability = (double)rand() / (double)RAND_MAX;
		for (int j = 0; j < popSize; j++) {
			if (randomProbability > cumulatedProbability[j] && randomProbability <= cumulatedProbability[j + 1]) {
				for (int c = 1; c <= numberOfVerticies; c++) {
					newPopulation[i][c] = oldPopulation[j][c];
				}
				break;
			}
		}
	}
	
	//free(cumulatedProbability);
	free(oldPopulation);
	return newPopulation;
}

void applyMutation(graph g,int *chromosome, double probabilityOfMutation, int numberOfVerticies) {
	if (chromosome[1] < 0) {
		colorRandom(chromosome, numberOfVerticies);
	}
	double randomProbability;
	int newColor;
	int maxC = maxColor(chromosome, numberOfVerticies);
	bool availableColors[MAXNODES];
	for (int i = 0; i < numberOfVerticies; i++) {
		availableColors[i] = true;
	}
	for (int i = 0; i < numberOfVerticies; i++) {
		randomProbability = (double)rand() / RAND_MAX;
		if (randomProbability < probabilityOfMutation) {
			for (int j = 0; j < g.adjacencyList[i].size(); j++) {
				availableColors[chromosome[g.adjacencyList[i][j]]] = false;
			}
			int ok = 0;
			for (int j = 0; j < maxC; j++) {
				if (availableColors[j] == true) {
					ok = 1;
				}
			}
			if (ok == 1) {
				newColor = rand() % (maxC + 1);
			}
			else {
				newColor = rand() % (maxC + 2);
			}
			while (availableColors[newColor] == false) {
				if (ok == 1) {
					newColor = rand() % (maxC + 1);
				}
				else {
					newColor = rand() % (maxC + 2);
				}
			}
			chromosome[i] = newColor;
			for (int j = 0; j < numberOfVerticies; j++) {
				availableColors[j] = true;
			}
			maxC = maxColor(chromosome, numberOfVerticies);
		}
	}
}

void applyCrossover(int *chromosome1, int *chromosome2, int numberOfVertcies, int randomPosition) {
	int aux;
	for (int i = randomPosition; i <= numberOfVertcies; i++) {
		aux = chromosome1[i];
		chromosome1[i] = chromosome2[i];
		chromosome2[i] = aux;
	}
}

void applyGAOperators(graph g, int **population, int popSize, int numberOfVerticies, double probabilityOfMutation, double crossoverProbability) {
	for (int i = 0; i < popSize; i++) {
		applyMutation(g, population[i], probabilityOfMutation, numberOfVerticies);
	}

	bool *selectedForCrossover = (bool *)malloc(popSize * sizeof(bool));
	double randomProbability;
	for (int i = 0; i < popSize; i++) {
		randomProbability = (double)rand() / (double)RAND_MAX;
		if (randomProbability < crossoverProbability) {
			selectedForCrossover[i] = 1;
		}
		else {
			selectedForCrossover[i] = 0;
		}
	}
	int randomPosition;
	int *chromosome1 = 0, *chromosome2 = 0;
	int chromosome1Index = -1, chromosome2Index = -1;
	for (int i = 0; i < popSize; i++) {
		if (selectedForCrossover[i] == 1) {
			if (chromosome1Index == -1) {
				chromosome1 = population[i];
				chromosome1Index = i;
			}
			else {
				chromosome2 = population[i];
				chromosome2Index = i;
			}
			if (chromosome1Index != -1 && chromosome2Index != -1) {
				randomPosition = (rand() % numberOfVerticies) + 1;

				applyCrossover(chromosome1, chromosome2, numberOfVerticies, randomPosition);

				population[chromosome1Index] = chromosome1;
				population[chromosome2Index] = chromosome2;
				chromosome1Index = -1;
				chromosome2Index = -1;
			}
		}
	}
}

void GAColoring(graph g, int *coloring, int popSize, double probabilityOfMutation, double probabilityOfCrossover, int numberOfGenerations) {
	
	int **population = generateRandomPopulation(popSize, g.numberOfVerticies);

	double *populationFitness = evaluatePopulation(population, g, popSize);
	
	for (int t = 0; t < numberOfGenerations; t++) {
		population = selectNewPopulation(population, populationFitness, popSize, g.numberOfVerticies);
		applyGAOperators(g, population, popSize, g.numberOfVerticies, probabilityOfMutation, probabilityOfCrossover);
		populationFitness = evaluatePopulation(population, g, popSize);
	}
	double max1 = -1;
	int pos = -1;
	for (int i = 0; i < popSize; i++) {
		if (populationFitness[i] > max1) {
			max1 = populationFitness[i];
			pos = i;
		}
	}
	
	printf("Fitness %lf\n", max1);
	for (int i = 1; i <= g.numberOfVerticies; i++) {
		coloring[i] = population[pos][i];
	}

	free(population);
	free(populationFitness);
}