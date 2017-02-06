#include "Graph.h"

//FILE *in = freopen("date.out", "w", stdout);
void main() {
	srand(time(0));
	graph g;
	g = readGraph();
	int *coloring = (int *)malloc((g.numberOfVerticies) * sizeof(int));
	//bktColoring(1, g, 4, coloring);
	//for (int i = 1; i <= g.numberOfVerticies; i++) {
	//	printf("%d ", coloring[i]);
	//}
	//printf("\n");
	greedyColoring(g, coloring);
	for (int i = 1; i <= g.numberOfVerticies; i++) {
		printf("%d ", coloring[i]);
	}
	printf("\n");
	printf("Nr colors: %d\n", maxColor(coloring, g.numberOfVerticies));
	for (int i = 0; i < 10; i++) {
		GAColoring(g, coloring, 200, 0.5, 0.01, 25);
		printf("Colorare: ");
		for (int j = 1; j <= g.numberOfVerticies; j++) {
			printf("%d ", coloring[j]);
		}
		printf("\n");
		setColoring(g, coloring);
		printf("Muchii nepotrivite: %d\n", evaluateColoring(g) / 2);
		printf("Nr colors: %d\n", maxColor(coloring, g.numberOfVerticies));
		printf("\n");
	}
 	
	
}