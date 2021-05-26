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
static void _infix_print(NODE *root, int level);
static NODE *rotateRight(NODE *root);
static NODE *rotateLeft(NODE *root);
static NODE *_makeNode(char* data);
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
	pTree->count++;
	return 1;
}
/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr){
	NODE* nRoot;
	if (root == NULL) {
		root = newPtr; // �� �ڸ��� ��带 �߰�
		return root;
	}
	else {
		if (strcmp(newPtr->data,root->data) >= 0) { // �߰��� ����� data�� root�� data���� ũ�ų� ���� ���
			nRoot = _insert(root->right, newPtr); // ���� ����� ������ �ڽĳ�带 ������� insert�Լ��� ��������� ȣ��
			root->right = nRoot; // �ֽ�ȭ �� nRoot�� root�� �����ʳ��� ����
		}
		else { // �߰��� ����� data�� root�� data���� ���� ���
			nRoot = _insert(root->left, newPtr); // ���� ����� ���� �ڽĳ��� ����
			root->left = nRoot; // �ֽ�ȭ �� nRoot�� root�� ���ʳ��� ����
		}
		root->height = getHeight(root); // root�� ���� �ֽ�ȭ
		if (getHeight(root) <= 2) { // root�� ���̰� 3 �̸��̸� �������� �ʿ� ������ root�� return�ϰ� insert�Լ� ����
			return root;
		}
		else { // ���̰� 3 �̻��̹Ƿ� rebalance�� �ʿ������� �׽�Ʈ
			int bf = getHeight(root->left) - getHeight(root->right);
			if (bf > 1) { // LH ������ ��
				if (getHeight(root->left->left) > getHeight(root->left->right)) // Left of left ������ ��
				{
					nRoot = rotateRight(root);
				}
				else  // Right of left ������ ��
				{
					nRoot = rotateLeft(root->left);
					root->left = nRoot;
					nRoot = rotateRight(root);

				}
			}
			else if (bf < -1) { // RH ������ ��
				if (getHeight(root->right->left) < getHeight(root->right->right)) // Right of right ������ ��
				{

					nRoot = rotateLeft(root);
				}
				else // Left of right ������ ��
				{
					nRoot = rotateRight(root->right);
					root->right = nRoot;
					nRoot = rotateLeft(root);
				}

			}
			else return root;
		}
		nRoot->height = getHeight(nRoot);
		return nRoot;
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
	NODE* result = _retrieve(pTree->root, key);
	if (result == NULL) return NULL;
	return result->data;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, char *key) {
	if (root == NULL) return NULL;

	if (strcmp(key, root->data) >= 1)
		return _retrieve(root->right, key);
	else if (strcmp(key, root->data) <= -1)
		return _retrieve(root->left, key);
	else return root;
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
	for (int i = 0; i < level; i++) printf("\t");
	printf("%s\n", root->data);
	_infix_print(root->left, level + 1);
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE *root) {
	int lh, rh;
	if (root == NULL) return 0; // NULL�� ���̴� 0
	if (root->left == NULL) lh = 0; // ���� ����Ʈ���� ���̴� 0
	else lh = root->left->height; // ���� ����Ʈ���� �����ϸ� �� ����� ���̸� ����
	if (root->right == NULL) rh = 0; // ������ ����Ʈ���� ���̴� 0
	else rh = root->right->height;// ������ ����Ʈ���� �����ϸ� �� ����� ���̸� ����

	if (lh > rh) return lh + 1;
	return rh + 1;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight(NODE *root) {
	NODE* pNode = root;
	NODE* cNode = root->left;

	pNode->left = cNode->right;
	cNode->right = pNode;

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
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
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

