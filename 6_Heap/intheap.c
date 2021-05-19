#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heapCreate(int capacity)
{
	HEAP* newHeap = (HEAP*)malloc(sizeof(HEAP));
	if (newHeap == NULL) return NULL;
	newHeap->heapArr = (int*)malloc(sizeof(int)*capacity);
	if (newHeap->heapArr == NULL) return NULL;
	newHeap->last = -1;
	newHeap->capacity = capacity;
	return newHeap;
}

/* Free memory for heap
*/
void heapDestroy(HEAP *heap) {
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP *heap, int data) {
	heap->last++;
	if (heap->last == heap->capacity) return 0;
	heap->heapArr[heap->last] = data;
	_reheapUp(heap, heap->last);
	return 1;
}

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP *heap, int index) {
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (heap->heapArr[parent] >= heap->heapArr[index]) break;
		int temp = heap->heapArr[parent];
		heap->heapArr[parent] = heap->heapArr[index];
		heap->heapArr[index] = temp;
		index = parent;
	}
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP *heap, int* dataOut) {
	*dataOut = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[heap->last--];
	if (heap->last == -1) return 0; // 1개를 삭제해서 heap이 빈 경우 0을 return
	int index = 0;
	int child = 0;
	if (heap->last >= index * 2 + 1) child = index * 2 + 1;
	if (heap->heapArr[index * 2 + 1] < heap->heapArr[index * 2 + 2]) child = index * 2 + 2;
	while(index <= heap->last) {
		int child = heap->heapArr[]
	}
}

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index);

/* Print heap array */
void heapPrint( HEAP *heap)
{
	int i;
	int last = heap->last;
	
	for( i = 0; i <= last; i++) 
	{
		printf("%6d", heap->heapArr[i]);
	}
	printf( "\n");
}

int main(void)
{
	HEAP *heap;
	int data;
	int i;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		//heapPrint( heap);
 	}

	//while (heap->last >= 0)
	{
		// delete function call
		//heapDelete( heap, &data);

		printf( "Deleting %d: ", data);

		//heapPrint( heap);
 	}
	
	//heapDestroy( heap);
	
	return 0;
}



