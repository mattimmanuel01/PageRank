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


void printTree(Tree t , FILE *output) {
	if(t->left!=NULL)printTree(t->left,output);
	fprintf(output,"%s",t->word);
	ListNode curr = t->urlList;
	while(curr != NULL){
		fprintf(output," %s",curr->url );
		curr = curr->next;
	}
	fprintf(output,"\n");
	if(t->right!=NULL)printTree(t->right,output);

}

																
ListNode new_list_node(char *it) {
	ListNode new = malloc(sizeof(List));
	assert(new != NULL);
	new->url = strdup(it);
	new->next = NULL;
	return new;
}
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

void norm_word(char *it){
	if(strcmp(it,"Section-2") == 0)return;
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
int main(void) {


//	For each url in List_of_Urls
//        read <url>.txt file, and update inverted index

	Tree new = NULL;

	FILE *f = fopen("collection.txt", "r");
	char *url = malloc(13);
	char *section = "Section-2";
	int count_section = 0;
	while (fscanf(f, "%s", url) != EOF) {
		//printf("%s\n",url );
		char *open = strdup(url);
		char *url_in = strdup(url);
		strcat(open, ".txt");
		FILE *file_open = fopen(open, "r"); // open url from collection.txt
		count_section = 0;
		char char_inside[100000] = {};

		while (fscanf(file_open, "%s", char_inside) != EOF) { //scannning url.txt
			norm_word(char_inside);
			if (strcmp(char_inside, section) == 0) {
				count_section += 1;
			}
			if (count_section == 1) {
				if(isalpha(char_inside[0]) != 1)continue;
				if ((strcmp(char_inside,"Section-2") != 0) ||
				 (!TreeSearch(new, char_inside) && strcmp(char_inside,"#end") != 0 && strcmp(char_inside,"Section-2") != 0)) { // fix
					new = TreeInsert(new, char_inside);
					Tree node = get_node(new, char_inside);
					insert_node_url(node,url_in);
				}
			}
		}
	}
FILE *out =  fopen("invertedIndex.txt","w");
printTree(new,out) ;
}



