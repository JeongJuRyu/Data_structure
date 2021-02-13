#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct 
{
	char	*name;
	int		freq;
} tName;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	void		*dataPtr;	//tName		*dataPtr;
	struct node	*llink;
	struct node	*rlink;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos; // unused
	NODE	*head;
	NODE	*rear;
	int		(*compare)(const void *, const void *); // used in _search function
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
static int _insert(LIST *pList, NODE *pPre, void *dataInPtr); //tName
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr); //tName
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu);
tName *createName(char *str, int freq);
void traverseList(LIST *pList, void(*callback)(const void *));
void print_name(const void *dataPtr);

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a list head node and returns its address to caller
return	head node pointer
NULL if overflow
*/


LIST *createList(int(*compare)(const void *, const void *))
{
	LIST* pList = (LIST*)malloc(sizeof(LIST));
	if (pList == NULL) return NULL; // overflow시 null 리턴
	pList->compare = compare;
	pList->count = 0;
	pList->head = NULL;
	pList->rear = NULL;
	return pList; //정상적으로 생성되었을 시 해당 주소값 리턴
}

/* Deletes all data in list and recycles memory
*/

void destroyList(LIST *pList)
{
	NODE* pNode = pList->head;
	NODE* pPre = NULL;
	for (int i = 0; i < pList->count; i++) {
		pPre = pNode;
		pNode = pNode->rlink;
		tName* pName = (tName*)(pPre->dataPtr);
		free(pName->name);
		free(pName);
		free(pPre);
	}
}

/* Inserts data into list
return	0 if overflow
1 if successful
2 if duplicated key
*/

int addNode(LIST *pList, void *dataInPtr, void(*callback)(const void *, const void *)) //tName
{
	int result;
	NODE* sNode = pList->head;
	tName* tempName = (tName*)dataInPtr;
	if (pList->count == 0) { // pList에 1개도 없다면 바로 삽입
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		newNode->dataPtr = dataInPtr;
		newNode->llink = NULL;
		newNode->rlink = NULL;
		pList->head = newNode;
		pList->rear = newNode;
		pList->count++;
		return 1;
	}
	else if (pList->count == 1) result = _insert(pList, pList->head, dataInPtr); // pList에 1개의 노드만 존재하는 경우
	else {
		//printf("%s %s\n", ((tName*)(sNode->dataPtr))->name, tempName->name);
		while (1) {
			int compare_value = pList->compare(tempName, sNode->dataPtr);
			if (!compare_value) { // 같은 이름을 찾았을 시 freq만 추가
				callback(sNode->dataPtr, dataInPtr);
				return 2;
			}
			else if (compare_value <= -1) { // 삽입 될 위치 찾음
				if (sNode == pList->head) result = _insert(pList, sNode, dataInPtr);
				else result = _insert(pList, sNode->llink, dataInPtr);
				if (result) return 1;
				else return 0;
			}
			else { // sNode가 마지막 노드면 
				if (sNode->rlink == NULL) {
					result = _insert(pList, sNode, dataInPtr);
					if (result) return 1;
					else return 0;
				}
				else sNode = sNode->rlink;
			}
		}
	}
}

/* internal insert function
inserts data into a new node
return	1 if successful
0 if memory overflow
1. pPre가 head인 경우
2. pPre가 중간인 경우
3. pPre가 마지막 노드인 경우
*/

static int _insert(LIST *pList, NODE *pPre, void *dataInPtr) {
	tName* tempName = (tName*)dataInPtr; // 새로 삽입 될 tName의 정보를 담음. 형변환
	NODE* newNode = (NODE*)malloc(sizeof(NODE)); // 새로 삽입 될 노드
	newNode->llink = NULL;
	newNode->rlink = NULL;
	tName* nameToInserted = createName(tempName->name, tempName->freq);
	if (newNode == NULL || nameToInserted == NULL) return 0;
	newNode->dataPtr = nameToInserted;
	int compare_value = pList->compare(tempName, pPre->dataPtr);
	if (pList->count == 1) {
		if(compare_value <= -1){ // head 앞에 들어가야 하는 경우
			newNode->rlink = pList->head;
			pList->head->llink = newNode;
			pList->head = newNode;
		}
		else { // head 뒤에 들어가야 하는 경우
			newNode->llink = pList->head;
			pList->head->rlink = newNode;
			pList->rear = newNode;
		}
	}
	else { //노드가 2개 이상인 경우
		if (pPre == pList->head) { //pPre가 head인 경우 head 앞에 삽입될 지 뒤에 삽입될 지를 나눠야 한다.
			if (compare_value <= -1) { //head의 앞에 들어가는 경우
				newNode->rlink = pList->head;
				pList->head->llink = newNode;
				pList->head = newNode;
			}
			else { //head의 뒤에 들어가는 경우
				if (pList->compare(tempName, pPre->rlink->dataPtr) >= 1) pPre = pPre->rlink; // head를 가리키긴 했는데 head 다음 노드보다도 크면
				newNode->rlink = pPre->rlink;
				pPre->rlink->llink = newNode;
				newNode->llink = pPre;
				pPre->rlink = newNode;
			}
		}
		else if (pPre == pList->rear) {
			pPre->rlink = newNode;
			newNode->llink = pPre;
			pList->rear = newNode;
		}
		else {
			newNode->rlink = pPre->rlink;
			newNode->rlink->llink = newNode;
			pPre->rlink = newNode;
			newNode->llink = pPre;
		}
	}
	pList->count++;
	return 1;
}

/* internal delete function
deletes data from a list and saves the (deleted) data to dataOut
*/

static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr) {
	if (pPre == pList->head) {
		pList->head = pPre->rlink;
		pList->head->llink = NULL;
	}
	else if (pPre == pList->rear) {
		pList->rear = pPre->llink;
		pList->rear->rlink = NULL;
	}
	else {
		pPre->llink->rlink = pPre->rlink;
		pPre->rlink->llink = pPre->llink;
	}
	*dataOutPtr = pPre->dataPtr;
	free(pPre);
	
}//tName

/* internal search function
searches list and passes back address of node
containing target and its logical predecessor
return	1 found
0 not found
*/
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu) {
	*pPre = *pLoc; //pList의 head 가리킴.
	for (int i = 0; i < pList->count; i++) {
		if (pList->compare((*pPre)->dataPtr, pArgu) == 0) {
			return 1;
		}
		else {
			*pPre = (*pPre)->rlink;
		}
	}
	return 0;
}


/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode(LIST *pList, void *keyPtr, void **dataOut) {
	NODE* pLoc = pList->head;
	tName* nameToDeleted = (tName*)keyPtr;
	if (_search(pList, dataOut, &pLoc, nameToDeleted) == 0) return 0;
	else {
		_delete(pList, *dataOut, pLoc, dataOut);
		(pList->count)--;
		return 1;
	}

}
/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList(LIST *pList, void *pArgu, void **dataOutPtr) { //tName
	int result = _search(pList, dataOutPtr, &(pList->head), pArgu);
	if (!result) return 0;
	*dataOutPtr = ((NODE*)(*dataOutPtr))->dataPtr; //가져온 dataOutPtr에서 우리가 원하는 dataPtr부분을 추출한다.
	return 1;
}
/* returns number of nodes in list
*/
int countList(LIST *pList)
{
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST *pList) {
	if (pList->count == 0) return 1;
	return 0;
}

/* traverses data from list (forward)
*/
void traverseList(LIST *pList, void(*callback)(const void *)) {
	NODE* pNode = pList->head;
	for (int i = 0; i < pList->count; i++) {
		callback(pNode->dataPtr);
		pNode = pNode->rlink;
	}
}

/* traverses data from list (backward)
*/
void traverseListR(LIST *pList, void(*callback)(const void *)) {
	NODE* pNode = pList->rear;
	for (int i = 0; i < pList->count; i++) {
		callback(pNode->dataPtr);
		pNode = pNode->llink;
	}
}


/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
return	name structure pointer
NULL if overflow
*/

tName *createName(char *str, int freq) {
	tName* pName = (tName*)malloc(sizeof(tName));
	if (pName == NULL) return NULL;
	pName->freq = freq;
	pName->name = strdup(str);
	return pName;
}

/* Deletes all data in name structure and recycles memory
*/
void destroyName(tName *pNode) {
	free(pNode->name);
	free(pNode);
}

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'F':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'S':
			return SEARCH;
		case 'D':
			return DELETE;
		case 'C':
			return COUNT;
	}
	return 0; // undefined action
}

// compares two names in name structures
// for createList function
int cmpName( const void* pName1, const void* pName2)
{
	return strcmp( ((tName *)pName1)->name, ((tName *)pName2)->name);
}

// prints name and freq in name structure
// for traverseList and traverseListR functions
void print_name(const void *dataPtr)
{
	printf( "%s\t%d\n", ((tName *)dataPtr)->name, ((tName *)dataPtr)->freq);
}

// increases freq in name structure
// for addNode function
void increse_freq(const void *dataOutPtr, const void *dataInPtr)
{
	((tName *)dataOutPtr)->freq += ((tName *)dataInPtr)->freq;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	
	char str[1024];
	int freq;
	
	tName *pName;
	int ret;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	/*fp = fopen("name3.txt", "rt");
	if (!fp)
	{
		fprintf(stderr, "Error: cannot open file [%s]\n", "name3.txt");
		return 2;
	}*/
	// creates an empty list
	list = createList( cmpName);
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	void *p;
	
	while(fscanf( fp, "%*d\t%s\t%*c\t%d", str, &freq) != EOF)
	{
		pName = createName( str, freq);
		
		ret = addNode( list, pName, increse_freq);

		if (ret == 2) // duplicated
		{
			destroyName( pName);
		}
	}

	fclose( fp);
	
	fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	
	while (1)
	{
		void *p;
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				traverseList( list, print_name);
				break;
			
			case BACKWARD_PRINT:
				traverseListR( list, print_name);
				break;
			
			case SEARCH:
				fprintf( stderr, "Input a string to find: ");
				fscanf( stdin, "%s", str);

				pName = createName( str, 0);

				if (searchList( list, pName, &p))
				{
					print_name( p);
				}
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
				
			case DELETE:
				fprintf( stderr, "Input a string to delete: ");
				fscanf( stdin, "%s", str);
				
				pName = createName( str, 0);

				if (removeNode( list, pName, &p))
				{
					fprintf( stdout, "(%s, %d) deleted\n", ((tName *)p)->name, ((tName *)p)->freq);
					destroyName( (tName *)p);
				}
				else fprintf( stdout, "%s not found\n", str);
				
				destroyName( pName);
				break;
			
			case COUNT:
				fprintf( stdout, "%d\n", countList( list));
				break;
		}
		
		if (action) fprintf( stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
