#include <stdio.h>
#include <stdlib.h>

static const int BLACK = 0;
static const int RED = 1;

typedef struct Node* PtrNode;
struct Node{
	int value;
	int color; // Black = 0, Red = 1
	PtrNode parent;
	PtrNode lchild;
	PtrNode rchild;
};

void print_node(PtrNode node){
	static char color[] = {'B', 'R'};
	if(node->lchild){
		print_node(node->lchild);
	}
	printf("%c:%d\n", color[node->color] , node->value);	
	if(node->rchild){
		print_node(node->rchild);
	}
}

PtrNode new_node(int value){
	PtrNode node;
	node = malloc(sizeof(struct Node));
	node->value = value;
	node->color = RED;
	node->parent = NULL;
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
	if(!*root) return;
	_clear_recu(*root);
	*root = NULL;
}

int _uncle_color(PtrNode node){
	// cause root is BLACK
	// when (parent is red) search uncle color, 
	// there are always has grandparent
	PtrNode grandparent =  node->parent->parent;
	// nil node is black
	if(!grandparent->lchild || !grandparent->rchild){
		return BLACK;
	}
	// parent's left child is uncle 
	else if(grandparent->lchild != node->parent){
		return grandparent->lchild->color;	
	}
	// parent's right child is uncle
	else{
		return grandparent->rchild->color;	
	}
}

void _draw_node(PtrNode node){
	// cause root is BLACK
	// when (parent is red) search uncle color, 
	// there are always has grandparent
	PtrNode grandparent = node->parent->parent;
	grandparent->color = RED;
	if( grandparent->lchild ){
		grandparent->lchild->color = BLACK;
	}
	if( grandparent->rchild ){
		grandparent->rchild->color = BLACK;
	}
}

PtrNode get_root(PtrNode node){
	return node->parent ? get_root(node->parent) : node;
}

void _left_rotate(PtrNode node){
	PtrNode parent = node->parent;
	PtrNode grandparent = parent->parent;

	parent->rchild = node->lchild;
	parent->parent = node;
	node->lchild = parent;
	node->parent = grandparent;

	if(grandparent){
		if(grandparent->lchild == parent){
			grandparent->lchild = node;
		}
		else{
			grandparent->rchild = node;
		}
	}
}

void _right_rotate(PtrNode node){
	PtrNode parent = node->parent;
	PtrNode grandparent = parent->parent;

	parent->lchild = node->rchild;
	parent->parent = node;
	node->rchild = parent;
	node->parent = grandparent;

	if(grandparent){
		if(grandparent->lchild == parent){
			grandparent->lchild = node; 
		}
		else{
			grandparent->rchild = node; 
		}
	}
}
// return root
PtrNode _insert_adjust(PtrNode node){
	// root
	if( !node->parent){ 
		node->color = BLACK;
		return node;
	}
	// parent is black
	else if( node->parent->color == BLACK ){
		return get_root(node);
	}
	// uncle is red 
	else if(_uncle_color(node) == RED){
		_draw_node(node);
		return _insert_adjust(node->parent->parent);
	}
	// uncle is black
	else{
		// parent is left child
		if( node->parent == node->parent->parent->lchild ){
			// child is right child
			if(node == node->parent->rchild){
				_left_rotate(node);
				_right_rotate(node);
			}
			// child is left child
			else{
				_right_rotate(node->parent);
			}
		}
		// parent is right child
		else{
			// child is left child	
			if(node == node->parent->lchild){
				_right_rotate(node);
				_left_rotate(node);
			}
			// child is right child
			else{
				_left_rotate(node->parent);
			}
		}

		node->lchild->color = RED;
		node->rchild->color = RED;
		return get_root(node);
	}
}

void _insert_recu(PtrNode cur, PtrNode node){
	if(cur->value  >= node->value){
		if(cur->lchild){
			_insert_recu(cur->lchild, node);
		}	
		else{
			cur->lchild = node;
			node->parent = cur;
		}
	}
	else{
		if(cur->rchild){
			_insert_recu(cur->rchild, node);
		}	
		else{
			cur->rchild = node;
			node->parent = cur;
		}
	}
}
void insert_node(PtrNode *root, PtrNode node){
	// No root
	if(!*root){
		*root = node;
	}
	else{
		_insert_recu(*root, node);
	}
	*root = _insert_adjust(node);
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
