#include <stdio.h>
#include <stdlib.h>

static const int BLACK = 0;
static const int RED = 1;

typedef struct Node* PtrNode;
struct Node{
	int value;
	int color; // Black = 0, Red = 1
	int null;
	PtrNode parent;
	PtrNode lchild;
	PtrNode rchild;
};

void _print_node(PtrNode node){
	static char *color[] = {"black", "red"};
	if(node->lchild){
		_print_node(node->lchild);
	}
	printf("\t\"%d\" [fontcolor=white, style=filled, fillcolor=%s]\n", node->value, color[node->color]);	
	if(node->rchild){
		_print_node(node->rchild);
	}
}

static int nil_count = 0;
void _print_dot_recu(PtrNode node){
	static char text[] = "\t\"%d\" -- \"%d\"\n";
	static char text2[] = "\t\"%d\" -- \"nil_%d\"\n";
		if(node->lchild){
		printf(text, node->value, node->lchild->value);	
		_print_dot_recu(node->lchild);
	}
	else{
		printf(text2, node->value, nil_count++);	
	}
	if(node->rchild){
		printf(text, node->value, node->rchild->value);	
		_print_dot_recu(node->rchild);
	}
	else{
		printf(text2, node->value, nil_count++);	
	}
}
void print_dot(PtrNode root){
	if(!root){
		printf("No Node\n");
		return;
	}
	printf("graph {\n");
	_print_node(root);
	printf("\n");
	_print_dot_recu(root);
	printf("\n");
	for(int i = 0; i < nil_count; ++i){
		printf("\t\"nil_%d\" [style = filled, fontcolor=white, fillcolor=black]\n", i);
	}
	printf("}\n");
}

PtrNode new_node(int value){
	PtrNode node;
	node = malloc(sizeof(struct Node));
	node->value = value;
	node->color = RED;
	node->null = 0;
	node->parent = NULL;
	node->lchild = NULL;
	node->rchild = NULL;
	return node;
}

PtrNode new_node_empty(){
	PtrNode node;
	node = malloc(sizeof(struct Node));
	node->color = BLACK;
	node->value = 0;
	node->null = 1;
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

PtrNode _insert_recu(PtrNode cur, int value){
	if(cur->value == value){
		return NULL;
	}
	else if(cur->value > value){
		if(cur->lchild){
			return _insert_recu(cur->lchild, value);
		}	
		else{
			PtrNode node = new_node(value);
			cur->lchild = node;
			node->parent = cur;
			return node;
		}
	}
	else{
		if(cur->rchild){
			return _insert_recu(cur->rchild, value);
		}	
		else{
			PtrNode node = new_node(value);
			cur->rchild = node;
			node->parent = cur;
			return node;
		}
	}
}
void insert_node(PtrNode *root, int value){
	PtrNode node;
	// No root
	if(!*root){
		node = new_node(value);
	}
	else{
		node = _insert_recu(*root, value);
	}
	// if insert successed, adjust rbtree.
	if(node){
		*root = _insert_adjust(node);
	}
}

PtrNode _search_recu(PtrNode cur, int value){
	if(cur->value == value){
		return cur;
	}
	if(cur->value  > value){
		if(cur->lchild){
			return _search_recu(cur->lchild, value);
		}	
		else{
			return NULL;
		}
	}
	else{
		if(cur->rchild){
			return _search_recu(cur->rchild, value);
		}	
		else{
			return NULL;
		}
	}
}

PtrNode search_node(PtrNode root, int value){
	if(!root){
		return NULL;
	}
	else{
		return _search_recu(root, value);
	}
}

PtrNode _delete_node_with_0child(PtrNode del){
	if(del->parent){
		if(del->parent->lchild == del){
			del->parent->lchild = NULL;
			del->null = 1;
			return del;
		}
		else{
			del->parent->rchild = NULL;
			del->null = 1;
			return del;
		}
	}
	else{
		del->null = 1;
		return del;
	}
}

PtrNode _delete_node_with_1child(PtrNode del){
	PtrNode child = del->lchild ? del->lchild: del->rchild;
	child->parent = del->parent;
	if(del->parent){
		if(del->parent->lchild == del){
			del->parent->lchild = child;
			free(del);
			return child;
		}
		else{
			del->parent->rchild = child;
			free(del);
			return child;
		}
	}
	else{
		free(del);
		return child;
	}
}

PtrNode _delete_node_with_2child(PtrNode del){
	// declare
	PtrNode _delete_node(PtrNode del);
	PtrNode lmax = del->lchild;
	while(lmax->rchild != NULL){
		lmax = lmax->rchild;
	}
	del->value = lmax->value;
	return _delete_node(lmax);
}

// return x 
PtrNode _delete_node(PtrNode del){
	if(del->lchild && del->rchild){
		return _delete_node_with_2child(del);
	}
	else if(del->lchild || del->rchild){
		return _delete_node_with_1child(del);
	}
	else{
		return _delete_node_with_0child(del);
	}
}

void delete_node(PtrNode *root, int value){
	PtrNode del;
	PtrNode x;
	del = search_node(*root, value);
	if(del == NULL){
		return;
	}
	else{
		x = _delete_node(del);
		if(x->parent == NULL){
			if(x->null){
				*root = NULL;
			}
			else{
				*root = x;
			}
		}
		if(x->null){
			free(x);
		}
	}
}

int main()
{
	PtrNode root = NULL;
	int insert_number;
	int delete_number;
	int value;
	
	scanf("%d", &insert_number);
	for(int i = 0; i < insert_number; ++i){
		scanf("%d", &value); 
		insert_node(&root, value);
	}
	printf("insert\n");
	print_dot(root);
	scanf("%d", &delete_number);
	for(int i = 0; i < insert_number; ++i){
		scanf("%d", &value); 
		delete_node(&root, value);
	}
	printf("\n\ndelete\n");
	print_dot(root);

	clear_nodes(&root);
	return 0;
}
