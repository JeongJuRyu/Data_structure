#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

typedef struct
{
	NODE* element[MAX_STACK_SIZE];
	int top;
}STACK;

typedef struct
{
	float element[MAX_STACK_SIZE];
	int top;
}STACK_FLOAT;
////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
TREE *createTree(void) {
	TREE* pTree = (TREE*)malloc(sizeof(TREE));
	if (pTree == NULL) return NULL;
	return pTree;
}

/* Deletes all data in tree and recycles memory
*/
void destroyTree(TREE *pTree);

static void _destroy(NODE *root) {
	if (root == NULL) return;
	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

/*  Allocates dynamic memory for a node and returns its address to caller
return	node pointer
NULL if overflow
*/
static NODE *_makeNode(char ch) {
	NODE* eNode = (NODE*)malloc(sizeof(NODE));
	eNode->left = NULL;
	eNode->right = NULL;
	eNode->data = ch;
	return eNode;
}

/* converts postfix expression to binary tree
return	1 success
0 invalid postfix expression
*/
int postfix2tree(char *expr, TREE *pTree) {
	STACK stack;
	stack.top = -1;
	int index = 0;
	while (expr[index] != '\0') {
		if (index == 0) {
			if (!isdigit(expr[index])) return 0; //ó������ �����ڰ� ������ �ٷ� invalid
			stack.element[++stack.top] = _makeNode(expr[index]);
			index++;
			continue;
		}
		NODE* tempNode = _makeNode(expr[index]);
		if (isdigit(expr[index])) { //������ ��� push
			stack.element[++stack.top] = tempNode;
		}
		else { // �������� ���, invalid üũ �ϰ� valid�� ���� Ʈ�� ����
			if (stack.top < 1) { //pop�� 2�� �ؾ��ϴµ� ������� �ʴٸ�
				if (stack.top == 0) free(stack.element[stack.top]);
				free(tempNode);
				return 0;
			}
			tempNode->right = stack.element[stack.top--];
			tempNode->left = stack.element[stack.top--];
			stack.element[++stack.top] = tempNode;
		}
		index++;
	}
	if (stack.top != 0) {
		while (stack.top >= 0) {
			_destroy(stack.element[stack.top--]);
		}
		return 0;
	}
	pTree->root = stack.element[0];
	return 1;
}

/* Print node in tree using inorder traversal
*/
void traverseTree(TREE *pTree);

/* internal traversal function
an implementation of ALGORITHM 6-6
*/
static void _traverse(NODE *root) {
	if (root == NULL) {
		return;
	}
	if (!isdigit(root->data)) printf("(");
	_traverse(root->left);
	printf("%c", root->data);
	_traverse(root->right);
	if (!isdigit(root->data)) printf(")"); 
	
}

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE *pTree);

/* internal traversal function
*/
static void _infix_print(NODE *root, int level) {
	if (root == NULL) return;
	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++) printf("\t");
	printf("%c\n", root->data);
	_infix_print(root->left, level + 1);
}

/* evaluate postfix expression
return	value of expression
*/
float evalPostfix(char *expr) {
	STACK_FLOAT stack;
	stack.top = -1;
	int index = 0;
	while (expr[index] != '\0') {
		if (index == 0) {
			if (!isdigit(expr[index])) return 0; //ó������ �����ڰ� ������ �ٷ� invalid
			stack.element[++stack.top] = (float)(expr[index++]-'0');
			continue;
		}
		if (isdigit(expr[index])) { //������ ��� push
			stack.element[++stack.top] = (float)(expr[index] - '0');
		}
		else { // �������� ���, invalid üũ �ϰ� valid�� ���� Ʈ�� ����
			if (stack.top < 1) { //pop�� 2�� �ؾ��ϴµ� ������� �ʴٸ�
				printf("����");
				return 0;
			}
			float right = stack.element[stack.top--];
			float left = stack.element[stack.top--];
			float temp;
			switch (expr[index]) {
			case '+':
				temp = left + right;
				break;
			case '-':
				temp = left - right;
				break;
			case '*':
				temp = left * right;
				break;
			case '/':
				temp = left / right;
				printf("%f %f %f",left, right, temp);
				break;
			default:
				return 0; // �����ڸ� �߸� �Է��� ��� invalid
			}
			stack.element[++stack.top] = temp;
		}
		index++;
	}
	if (stack.top != 0) {
		return 0;
	}
	return stack.element[0];
}


////////////////////////////////////////////////////////////////////////////////
void destroyTree(TREE *pTree){
	if (pTree)
	{
		_destroy( pTree->root);
	}

	free(pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree(TREE *pTree)
{
	_infix_print(pTree->root, 0);

	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree(TREE *pTree)
{
	_traverse(pTree->root);

	return;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TREE *tree;
	char expr[1024];

	fprintf(stdout, "\nInput an expression (postfix): ");

	while (fscanf(stdin, "%s", expr) == 1)
	{
		tree = createTree();

		if (!tree)
		{
			printf("Cannot create tree\n");
			return 100;
		}
		// postfix expression -> expression tree
		int ret = postfix2tree(expr, tree);
		if (!ret)
		{
			fprintf(stdout, "invalid expression!\n");
			destroyTree(tree);
			fprintf(stdout, "\nInput an expression (postfix): ");
			continue;
		}

		// expression tree -> infix expression
		fprintf(stdout, "\nInfix expression : ");
		traverseTree(tree);

		// print tree with right-to-left infix traversal
		fprintf(stdout, "\n\nTree representation:\n");
		printTree(tree);

		// evaluate postfix expression
		float val = evalPostfix(expr);
		fprintf(stdout, "\nValue = %f\n", val);

		// destroy tree
		destroyTree(tree);

		fprintf(stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}
