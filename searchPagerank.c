#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "set.h"
#include <math.h>
#include <assert.h>

void findmatches(int size, int appearances[size], Set urls, char *word);

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

 void findmatches(int size, int appearances[size], Set urls, char *word) {
 	FILE *text = fopen("invertedIndex.txt", "r");


 	char *match = malloc(10);
 	char *line = malloc(1000);
 	while (fscanf(text,"%s", match) == 1) {
 		if (strcmp(word, match) == 0) {
 			fgets(line, 1000, text);
 		 	strtok(line, "\n");
 			line = &line[1];
 			break;
 		}
 	}
   //	int line_size = strlen(line);
 	int position = 0;
 	while (strtok(&line[position], " ")!= NULL) {
 	 	int index = get_index(urls, &line[position]);
 	 	appearances[index]++;
 		position = position + strlen(&line[position])+ 1;
 	}
 	return;
 }