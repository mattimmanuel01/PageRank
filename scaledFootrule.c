#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define TRUE 1
#define FALSE 0

typedef struct scalednode *sc_node;
typedef struct scaled_list *sc_list;

typedef struct scalednode {
	char *url;
	int position;
	sc_node next;
} scnode;

typedef struct scaled_list {
	int nitems;
	sc_node head;
} sclist;

static sclist *new_sc_list() {
	sclist *new;
	new = malloc(sizeof(sclist));
	new->head = NULL;
	new->nitems = 0;
	return new;
}

static scnode *new_sc_node(char *url) {
	scnode *new;
	new = malloc(sizeof(scnode));
	if (new == NULL) {printf("failed malloc\n"); abort();}
	new->url = strdup(url);
	new->position = 0;
	new->next = NULL;
	return new;
}
void insert_node_to_list(sc_list list, char *insert) {
	sc_node new = new_sc_node(insert);
	if (list->nitems == 0) {
		list->head = new;
	} else {
		sc_node curr = list->head;
		while (curr->next != NULL) {
			curr = curr->next;
		}
		curr->next = new;

	}
	list->nitems++;
	return;
}
// find if url already exists in the list
int find_url_in_list(sc_list list, char *new) {
	sc_node curr = list->head;
	while (curr != NULL) {
		if (strcmp(curr->url, new) == 0)return TRUE;
		curr = curr->next;
	}
	return FALSE;
}
void print_list(sc_list list) {
	sc_node curr = list->head;
	while (curr != NULL) {
		printf("%s\n", curr->url);
		curr = curr->next;
	}
}
int main(int argc, char *argv[]) {
	// find union url
	sc_list union_list = new_sc_list();
	int i;
	for (i = 1; i < argc; i++) {
		char compare[1000];
		FILE *open = fopen(argv[i], "r");
		while (fscanf(open, "%s", compare) != EOF) {
			if (!find_url_in_list(union_list, compare)) {
				insert_node_to_list(union_list, compare);
			}
		}
	}

	print_list(union_list);




}