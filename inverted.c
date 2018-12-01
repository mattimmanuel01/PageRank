#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define COUNT 10 

typedef struct list *ListNode;

typedef struct list {
	char *url;
	ListNode next;
} List;

typedef struct Node *Tree;

typedef struct Node {
	char  *word;
	ListNode urlList;
	Tree left, right;
} Node;

//Prints out the tree
void printTree(Tree t , FILE *output) {

	if(t->left!=NULL)printTree(t->left,output);
	fprintf(output,"%s ",t->word);
	ListNode curr = t->urlList;
	while(curr != NULL){
		fprintf(output," %s",curr->url );
		curr = curr->next;
	}
	fprintf(output,"\n");
	if(t->right!=NULL)printTree(t->right,output);
}

//Creates a new node for the list															
ListNode new_list_node(char *it) {

	ListNode new = malloc(sizeof(List));
	assert(new != NULL);
	new->url = strdup(it);
	new->next = NULL;
	return new;
}

//Checks if node is in list
int url_in_list(ListNode urlList, char *it) {

	ListNode curr = urlList;
	while (curr != NULL) {
		if (strcmp(it, curr->url) == 0) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

//Inserts node at proper location intree
void insert_node_url(Tree t, char *it) {

	ListNode new = new_list_node(it);
	if (t->urlList == NULL ) {
		// still empty
		new->next = NULL;
		t->urlList = new;
		return;
	}
	if (!url_in_list(t->urlList, it)) {
		if (strcmp(it, t->urlList->url) < 0) {
			new->next = t->urlList;
			t->urlList = new;
			return;
		}
		else {
			// insert in order
			ListNode curr = t->urlList;
			while (curr->next != NULL && strcmp(it, curr->next->url) > 0) {
				curr = curr->next;
			}
			new->next = curr->next;
			curr->next = new;
		}
	}
}

//Make new node
Tree newNode(char *it) {

	Tree new = malloc(sizeof(Node));
	assert(new != NULL);
	new->word = strdup(it);
	new->urlList = NULL;
	new->left = new->right = NULL;
	return new;
}

// You can use strcmp to compare key strings.
// insert a new item into a Tree
Tree TreeInsert(Tree t, char *it) {

	if (t == NULL)
		return newNode(it);
	else if (strcmp(it, t->word) < 0)
		t->left = TreeInsert(t->left, it);
	else if (strcmp(it, t->word) > 0)
		t->right = TreeInsert(t->right, it);
	return t;
}

// check whether a key is in a Tree
bool TreeSearch(Tree t, char  *it) {

   if (t == NULL)
      return false;
   else if (strcmp(it,t->word) < 0)
      return TreeSearch(t->left, it);
   else if (strcmp(it,t->word) > 0)
      return TreeSearch(t->right, it);
   else                                 // it == data(t)
      return true;
}

//Gets the nodes associated with a key
Tree get_node(Tree t, char *it) {

	Tree curr = t;
	while(curr != NULL){
		if(strcmp(it,curr->word) > 0){
			curr = curr->right;
		} else if(strcmp(it,curr->word) < 0){
			curr = curr->left;
		} else{
			return curr;
		}
	}
	return NULL;
}

//Removes ending non-alphabetic characters and
//converts word to lower case
void norm_word(char *it){

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

Tree tree_insert(char *char_inside, Tree new, char*url_in) {
	if((char_inside[0]) == 0) {
		return new;
	}
	if ((!TreeSearch(new, char_inside))) { // fix
		new = TreeInsert(new, char_inside);
		Tree node = get_node(new, char_inside);
		insert_node_url(node,url_in);
	} else if (TreeSearch(new, char_inside)) {
		Tree node = get_node(new, char_inside);
		insert_node_url(node,url_in);
	}
	return new;
}

//Forms inverted index tree
int main(void) {
	
	Tree new = NULL;
	FILE *f = fopen("collection.txt", "r");
	char *url = malloc(20);
	char *section = "Section-2";
	int count_section = 0;
	int start = 0;
	int end = 0;
	while (fscanf(f, "%s", url) != EOF) {
		char *open = strdup(url);
		char *url_in = strdup(url);
		strcat(open, ".txt");
		FILE *file_open = fopen(open, "r"); // open url from collection.txt
		assert(file_open != NULL);
		count_section = 0;
		char char_inside[1000] = {};
		while (fscanf(file_open, "%s", char_inside) != EOF) { //scannning url.txt
			norm_word(char_inside);
			printf("%s\n", char_inside);
			if (strcmp(char_inside, "#start") == 0 && count_section == 0) {
				start = 1;
			} else if (strcmp(char_inside, "section-2") == 0 && start == 1 && count_section == 0)  {
				count_section += 1;
				continue;
			} else {
				start = 0;
			}
			if (strcmp(char_inside, "#end") == 0 && count_section == 1) {
				end = 1;
				continue;
			} else if (strcmp(char_inside, "section-2") == 0 && end == 1 && count_section == 1)  {
				count_section += 1;
				continue;
			} else if (end == 1 && count_section == 1) {
				end = 0;
				new = tree_insert(strdup("#end"), new, url_in);
			} else {
				end = 0;
			}
			if (count_section == 1) {
				new = tree_insert(char_inside, new, url_in);
			}
		}
		fclose(file_open);
	}
	FILE *out =  fopen("invertedIndex.txt","w");
	assert(new != NULL);
	printTree(new,out);
}

