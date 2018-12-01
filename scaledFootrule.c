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
	double nitems;
	sc_node head;
} sclist;

static sclist *new_sc_list() {
	sclist *new;
	new = malloc(sizeof(sclist));
	new->head = NULL;
	new->nitems = 0.0;
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

//Inserts node into list
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
	list->nitems += 1.0;
	return;
}

//Determines list size
int list_size(sc_list list) {
	int size =0;
	sc_node curr = list->head;
	while (curr != NULL) {
		size++;
		curr = curr->next;
	}
	return size;
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

//Prints out all the values in the list
void print_list(sc_list list) {
	sc_node curr = list->head;
	while (curr != NULL) {
		printf("%s\n", curr->url);
		curr = curr->next;
	}
}

//Calculates the footrule distance for a set of positions
double calculate_cost(int argc, char **args, sc_list union_list, double *positions) {
	sc_list *rankings = malloc(sizeof(sc_list) * argc + 1);
	int i = 0;
	while (i < argc - 1) {
		rankings[i] = new_sc_list();
		i++;
	}
	i = 0;
	while (i < argc - 1) {
		FILE *fp = fopen(args[i], "r");
		assert(fp != NULL);
		char line[1000];
		while (fscanf(fp,"%s",line) != EOF) {
			insert_node_to_list(rankings[i], strdup(line));
		}
		i++;
		fclose(fp);
	}

	i = 0;
	while (i < argc - 1) {
		i++;
	}
	int z = 0;
	int k = 0;
	sc_node curr = union_list->head;
	double sum = 0.0;
	while (z < (int)union_list->nitems) {
		char *url_ = strdup(curr->url);
		curr = curr->next;
		double position = positions[z];
		k = 0;
		while (k < argc - 1) {
			double position_in_ranking = 1;
			sc_node curr1 = rankings[k]->head;
			while (curr1 != NULL) {
				if (strcmp(curr1->url, url_) == 0) {
					sum = sum + fabs((position_in_ranking / rankings[k]->nitems) - (position / union_list->nitems));
				}
				position_in_ranking += 1;
				curr1 = curr1->next;
			}
			k++;
		}
		z++;
	}
	return sum;
}

//Strdup for integers
double *copy(double *combin, int size) {
	double *comb = malloc((size+1)*sizeof(double));
	int i =0;
	while (i < size) {
		comb[i] = combin[i];
		i++;
	}
	return comb;
}

//Recursive function for generating permutations
double *generate(int argc, char **args, sc_list union_list, double *combin, int num, double *string, int size) {
	if (num == 0) {
		string[size] = calculate_cost(argc, args, union_list, string);
		return string;
	}
	double *ringo = malloc(size*sizeof(double));
	ringo[size] = 10000000000;
	int i = 0;
	while (i < size) {
		if (combin[i] != 0) {
			string[size-num] = combin[i];
			double *comb = copy(combin, size);
			comb[i] = 0;
			double * value = generate(argc, args, union_list, comb, num-1, copy(string, size), size);
			if (value[size] <= ringo[size]) {
				ringo = value;
			}
		}
		i++;
	}
	return ringo;
}


int main(int argc, char *argv[]) {
	char **args = malloc(sizeof(char) * (argc + 1));
	int j = 1;
	int i = 0;
	while (j < argc) {
		args[i] = strdup(argv[j]);
		i++;
		j++;
	}

	sc_list union_list = new_sc_list();
	for (i = 1; i < argc; i++) {
		char compare[1000];
		FILE *open = fopen(argv[i], "r");
		while (fscanf(open, "%s", compare) != EOF) {
			if (!find_url_in_list(union_list, compare)) {
				insert_node_to_list(union_list, compare);
			}
		}
	}

	int size = list_size(union_list);
	double *comb = malloc((size + 1)*sizeof(double));
	comb[0] = '\0';
	double *string = malloc((size + 1)*sizeof(double));
	i = 0;
	while (i < size) {
		comb[i] = i+1;
		string[i] = 0;
		i++;
	}

	double *combination = generate(argc, args, union_list, comb, size, string, size);
	printf("%.6f\n", combination[size]);

	i = 1;
	while (i < size+1) {
		int pos = 0;
		int j =0;
		while (j < size) {
			if (combination[j] == i) {
				break;
			}
			j++;
		}
		//We know j holds proper position
		sc_node curr = union_list->head;
		int k =0;
		while (curr != NULL) {
			if (k == j) {
				printf("%s\n", curr->url);
			}
			curr= curr->next;
			k++;
		}
		i++;
	}
}