#include <stdio.h>
#include <stdlib.h>

static const int BLACK = 0;
static const int RED = 1;

typedef struct Node* PtrNode;
struct Node{
	int value;
	int color; // Black = 0, Red = 1
	PtrNode lchild;
	PtrNode rchild;
};

void print_node(PtrNode node){
	static char color[] = {'B', 'R'};
	if(node->lchild){
		print_node(node->lchild);
	}
	if(node->rchild){
		print_node(node->rchild);
	}
	printf("%c:%d\n", color[node->color] , node->value);	
}

PtrNode new_node(int value){
	PtrNode node;
	node = malloc(sizeof(struct Node));
	node->value = value;
	node->color = RED;
	node->lchild = NULL;
	node->rchild = NULL;
	return node;
}

void clear_recu(PtrNode node){
	if(node->lchild){
		clear_recu(node->lchild);
	}	
	if(node->rchild){
		clear_recu(node->rchild);
	}
	free(node);
}

void clear_nodes(PtrNode* root){
	if(*root) return;
	clear_recu(*root);
	*root = NULL;
}


int main()
{
	PtrNode root;
	root = new_node(3);
	root->rchild = new_node(4);
	root->lchild = new_node(1);
	
	print_node(root);
	
	clear_nodes(&root);
	return 0;
}
