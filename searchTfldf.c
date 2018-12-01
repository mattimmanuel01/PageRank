#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct tfidf_node *node_tfid;
typedef struct list_tfidf *list;

typedef struct tfidf_node {
	char *url;
	double tfidf;
	int appearances;
	node_tfid next;
} _node_tfid;

typedef struct list_tfidf {
	int nitems;
	node_tfid first;
} list_tfidf_;

//Creates new list
static list_tfidf_ *newList() {

	list_tfidf_ *new;
	new = malloc(sizeof(list_tfidf_));
	new->first = NULL;
	new->nitems = 0;
	return new;
}

//Creates new node
static _node_tfid *new_node(char *it) {

	_node_tfid *new;
	new = malloc(sizeof(_node_tfid));
	if (new == NULL) {
		printf("Malloc not successful\n");
		abort();
	}
	new->url = strdup(it);
	new->tfidf = 0.0;
	new->next = NULL;
	return new;
}

//Checks whether url is in list
int find_url_in_list(node_tfid Head, char *t) {

	node_tfid curr = Head;
	int found = 0;
	while (curr != NULL) {
		if (strcmp(curr->url, t) == 0) {
			found += 1;
			break;
		}
		curr = curr->next;
	}
	return found;
}

//Finds node and adds tfidf
void increment_tfidf(list List, char *url, double result) {

	node_tfid curr = List->first;
	while (curr != NULL) {
		if (strcmp(url, curr->url) == 0) {
			curr->tfidf += result;
			curr->appearances++;
			break;
		}
		curr = curr->next;
	}
}

//Insert node into list
void node_insert(list List, char *url, double result) {

	node_tfid curr = List->first;
	List->nitems++;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	node_tfid new = new_node(url);
	new->tfidf = result;
	new->appearances = 1;
	curr->next = new;
}

//Normalizes words for checking
void norm_word(char *it) {

	int size = strlen(it);
    for (int i = size-1; i >= 0; i--) {
     	if (it[i] == ',' || it[i] == '.' || it[i] == ';' || it[i] == '?') {
         	it[i] = '\0';
        } else {
        	break;
        }
   	}
   	size = strlen(it);
   	for (int i = 0; i < size; i++) {
   		if (isalpha(it[i]) != 0) {
   			it[i] = tolower(it[i]);
   		}
   	}
   return;
}


//Calculate term frequency
double term_frequency(char *word, char *url) {

	char compare[1000];
	strcpy(compare, word);

	char *url_link = strdup(url);
	strcat(url_link, ".txt");
	FILE *open = fopen(url_link, "r"); 

	double freq_of_term = 0.0;
	while (fscanf(open, "%s", compare) != EOF) {
		norm_word(compare);
		if (strcmp(compare, word) == 0) {
			freq_of_term++;
		}
	}

	// make sure you count up correctly
	rewind(open);
	double total_words = 0.0;
	int start = 0;
	while (fscanf(open, "%s", compare) != EOF) {
		if (strcmp(compare, "#start") == 0) {
			start = 1;
		} else if (strcmp(compare, "Section-2") == 0 && start == 1) {
			break;
		} else {
			start = 0;
		}
	}

	while (fscanf(open, "%s", compare) != EOF) {
		norm_word(compare);
		if (compare[0] != '\0') {
			total_words++;
		}
	}

	fclose(open);
	total_words -= 2;
	return freq_of_term / total_words;
}

double count_urls(char *new_url) {

	int count = 0;
	int check = 1;
	int i = 0;
	while (new_url[i] != '\n') {
		if (check == 1 && new_url[i] != ' ') {
			count++;
			check = 0;
		} if (new_url[i] == ' ') {
			check = 1;
		} 
		i++;
	}
	return count;
}

//Calulate idf
double idf(char *term) { //lowercase t, collection.txt

	FILE *open = fopen("collection.txt", "r");
	double numURL = 0.0;
	char *line = malloc(1000);

	while (fscanf(open, "%s", line) != EOF) { // total number of documents
		numURL += 1;
	}

	FILE *fp = fopen("invertedIndex.txt", "r");

	char word[strlen(term) + 1];
	strcpy(word, term);
	int found = 0;

	while (fscanf(fp, "%s", line) != EOF) { // go to the word in invertedIndex.txt
		if (strcmp(line, word) == 0) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		return 0;
	}
	fgets(line, 1000, fp);
	int k = 0;
	while (line[k] == ' ') {
		k++;
	}
	line = &line[k];
	double count = count_urls(strdup(line));
	return log10(numURL / count);
}

//Calculate tfidf
double calculate_tfidf(char *search, char *new_url,list tfidfs) {
	double tf = term_frequency(search, new_url);
	double idfs = idf(search);
	double results_tfidf = tf * idfs;
	return results_tfidf;
}

//Inserts node into list
void insertnode(list tfidfs, char *new_url, double result_tfidf) {
	int found = find_url_in_list(tfidfs->first, new_url);
	if (tfidfs->nitems == 0) { // initialize the list
		node_tfid new = new_node(new_url);
		tfidfs->nitems++;
		tfidfs->first = new;
		tfidfs->first->tfidf = result_tfidf;
		tfidfs->first->appearances = 1;
	} else {
		if (found == 1) {
			increment_tfidf(tfidfs, new_url, result_tfidf);
		} else {
			node_insert(tfidfs, new_url, result_tfidf);
		}
	}
}

//Prints out values in proper order
void output(list tfidfs) {
	
	int i = 0;
	while (i < 30) {
		int largest_app = 0;
		node_tfid curr = tfidfs->first;
		while (curr != NULL) {
			if (largest_app < curr->appearances) {
				largest_app = curr->appearances;
			}
			curr = curr->next;
		}
		curr = tfidfs->first;
		node_tfid largest;
		double largest_tfidf = 0;
		while (curr != NULL) {
			if (curr->appearances == largest_app && largest_tfidf < curr->tfidf) {
				largest_tfidf = curr->tfidf;
				largest = curr;
			}
			curr = curr->next;
		}
		if (largest_app == 0) {
			break;
		}
		printf("%s %.6f\n", largest->url, largest->tfidf);
		largest->appearances = 0;
		i++;
	}
}

int main(int argc, char *argv[]) {

	list tfidfs = newList();
	FILE *fp = fopen("invertedIndex.txt", "r");
	char *line = malloc(1000);
	char *found;
	int i = 1;
	while (i < argc) { 
		char *search = malloc(strlen(argv[i]) + 1);
		strcpy(search, argv[i]);
		while (fgets(line, 1000, fp) != NULL) {
			char *url = strtok(line, "\n");
			url = strtok(line, " ");
			if (strcmp(url, search) == 0) {
				url = strtok(NULL, " ");
				while (url != NULL) {
 					double result_tfidf = calculate_tfidf(search, url, tfidfs);
					insertnode(tfidfs, url, result_tfidf);
					url = strtok(NULL, " ");
				}
			}
		}
		rewind(fp);
		i++;
	}
	output(tfidfs);
}