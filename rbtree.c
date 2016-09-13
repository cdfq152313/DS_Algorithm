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

void _clear_recu(PtrNode node){
	if(node->lchild){
		_clear_recu(node->lchild);
	}	
	if(node->rchild){
		_clear_recu(node->rchild);
	}
	free(node);
}

void clear_nodes(PtrNode* root){
	if(*root) return;
	_clear_recu(*root);
	*root = NULL;
}

PtrNode _insert_recu(PtrNode cur, PtrNode node){
	if(cur->value  >= node->value){
		if(cur->lchild){
			return _insert_recu(cur->lchild, node);
		}	
		else{
			return cur->lchild = node;
		}
	}
	else{
		if(cur->rchild){
			return _insert_recu(cur->rchild, node);
		}	
		else{
			return cur->rchild = node;
		}
	}
}
PtrNode insert_node(PtrNode *root, PtrNode node){
	// No root
	if(!*root){
		*root = node;
		return node;
	}
	return _insert_recu(*root, node);
}

int main()
{
	PtrNode root = NULL;
	insert_node(&root, new_node(5));
	insert_node(&root, new_node(2));
	insert_node(&root, new_node(4));
	insert_node(&root, new_node(1));
	
	print_node(root);
	
	clear_nodes(&root);
	return 0;
}
