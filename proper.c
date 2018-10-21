#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readData.h"
#include "set.h"
#include "graph.h"
#include <math.h>
void iterate_page_rank(int size, double curr_page_rank[size], double prev_page_rank[size]);
double calculate_diff(int size, double curr_page_rank[size], double prev_page_rank[size]);
double weighted_formula(char *url, Graph web, int size, Set urls, double curr_page_rank[size]);
double out_links(char *url, Graph web, Set urls);
double in_links(char *url, Graph web, Set urls);
double weighted_value(char *prev_url, Graph web, int size, Set urls, double page_rank, char *url);

int main (int arg , char *argv[]) {
	Set urls = GetCollection();
	Graph web = GetGraph(urls);
	showGraph(web, 1);
	showGraph(web, 0);

	int size = nElems(urls);

	double curr_page_rank[size];
	double prev_page_rank[size];
	int i = 0;
	for (i = 0; i < size; i++) {
		curr_page_rank[i] = (double)1.0/(double)size;
		prev_page_rank[i] = (double)1.0/(double)size;
	}	
	int iteration = 0;
	double diffPr;
	double d;
	sscanf(argv[2], "%lf", &diffPr);
	sscanf(argv[1], "%lf", &d);

	double diff = diffPr;

	int maxIterations = atoi(argv[3]);
	while (iteration < maxIterations && diff >= diffPr) {
		int i = 0;
		while (i < size) {
			char *url = get_element(urls, i);
			curr_page_rank[i] = ((1.0-d)/(double)size) + (d * weighted_formula(url, web,size, urls, prev_page_rank));
			i++;
		}
		diff = calculate_diff(size, curr_page_rank, prev_page_rank);
		iterate_page_rank(size, curr_page_rank, prev_page_rank);
		iteration++;
	}
	i = 0;
	while (i < size) {
		printf("%s\n", get_element(urls, i));
		printf("%f\n",curr_page_rank[i]);
		i++;
	}
}

void iterate_page_rank(int size, double curr_page_rank[size], double prev_page_rank[size]) {
	int i = 0;
	while (i < size) {
		prev_page_rank[i] = curr_page_rank[i];
		i++;
	}
}

double calculate_diff(int size, double curr_page_rank[size], double prev_page_rank[size]) {
	double value = 0;
	int i = 0;
	while (i < size) {
		value = value + fabs(curr_page_rank[i] - prev_page_rank[i]);
		i++;
	}
	return value;
}

double weighted_formula(char *url, Graph web, int size, Set urls, double page_rank[size]) {
	Set inlinks = newSet();
	double value = 0;
	int i = 0;
	for (i = 0; i < size; i++) {
		char *prev_url = get_element(urls, i);
		if (isConnected(web, prev_url, url)) {
			if (strcmp(prev_url, url) != 0  && !isElem(inlinks, prev_url)) {
				insertInto(inlinks, prev_url);
				value += weighted_value(prev_url, web,size, urls, page_rank[i], url);
			}
		}
	}
	return value;
}

double weighted_value(char *init_url, Graph web, int size, Set urls, double page_rank, char *url) {
	Set outlinks = newSet();
	int outlinks_num = 0;
	int i = 0;
	for (i = 0; i < size; i++) {
		char *prev_url = get_element(urls, i);
		if (isConnected(web, init_url, prev_url)) {
			if (strcmp(prev_url, init_url) != 0  && !isElem(outlinks, prev_url)) {
				insertInto(outlinks, prev_url);
				outlinks_num++;

			}
		}
	}
	double url_outlink;
	double url_inlink;
	double total_outlinks = 0;
	i = 0;
	while (i < outlinks_num) {
		char *prev_url = get_element(outlinks, i);
		if (strcmp(prev_url, url) == 0) {
			url_outlink = out_links(prev_url, web, urls);
		}
		total_outlinks += out_links(prev_url, web, urls);
		i++;
	}
	double total_inlinks = 0;
	i = 0;
	while (i < outlinks_num) {
		char *prev_url = get_element(outlinks, i);
		if (strcmp(prev_url, url) == 0) {
			url_inlink = in_links(prev_url, web, urls);
		}
		total_inlinks += in_links(prev_url, web, urls);
		i++;
	}
	return page_rank * url_inlink/total_inlinks * url_outlink/total_outlinks;
}

double out_links(char *url, Graph web, Set urls) {
	double out_links = 0;
	int size = nElems(urls);

	int i = 0;
	for (i = 0; i < size; i++) {
		char *prev_url = get_element(urls, i);
		if (isConnected(web, url, prev_url)) {
			if (strcmp(url, prev_url) != 0) {
				out_links++;
			}
		}
	}
	if (out_links == 0.0) {
		out_links = 0.5;
	}
	return out_links;
}

double in_links(char *url, Graph web, Set urls) {
	double in_links = 0;
	int size = nElems(urls);

	int i = 0;
	for (i = 0; i < size; i++) {
		char *prev_url = get_element(urls, i);
		if (isConnected(web, prev_url, url)) {
			if (strcmp(url, prev_url) != 0) {
				in_links++;
			}
		}
	}
	return in_links;
}


