#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

static void _infix_print(NODE *root, int level);
static void _destroy(NODE *root);
static void _insert(NODE *root, NODE *newPtr);
static void _traverse(NODE *root);
static void _infix_print(NODE *root, int level);
NODE *_makeNode(int data);
static NODE *_delete(NODE *root, int dltKey, int *success);
/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create(void) {
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	newTree->root = NULL;
	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy(TREE *pTree) {
	_destroy(pTree->root);
	free(pTree);
}

/* internal function (not mandatory)
*/
static void _destroy(NODE *root) {
	if (root == NULL) return;
	_destroy(root->right);
	_destroy(root->left);
	free(root);
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE *pTree, int data) {
	NODE* newNode = _makeNode(data);
	if (newNode == NULL) return 0;
	if (pTree->root == NULL) {
		pTree->root = newNode;
	}
	else _insert(pTree->root, newNode);
	return 1;
}

/* internal function (not mandatory)
*/
static void _insert(NODE *root, NODE *newPtr) {
	if (root->data > newPtr->data) {
		if (root->left == NULL) {
			root->left = newPtr;
		}
		else _insert(root->left, newPtr);
	}
	else if (root->data <= newPtr->data) {
		if (root->right == NULL) {
			root->right = newPtr;
		}
		else _insert(root->right, newPtr);
	}
}

NODE *_makeNode(int data) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE *pTree, int dltKey) {
	int success;
	pTree->root = _delete(pTree->root, dltKey, &success);
	if ((success)) return 1;
	else return 0;
	
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete(NODE *root, int dltKey, int *success) {
	if (root->data == dltKey) {
		*success = 1;
		if (root->left == NULL) {
			if (root->right == NULL) { // 자식 노드 x
				free(root);
				return NULL;
			}
			else { // 오른쪽 자식노드만 있는 경우
				NODE* temp = root;
				root = root->right;
				free(temp);
				return root;
			}
		}
		else {
			if (root->right == NULL) { //왼쪽 자식노드만 있는 경우
				NODE* temp = root;
				root = root->left;
				free(temp);
				return root;
			}
			else { //양쪽 자식노드 모드 존재
				NODE* tempNode = root->right;
				NODE* parentNode = root;
				while (tempNode->left != NULL) {
					parentNode = tempNode;
					tempNode = tempNode->left;

				}
				if (parentNode->right == tempNode){ // 부모노드의 오른쪽 자식이 tempNode인 경우
					parentNode->right = tempNode->right;
				}
				else { // 부모노드의 왼쪽 자식이 tempNode인 경우
					parentNode->left = tempNode->right;
				}
				root->data = tempNode->data;
				free(tempNode);
				return root;
			}
		}
	}
	NODE* parentNode = NULL;
	NODE* deleteNode = root;
	while (deleteNode != NULL) { 
		if (deleteNode->data == dltKey) { //키를 찾은 경우
			*success = 1;
			if (deleteNode->left == NULL && deleteNode->right == NULL) { // 자식노드가 없다면
				if (parentNode->left == deleteNode) parentNode->left = NULL;
				else  parentNode->right = NULL;
				free(deleteNode);
			}
			else if (deleteNode->left == NULL && deleteNode->right != NULL) { //오른쪽 자식만 존재
				if (parentNode->right == deleteNode) {
					parentNode->right = deleteNode->right;
				}
				else parentNode->left = deleteNode->right;
				free(deleteNode);
			}
			else if (deleteNode->left != NULL && deleteNode->right == NULL) { //왼쪽 자식만 존재
				if (parentNode->right == deleteNode) {
					parentNode->right = deleteNode->left;
				}
				else parentNode->left = deleteNode->left;
				free(deleteNode);
			}
			else { // 두 자식 모두 존재
				NODE* exchangeNode = deleteNode->right;
				parentNode = deleteNode;
				while (exchangeNode->left != NULL) {
					parentNode = exchangeNode;
					exchangeNode = exchangeNode->left;
				}
				if (parentNode->right == exchangeNode) { // 부모노드의 오른쪽 자식이 tempNode인 경우
					parentNode->right = exchangeNode->right;
				}
				else { // 부모노드의 왼쪽 자식이 tempNode인 경우
					parentNode->left = exchangeNode->right;
				}
				deleteNode->data = exchangeNode->data;
				free(exchangeNode);
			}
			return root;
		}
		else if (deleteNode->data > dltKey) { // 왼쪽 서브트리로 탐색 이어가는 경우
			parentNode = deleteNode;
			deleteNode = deleteNode->left;
		}
		else { // 오른쪽 서브트리로 탐색 이어가는 경우
			parentNode = deleteNode;
			deleteNode = deleteNode->right;
		}
	}
	*success = 0;
	return root;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve(TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse(TREE *pTree) {
	_traverse(pTree->root);
}
static void _traverse(NODE *root) {
	if (root == NULL) return;
	_traverse(root->left);
	printf(" %d", root->data);
	_traverse(root->right);
}

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE *pTree) {
	_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
static void _infix_print(NODE *root, int level) {
	if (root == NULL) return;
	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++) printf("\t");
	printf("%d\n", root->data);
	_infix_print(root->left, level + 1);
}

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE *pTree) {
	if(pTree->root == NULL) return 1;
	else return 0;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	int data;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		//printf( "Cannot create a tree!\n");
		//return 100;
	}

	fprintf( stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf( "%d", &numbers);

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		int ret = BST_Insert( tree, data);
		if (!ret) break;
 	}
	fprintf( stdout, "\n");
			
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		int ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}


