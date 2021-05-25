#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
static NODE *_retrieve(NODE *root, char *key);
static void _destroy(NODE *root);
static NODE *_insert(NODE *root, NODE *newPtr);
static void _traverse(NODE *root);
static int getHeight(NODE *root);
/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create(void) {
	AVL_TREE* avl_tree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if (avl_tree == NULL) return NULL;
	avl_tree->root = NULL;
	avl_tree->count = 0;
	return avl_tree;
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy(AVL_TREE *pTree) {
	_destroy(pTree->root);
	free(pTree);
}

static void _destroy(NODE *root) {
	if (root == NULL) return;

	_destroy(root->right);
	_destroy(root->left);
	free(root->data);
	free(root);

}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/

int AVL_Insert(AVL_TREE *pTree, char *data) {
	NODE* newNode = _makeNode(data);
	if (newNode == NULL) return 0;

	pTree->root = _insert(pTree->root, newNode);
	return 1;
}
/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr){
	if (root == NULL) { // Ã¹ ³ëµå »ðÀÔ
		root = newPtr;
		return root;
	}
	else {
		if (strcmp(newPtr->data, root->data) >= 0) {
			_insert(root->right, newPtr);
		}
		else _insert(root->left, newPtr);

		int leftHeight, rightHeight;

		if (root->left == NULL) leftHeight = 0;
		else leftHeight = getHeight(root->left);

		if (root->right == NULL) rightHeight = 0;
		else rightHeight = root->right->height;

		int difference_height = leftHeight - rightHeight;

		if (difference_height == 2) {

		}
		else if (difference_height == -2) {

		};
	}
}

static NODE *_makeNode(char *data) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = strdup(data);
	newNode->height = 1;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve(AVL_TREE *pTree, char *key) {
	char* result = _retrieve(pTree->root, key);
	return result;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, char *key) {
	if (root == NULL) return NULL;
	if (strcmp(key, root->data) == 0) return root->data;
	if (strcmp(key, root->data) > 0) {
		_retrieve(root->right, key);
	}
	else _retrieve(root->left, key);
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE *pTree) {
	_traverse(pTree->root);
}
static void _traverse(NODE *root) {
	if (root == NULL) return;
	_traverse(root->left);
	printf("%s ", root->data);
	_traverse(root->right);
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE *pTree) {
	_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
static void _infix_print(NODE *root, int level) {
	if (root == NULL) return;
	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++) printf('\t');
	printf("%s\n", root->data);
	_infix_print(root->left, level + 1);
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE *root) {
	int temp1 = -1;
	int temp2 = -1;
	if (root->left != NULL) temp1 = root->left->height;
	if (root->right != NULL) temp2 = root->right->height;
	return max(temp1, temp2);
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight(NODE *root) {
	NODE* pNode = root;
	NODE* cNode = root->left;
	pNode->right = cNode->left;
	cNode->left = pNode;

	cNode->height = getHeight(cNode);
	pNode->height = getHeight(pNode);
	return cNode;

}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft(NODE *root) {
	NODE* pNode = root;
	NODE* cNode = root->right;
	pNode->right = cNode->left;
	cNode->left = pNode;

	cNode->height = getHeight(cNode);
	pNode->height = getHeight(pNode);
	return cNode;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		//printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	//AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	//printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}

