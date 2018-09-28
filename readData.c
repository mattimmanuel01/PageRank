#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "graph.h"





Set GetCollection() {
	Set url_set = newSet();
	FILE *f = fopen("collection.txt", "r");
	char *url = malloc(13);
	int size = 0;
	while (fscanf(f, "%s", url) != EOF) {
		insertInto(url_set, url);
		size++;
	}
	showSet(url_set);
	return url_set;


	Graph web = newGraph(size);
	showGraph(web, 0);
}


Graph GetGraph(Set s) {
	int size = nElems(s);
	Graph web = newGraph(size);
	int i = 0;
	char *next_url = malloc(13);
	while (i < size) {
		char *file = get_element(s, i);
		char *url = strdup(file);
		strcat(file, ".txt");
		FILE *f = fopen(file, "r");
		fscanf(f, "%s", next_url);
		fscanf(f, "%s", next_url);
		fscanf(f, "%s", next_url);
		while (next_url[0] != '#') {
			addEdge(web, url, next_url);
			fscanf(f, "%s", next_url);
		}
		i++;
	}
	return web;
}

void GetInvertedList(Set s) {
	
}

int main(void) {
	Set url = GetCollection();
	Graph g = GetGraph(url);
	showGraph(g, 0);

}
