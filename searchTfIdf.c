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
	node_tfid next;
} _node_tfid;

typedef struct list_tfidf {
	int nitems;
	node_tfid first;
} list_tfidf_;

void swap(node_tfid p, node_tfid q) {
	char *temp1;
	double temp2;

	temp1 = p->url;
	temp2 = p->tfidf;

	p->url = q->url;
	p->tfidf = q->tfidf;

	q->url = temp1;
	q->tfidf = temp2;

}
void bubbleSort(list List){
	node_tfid start = List->first;
	int swapped;
	node_tfid ptr1;
	node_tfid lptr = NULL;

	/* Checking for empty list */
	if (start == NULL)
		return;

	do
	{
		swapped = 0;
		ptr1 = start;

		while (ptr1->next != lptr)
		{
			if (ptr1->tfidf < ptr1->next->tfidf)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
	while (swapped);
}


static list_tfidf_ *newList() {
	list_tfidf_ *new;
	new = malloc(sizeof(list_tfidf_));
	new->first = NULL;
	new->nitems = 0;
	return new;

}
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
void increment_tfidf(list List, char *url, double result) {
	node_tfid curr = List->first;
	while (curr != NULL) {
		if (strcmp(url, curr->url) == 0) {
			curr->tfidf += result;
			break;
		}
		curr = curr->next;
	}
}

void node_insert(list List, char *url, double result) {
	node_tfid curr = List->first;
	List->nitems++;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	node_tfid new = new_node(url);
	new->tfidf = result;
	curr->next = new;
}
void norm_word(char *it) {
	if (strcmp(it, "Section-2") == 0)return;
	for (int i = 0; i < strlen(it) + 1; i++) {
		if (isupper(it[i]) != 0) {
			it[i] = tolower(it[i]);
		} else if (isalpha(it[i]) == 0 ) {
			it[i] = '\0';
			return;
		}
	}
	return;
}

double term_frequency(char *t, char *d) {
	char *url = strdup(d);
	strcat(url, ".txt");
	char compare[100000];
	char term[strlen(t) + 1];
	norm_word(term);
	strcpy(compare, t);
	FILE *open = fopen(url, "r"); //read url

	// count how many occurance of term in d
	double freq_of_term = 0.0;
	while (fscanf(open, "%s", compare) != EOF) {
		norm_word(compare);
		if (strcmp(compare, t) == 0)freq_of_term++;
	}

	rewind(open);
	double total_words = 0.0;
	char *section = "Section-2";

	//count total number of words in url
	int section_2_flag = 0;
	while (fscanf(open, "%s", compare) != EOF) {
		if (strcmp(compare, section) == 0) {
			section_2_flag++;
			break;
		}
	}
	while (fscanf(open, "%s", compare) != EOF) {
		total_words++;
	}
	fclose(open);
	total_words -= 2; // dont count #end Section-2
	return freq_of_term / total_words;
}

double idf(char *t, char *collection) { //lowercase t, collection.txt
	FILE *open = fopen(collection, "r");
	double numURL = 0.0;
	char dummy[10000];

	while (fscanf(open, "%s", dummy) != EOF) { // total number of documents
		numURL += 1;
	}

	FILE *fp = fopen("invertedIndex.txt", "r");
	char line[1000];
	char word[strlen(t) + 1];
	char *p;
	strcpy(word, t);
	norm_word(word);
	int found = 0;
	while (fgets(line, 1000, fp)) { // # of documents containing the term
		p = strstr(line, word);
		if (p) {
			found += 1;
			break;
		}
	}
	if (found == 0)return 0;
	double count = 0;
	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i] == ' ')count++;
	}
	return log10(numURL / count);
}




int main(int argc, char *argv[]) {
	int i = 1;
	list tfidfs = newList();
	while (i < argc) { // read all the argvs
		char search[strlen(argv[i]) + 1];
		strcpy(search, argv[i]);
		FILE *fp = fopen("invertedIndex.txt", "r");
		char line[1000];
		char *p;
		while (fgets(line, 1000, fp)) { // go to the word in invertedIndex.txt
			char* newline = strchr(line, '\n');
			if (newline)*newline = '\0';
			p = strstr(line, search);
			if (p) {
				char *new_word = strtok(line, " "); // contain urls of terms
				while (new_word != NULL) { // to extract the urls
					new_word = strtok(NULL, " "); // skip the word. eg: skip mars in 'mars url1 url2'
					if (new_word == NULL)break;
					int found = find_url_in_list(tfidfs->first, new_word);
					double tf = term_frequency(search, new_word);
					double idfs = idf(search, "collection.txt");
					double results_tfidf = tf * idfs;
					//printf("url is %s tfidf is %lf idfs is %f\n", new_word, tf, idfs);
					if (tfidfs->nitems == 0 ) { // initialize the list
						node_tfid new = new_node(new_word);
						tfidfs->nitems++;
						tfidfs->first = new;
						tfidfs->first->tfidf = results_tfidf;
					} else {
						if (found == 1) {
							// node already in list
							increment_tfidf(tfidfs, new_word, results_tfidf);
						} else {
							// create new node and add to list
							node_insert(tfidfs, new_word, results_tfidf);
						}
					}
				}
			}

		}
		fclose(fp);
		i += 1;
	}
	bubbleSort(tfidfs);
	node_tfid curr = tfidfs->first;
	i = 0;
	while (curr != NULL) {
		printf("%s %f \n", curr->url, curr->tfidf);
		curr = curr->next;
		i += 1;
		if (i == 30)break;
	}
}

