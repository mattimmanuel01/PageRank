#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "set.h"
#include <math.h>
#include <assert.h>
#include "graph.h"

void findmatches(int size, int appearances[size], Set urls, char *word);
void output(int size, int appearances[size], Set urls);

int main (int argc, char *argv[]) {

	Set urls = newSet();
	FILE *f = fopen("pagerankList.txt", "r");

	char *url = malloc(100);
	while (fgets(url, 100, f)!= NULL) {
		strtok(url, ",");
		insertUnordered(urls, url);
	}

	int size = nElems(urls);
	int appearances[size];

	int i = 0;
	while (i < size) {
		appearances[i] = 0;
		i++;
	}

	i = 1;
	while (i < argc) {
		findmatches(size, appearances, urls, argv[i]);
		i++;
	}
	output(size, appearances, urls);
}

//Finds all urls for word and increment appearances;
void findmatches(int size, int appearances[size], Set urls, char *word) {

 	FILE *text = fopen("invertedIndex.txt", "r");
 	char *match = malloc(10);
 	char *line = malloc(1000);
 	while (fscanf(text,"%s", match) == 1) {
 		if (strcmp(word, match) == 0) {
 			fgets(line, 1000, text);
 		 	strtok(line, "\n");
 		 	int i = 0;
 		 	while (line[i] == ' ') {
 		 		i++;
 		 	}
 		 	line = &line[i];
 			break;
 		}
 	}
 	if (strtok(line, " ")!= NULL) {
 		int index = get_index(urls, line);
 	 	appearances[index]++;
	 	while ((line = strtok(NULL, " ")) != NULL) {
	 	 	int index = get_index(urls, line);
	 	 	appearances[index]++;
	 	}
 	}

 	return;
}

//Prints output
void output(int size, int appearances[size], Set urls) {

	int k = 0;
	while (k < 30) {

		int j = 0;
		int largest = 0;
		int index = 0;
		while (j < size) {
			if (largest < appearances[j]) {
				index = j;
				largest = appearances[j];
			}
			j++;
		}
		if (largest == 0) {
			break;
		}
		appearances[index] = 0;
		printf("%s\n", get_element(urls,index));
		k++;
	}
}