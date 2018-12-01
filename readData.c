#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "graph.h"

//Gets all urls
Set GetCollection() {

	Set url_set = newSet();
	FILE *f = fopen("collection.txt", "r");
	char *url = malloc(14);
	int size = 0;
	while (fscanf(f, "%s", url) != EOF) {
		insertInto(url_set, url);
		size++;
	}
	return url_set;
}

//Forms a graph from urls
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
		int end = 0;
		while (fscanf(f, "%s", next_url) != 0) {
			if (strcmp(next_url, "#end") == 0) {
				end = 1;
			} else if (end == 1 && strcmp(next_url, "Section-1") == 0) {
				break;
			} else if (end == 1){
				end = 0;
				addEdge(web, url, "#end");
			}
			if (end == 0) {
				addEdge(web, url, next_url);
			}
		}
		i++;
	}
	return web;
}

